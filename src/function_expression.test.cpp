#include <catch/catch.hpp>
#include "function_expression.hpp"

TEST_CASE("addition of constants", "[ch01]")
{
    auto expr = expression{1.0} + expression{2.0};

    REQUIRE((expr.eval_at({}) - 3.0 < 0.0001));
}

TEST_CASE("simple expression with single variable - addition", "[ch01]")
{
    auto expr = expression{1.0} + expression{"x"};

    arg_map am = {{"x", 9.0}};

    REQUIRE((expr.eval_at(am) - 10.0 < 0.0001));
}

TEST_CASE("simple expression with single variable - multiplication", "[ch01]")
{
    auto expr = expression{1.0} + expression{"x"} * expression{"x"};

    arg_map am = {{"x", 2.0}};

    REQUIRE((expr.eval_at(am) - 5.0 < 0.0001));
}
