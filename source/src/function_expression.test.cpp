#include <catch.hpp>
#include "function_expression.hpp"
#include <boost/spirit/home/x3.hpp>

namespace
{
constexpr auto epsilon = std::numeric_limits<base_type>::epsilon();
}

TEST_CASE("addition of constants", "[ch01]")
{
    auto expr = expression{1.0} + expression{2.0};

    REQUIRE((expr.eval_at({}) - 3.0 < epsilon));
}

TEST_CASE("simple expression with single variable - addition", "[ch01]")
{
    auto expr = expression{1.0} + expression{"x"};

    arg_map am = {{"x", 9.0}};

    REQUIRE((expr.eval_at(am) - 10.0 < epsilon));
}

TEST_CASE("simple expression with single variable - subtraction", "[ch01]")
{
    auto expr = expression("x") - expression(123.1);

    arg_map am = {{"x", 123.1}};

    REQUIRE((expr.eval_at(am) < epsilon));
}

TEST_CASE("simple expression with single variable - multiplication", "[ch01]")
{
    auto expr = expression{1.0} + expression{"x"} * expression{"x"};

    arg_map am = {{"x", 2.0}};

    REQUIRE((expr.eval_at(am) - 5.0 < epsilon));
}

TEST_CASE("simple expression with single variable - division", "[ch01]")
{
    auto expr = expression("x") / expression(2.0);

    arg_map am = {{"x", 4.0}};

    REQUIRE((expr.eval_at(am) - 2.0 < epsilon));
}

// TEST_CASE("constexpr summing")
// {
//     int x = sum<1, 2, 3, 4, 5>();
//     REQUIRE(x == 15);

//     int y = sum<2353>();
//     REQUIRE(y == 2353);
// }
