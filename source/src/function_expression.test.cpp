#include <catch.hpp>
#include "function_expression.hpp"
#include <boost/spirit/home/x3.hpp>
#include <sstream>

namespace
{
constexpr auto epsilon = std::numeric_limits<drakmoor::base_type>::epsilon();
}

TEST_CASE("addition of constants", "[simple_evaluation]")
{
    using namespace drakmoor;
    auto expr = expression{1.0} + expression{2.0};

    REQUIRE((expr.eval_at({}) - 3.0 < epsilon));
}

TEST_CASE("simple expression with single variable - addition", "[simple_evaluation]")
{
    using namespace drakmoor;
    auto expr = expression{1.0} + expression{"x"};

    arg_map am = {{"x", 9.0}};

    REQUIRE((expr.eval_at(am) - 10.0 < epsilon));
}

TEST_CASE("simple expression with single variable - subtraction", "[simple_evaluation]")
{
    using namespace drakmoor;
    auto expr = expression("x") - expression(123.1);

    arg_map am = {{"x", 123.1}};

    REQUIRE((expr.eval_at(am) < epsilon));
}

TEST_CASE("simple expression with single variable - multiplication", "[simple_evaluation]")
{
    using namespace drakmoor;
    auto expr = expression{1.0} + expression{"x"} * expression{"x"};

    arg_map am = {{"x", 2.0}};

    REQUIRE((expr.eval_at(am) - 5.0 < epsilon));
}

TEST_CASE("simple expression with single variable - division", "[simple_evaluation]")
{
    using namespace drakmoor;
    auto expr = expression("x") / expression(2.0);

    arg_map am = {{"x", 4.0}};

    REQUIRE((expr.eval_at(am) - 2.0 < epsilon));
}

TEST_CASE("printing", "[printing]")
{

SECTION("addition of constants")
{
    using namespace drakmoor;
    auto expr = expression{1.0} + expression{2.0};
    std::stringstream ss;
    printer p(ss);

    expr.accept(p);

    REQUIRE(ss.str() == "(1 + 2)");
}

SECTION("simple expression with single variable - addition")
{
    using namespace drakmoor;
    auto expr = expression{1.0} + expression{"x"};
    std::stringstream ss;
    printer p(ss);

    expr.accept(p);

    REQUIRE(ss.str() == "(1 + x)");
}

SECTION("simple expression with single variable - subtraction")
{
    using namespace drakmoor;
    auto expr = expression("x") - expression(123.1);
    std::stringstream ss;
    printer p(ss);

    expr.accept(p);

    REQUIRE(ss.str() == "(x - 123.1)");
}

SECTION("simple expression with single variable - multiplication")
{
    using namespace drakmoor;
    auto expr = expression{1.0} + expression{"x"} * expression{"x"};
    std::stringstream ss;
    printer p(ss);

    expr.accept(p);

    REQUIRE(ss.str() == "(1 + (x * x))");
}

SECTION("simple expression with single variable - division")
{
    using namespace drakmoor;
    auto expr = expression("x") / expression(2.0);
    std::stringstream ss;
    printer p(ss);

    expr.accept(p);

    REQUIRE(ss.str() == "(x / 2)");
}

}
