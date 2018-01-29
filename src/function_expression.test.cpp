#include "function_expression.hpp"


TEST_CASE("addition with single variable", "[ch01]")
{
    auto expr = std::make_unique<constant>(1.0) + std::make_unique<constant>(2.0);

    REQUIRE(expr.eval_at({}) == 3.0);
}
