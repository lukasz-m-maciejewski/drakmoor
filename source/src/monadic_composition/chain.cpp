#include "chain.hpp"

#include <iostream>

namespace testing
{
Wrap operator>=(const Wrap& lhs, const Wrap& rhs)
{
    std::cout << "Called >= with lhs:" << lhs.v << " rhs:" << rhs.v << '\n';
    return Wrap(lhs.v + rhs.v);
}
}
