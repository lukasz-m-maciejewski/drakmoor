#pragma once

#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace funexpr::ast
{
namespace x3 = boost::spirit::x3;


using constant = double;
using placeholder = std::string;

struct atom : x3::variant<constant, placeholder>
{
};

}
