#include "function_expression.hpp"

void constant::accept(expression_visitor& ev) const
{
    static_cast<void>(ev);
}

void placeholder::accept(expression_visitor& ev) const
{
    static_cast<void>(ev);
}

void compound::accept(expression_visitor& ev) const
{
    static_cast<void>(ev);
}
