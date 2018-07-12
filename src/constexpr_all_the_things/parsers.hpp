#ifndef DRAKMOOR_PARSERS
#define DRAKMOOR_PARSERS

#include <string_view>
#include <optional>
#include <variant>
#include <string>

namespace parsers
{

using parse_input_t = std::string_view;

template <typename T>
using parse_result_t = std::optional<std::pair<T, std::string_view>>;

template <typename P>
using opt_pair_parse_t = std::result_of_t<P(parse_input_t)>;
template <typename P>
using pair_parse_t = typename opt_pair_parse_t<P>::value_type;
template <typename P>
using parse_t = typename pair_parse_t<P>::first_type;

auto make_char_parser(char c)
{
    return [=] (parse_input_t s) -> parse_result_t<char>
    {
        if (s.empty() || s[0] != c) return std::nullopt;

        return parse_result_t<char>(std::make_pair(c, parse_input_t(s.data()+1, s.size()-1)));
    };
}

auto one_of(std::string_view chars)
{
    return [=] (parse_input_t s) -> parse_result_t<char>
    {
        auto j = chars.find(s[0]);
        if (j != parse_input_t::npos)
        {
            return parse_result_t<char>(std::make_pair(s[0], parse_input_t(s.data()+1, s.size()-1)));
        }
        return std::nullopt;
    };
}

auto none_of(std::string_view chars)
{
    return [=] (parse_input_t s) -> parse_result_t<char>
    {
        auto j = chars.find(s[0]);
        if (j == parse_input_t::npos)
        {
            return parse_result_t<char>(std::make_pair(s[0], parse_input_t(s.data()+1, s.size()-1)));
        }
        return std::nullopt;
    };
}

template <typename F, typename P>
auto fmap(F&& f, P&& p)
{
    using R = parse_result_t<std::result_of_t<F(parse_t<P>)>>;
    return [f = std::forward<F>(f),
            p = std::forward<P>(p)] (parse_input_t i) -> R
           {
               const auto r = p(i);
               if (!r) return std::nullopt;
               return R(std::make_pair(f(r->first), r->second));
           };
}

template <typename P, typename F>
auto bind(P&& p, F&& f)
{
    using R = std::result_of_t<F(parse_t<P>, parse_input_t)>;
    return [=] (parse_input_t i) -> R
           {
               const auto r = p(i);
               if (!r) return std::nullopt;
               return f(r->first, r->second);
           };
}

template <typename P1, typename P2, std::enable_if_t<std::is_same_v<parse_t<P1>, parse_t<P2>>, int> = 0>
auto operator|(P1&& p1, P2&& p2)
{
    return [=](parse_input_t i)
           {
               const auto r1 = p1(i);
               if (r1) return r1;
               return p2(i);
           };
}

template <typename T>
auto fail(T)
{
    return [=] (parse_input_t) -> parse_result_t<T>
    {
        return std::nullopt;
    };
}

template <typename P1, typename P2, typename F,
          typename R = std::result_of_t<F(parse_t<P1>, parse_t<P2>)>>
auto combine(P1&& p1, P2&& p2, F&& f)
{
    return [=] (parse_input_t i) -> parse_result_t<R>
    {
        const auto r1 = p1(i);
        if (!r1) return std::nullopt;
        const auto r2 = p2(r1->second);
        if (!r2) return std::nullopt;

        return parse_result_t<R>(std::make_pair(f(r1->first, r2->first), r2->second));
    };
}

template <typename P1, typename P2,
          typename = parse_t<P1>, typename = parse_t<P2>>
auto operator<(P1&& p1, P2&& p2)
{
    return combine(std::forward<P1>(p1),
                   std::forward<P2>(p2),
                   [] (auto, const auto& r) { return r; });
}

template <typename P1, typename P2,
          typename = parse_t<P1>, typename = parse_t<P2>>
auto operator>(P1&& p1, P2&& p2)
{
    return combine(std::forward<P1>(p1),
                   std::forward<P2>(p2),
                   [] (const auto& l, auto) { return l; });
}

namespace detail
{
template <typename P, typename T, typename F>
std::pair<T, parse_input_t> accumulate_parse(
    parse_input_t s, P&& p, T init, F&& f)
{
    while (!s.empty()) {
        const auto r = p(s);
        if (!r) return std::make_pair(init, s);
        init = f(init, r->first);
        s = r->second;
    }
    return std::make_pair(init, s);
}

template <typename P, typename T, typename F>
std::pair<T, parse_input_t> accumulate_n_parse(
    parse_input_t s, P&& p, std::size_t n, T init, F&& f)
{
    while (n != 0) {
        const auto r = p(s);
        if (!r) return std::make_pair(init, s);
        init = f(init, r->first);
        s = r->second;
        --n;
    }
    return std::make_pair(init, s);
}
}

// apply * (zero or more) of a parser, accumulating the results according to a
// function F. F :: T -> (parse_t<P>, parse_input_t) -> T
template <typename P, typename T, typename F>
auto many(P&& p, T&& init, F&& f)
{
    return [p = std::forward<P>(p),
           init = std::forward<T>(init),
           f = std::forward<F>(f)] (parse_input_t s)
           {
               return parse_result_t<T>(
                   detail::accumulate_parse(s, p, init, f));
           };
}

// apply + (one or more) of a parser, accumulating the results according to a
// function F. F :: T -> (parse_t<P>, parse_input_t) -> T
template <typename P, typename T, typename F>
auto many1(P&& p, T&& init, F&& f)
{
    return [p = std::forward<P>(p),
           init = std::forward<T>(init),
           f = std::forward<F>(f)] (parse_input_t s) -> parse_result_t<T>
    {
        const auto r = p(s);
        if (!r) return std::nullopt;
        return parse_result_t<T>(
            detail::accumulate_parse(r->second, p, f(init, r->first), f));
    };
}

auto skip_whitespace()
{
    auto ws_parser =
        make_char_parser(' ')
        | make_char_parser('\t')
        | make_char_parser('\n')
        | make_char_parser('\r');
    return many(ws_parser, std::monostate{}, [](auto m, auto) { return m; });
}

auto int_parser()
{
    using namespace std::string_view_literals;

    static constexpr auto digits = "0123456789"sv;
    static constexpr auto non_zero_digits = "123456789"sv;

    return bind(one_of(non_zero_digits),
                [] (char x, parse_input_t rest)
                {
                    return many(one_of(digits),
                                static_cast<int>(x - '0'),
                                [] (int acc, char c) { return (acc*10) + (c - '0'); })(rest);
                });
}

auto string_parser(parse_input_t s)
{
    using namespace std::string_view_literals;
    const auto quote_parser = make_char_parser('"');
    const auto str_parser =
        many(none_of("\""sv),
             std::string_view(s.data() + 1, 0),
             [] (const auto& acc, auto)
             {
                 return std::string_view(acc.data(), acc.size()+1);
             });

    return (quote_parser < str_parser > quote_parser)(s);
}

}

#endif // DRAKMOOR_PARSERS
