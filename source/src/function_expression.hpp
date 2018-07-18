#ifndef DRAKMOOR_FUNCTION_EXPRESSION
#define DRAKMOOR_FUNCTION_EXPRESSION

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <numeric>
#include <vector>
#include <string>

using base_type = double;
using arg_map = std::map<std::string, base_type>;

class expression;

class atom
{
public:
    virtual ~atom() = default;
    virtual base_type eval_at(const arg_map&) const = 0;
    virtual std::unique_ptr<atom> clone() const = 0;
};

class constant : public atom
{
public:
    constant(base_type v_init) : v{v_init}
    {
    }

    base_type eval_at(const arg_map&) const override
    {
        return v;
    }

    std::unique_ptr<atom> clone() const override
    {
        return std::make_unique<constant>(*this);
    }

private:
    base_type v;
};

class placeholder : public atom
{
public:
    placeholder(std::string id_init) : id{std::move(id_init)}
    {
    }

    base_type eval_at(const arg_map& point) const override
    {
        return point.at(id);
    }

    std::unique_ptr<atom> clone() const override
    {
        return std::make_unique<placeholder>(*this);
    }

private:
    std::string id;
};

using operation_t = std::function<base_type(base_type, base_type)>;

class compound : public atom
{
public:
    compound(operation_t operation_init, std::unique_ptr<atom> v1, std::unique_ptr<atom> v2)
        : operation{operation_init}
    {
        values.emplace_back(v1->clone());
        values.emplace_back(v2->clone());
    }

    compound(const compound& other)
        : operation{other.operation}
    {
        values.reserve(other.values.size());
        std::transform(other.values.begin(), other.values.end(), std::back_inserter(values),
                       [](const auto& other_value)
                       {
                           return other_value->clone();
                       });
    }

    base_type eval_at(const arg_map& point) const override
    {
        if (values.empty())
        {
            throw std::logic_error{"no values in compound"};
        }

        std::vector<base_type> evaluated_values;
        evaluated_values.reserve(values.size());

        std::transform(values.begin(), values.end(), std::back_inserter(evaluated_values),
                       [&](const auto& v)
                       {
                           return v->eval_at(point);
                       });

        return std::accumulate(evaluated_values.begin() + 1, evaluated_values.end(),
                               *evaluated_values.begin(),
                               operation);
    }

    std::unique_ptr<atom> clone() const override
    {
        return std::make_unique<compound>(*this);
    }

private:
    operation_t operation;
    std::vector<std::unique_ptr<atom>> values;
};

class expression
{
public:
    expression(base_type c)
        : v{std::make_unique<constant>(c)}
    {
    }

    expression(std::string id)
        : v{std::make_unique<placeholder>(id)}
    {
    }

    expression(operation_t op, expression e1, expression e2)
        : v{std::make_unique<compound>(op, std::move(e1.v), std::move(e2.v))}
    {
    }

    base_type eval_at(const arg_map& point)
    {
        return v->eval_at(point);
    }

    std::unique_ptr<atom> v;
};

expression operator+(expression e1, expression e2)
{
    return expression(std::plus<base_type>{}, std::move(e1), std::move(e2));
}

expression operator-(expression e1, expression e2)
{
    return expression(std::minus<base_type>{}, std::move(e1), std::move(e2));
}

expression operator*(expression e1, expression e2)
{
    return expression(std::multiplies<base_type>{}, std::move(e1), std::move(e2));
}

expression operator/(expression e1, expression e2)
{
    return expression(std::divides<base_type>{}, std::move(e1), std::move(e2));
}

// template 
// struct sum_helper(int acc)
// {
//     return acc;
// };

// template <int N, int... Ns>
// int sum_helper(int acc)
// {
//     // if constexpr (sizeof...(Ns) > 0)
//     return sum_helper<Ns...>(acc + N);
//     // else
//     //     return N;
// };


// template <int... Ns>
// int sum()
// {
//     return sum_helper<Ns...>(0);
// }

#endif // DRAKMOOR_FUNCTION_EXPRESSION