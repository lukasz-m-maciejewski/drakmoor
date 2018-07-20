#pragma once

namespace funexpr::parser
{
namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;

using x3::lit;

x3::rule<constant_class, ast::constant> const constant = "constant";
x3::rule<placeholder_class, ast::placeholder> const placeholder = "placeholder";
x3::rule<atom_class, ast::atom> const atom = "atom";

auto const constant_def = x3::double_;
auto const placeholder_def = +(ascii::char_('a', 'z'));

auto const atom_def = constant | placeholder;
}
