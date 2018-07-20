#include "function_expression.hpp"
#include <ostream>

namespace drakmoor
{

void constant::accept(expression_visitor& ev) const
{
    ev.visit(*this);
}

void placeholder::accept(expression_visitor& ev) const
{
    ev.visit(*this);
}

void compound::accept(expression_visitor& ev) const
{
    ev.visit(*this);
}

printer::printer(std::ostream& p_os) : os(p_os)
{
}

void printer::visit(const constant& c)
{
    os << c.value();
}

void printer::visit(const placeholder& p)
{
    os << p.label();
}

void printer::visit(const compound& b)
{
    const auto& atoms = b.get_atoms();
    if (atoms.size() == 2u)
    {
        os << '(';
        atoms[0]->accept(*this);
        os << ' ' << b.get_operation_label() << ' ';
        atoms[1]->accept(*this);
        os << ')';
    }
}

} // namespace drakmoor
