#pragma once

#include "ast.hpp"
#include "ast_adapted.hpp"
#include "error_handler.hpp"
#include "rexpr.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace rexpr::parser
{
namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::lexeme;
using x3::lit;

using ascii::char_;
using ascii::string;

///////////////////////////////////////////////////////////////////////////
// Rule IDs
///////////////////////////////////////////////////////////////////////////

struct rexpr_value_class;
struct rexpr_key_value_class;
struct rexpr_inner_class;

///////////////////////////////////////////////////////////////////////////
// Rules
///////////////////////////////////////////////////////////////////////////

x3::rule<rexpr_value_class, ast::rexpr_value> const rexpr_value = "rexpr_value";

x3::rule<rexpr_key_value_class, ast::rexpr_key_value> const rexpr_key_value =
    "rexpr_key_value";

x3::rule<rexpr_inner_class, ast::rexpr> const rexpr_inner = "rexpr";

rexpr_type const rexpr = "rexpr";

///////////////////////////////////////////////////////////////////////////
// Grammar
///////////////////////////////////////////////////////////////////////////

auto const quoted_string = lexeme['"' >> *(char_ - '"') >> '"'];

auto const rexpr_value_def = quoted_string | rexpr_inner;

auto const rexpr_key_value_def = quoted_string > '=' > rexpr_value;

auto const rexpr_inner_def = '{' > *rexpr_key_value > '}';

auto const rexpr_def = rexpr_inner_def;

//BOOST_SPIRIT_DEFINE(rexpr_value, rexpr, rexpr_inner, rexpr_key_value)

template <typename Iterator, typename Context, typename Attribute>
inline bool parse_rule(decltype(rexpr_value), Iterator& first, Iterator const& last,
                       Context const& context, Attribute& attr)
{
    using boost::spirit::x3::unused;
    static auto const def_ = (rexpr_value = rexpr_value_def);
    return def_.parse(first, last, context, unused, attr);
}

template <typename Iterator, typename Context, typename Attribute>
inline bool parse_rule(decltype(rexpr), Iterator& first, Iterator const& last,
                       Context const& context, Attribute& attr)
{
    using boost::spirit::x3::unused;
    static auto const def_ = (rexpr = rexpr_def);
    return def_.parse(first, last, context, unused, attr);
}

template <typename Iterator, typename Context, typename Attribute>
inline bool parse_rule(decltype(rexpr_inner), Iterator& first, Iterator const& last,
                       Context const& context, Attribute& attr)
{
    using boost::spirit::x3::unused;
    static auto const def_ = (rexpr_inner = rexpr_inner_def);
    return def_.parse(first, last, context, unused, attr);
}

template <typename Iterator, typename Context, typename Attribute>
inline bool parse_rule(decltype(rexpr_key_value), Iterator& first, Iterator const& last,
                       Context const& context, Attribute& attr)
{
    using boost::spirit::x3::unused;
    static auto const def_ = (rexpr_key_value = rexpr_key_value_def);
    return def_.parse(first, last, context, unused, attr);
}

///////////////////////////////////////////////////////////////////////////
// Annotation and Error handling
///////////////////////////////////////////////////////////////////////////

// We want these to be annotated with the iterator position.
struct rexpr_value_class : x3::annotate_on_success
{
};
struct rexpr_key_value_class : x3::annotate_on_success
{
};
struct rexpr_inner_class : x3::annotate_on_success
{
};

// We want error-handling only for the start (outermost) rexpr
// rexpr is the same as rexpr_inner but without error-handling (see error_handler.hpp)
struct rexpr_class : x3::annotate_on_success, error_handler_base
{
};
} // namespace rexpr::parser

namespace rexpr
{
parser::rexpr_type const& rexpr()
{
    return parser::rexpr;
}
} // namespace rexpr
