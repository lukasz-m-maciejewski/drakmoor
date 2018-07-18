#include "chain.hpp"

#include <catch.hpp>

TEST_CASE("basic composition")
{
    std::optional<int> x_opt = 2;
    auto add2 = [] (int a) { return a + 2; };
    auto sqr = [] (auto x) { return x * x; };

    auto res = mc::apply(x_opt, add2);
    REQUIRE( (res && (*res == 4)) );

    using namespace mc;
    std::optional<double> y = 3.0;
    auto res2 = (y >= sqr);
    REQUIRE( !!res2 );
    REQUIRE( *res2 == Approx(9.0));
}
