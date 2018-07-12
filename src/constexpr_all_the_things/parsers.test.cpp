#include "parsers.hpp"
#include <iostream>

using namespace parsers;

int main()
{
    auto cp = make_char_parser('a');
    auto cp_res = cp("abc");
    std::cout << cp_res->first << " " << cp_res->second << "\n";

    auto cp_res2 = cp("def");

    if (cp_res2)
    {
        std::cout << cp_res2->first << " " << cp_res2->second << "\n";
    }
    else
    {
        std::cout << "failed to parse\n";
    }

    auto wskip = skip_whitespace();
    auto skipres = wskip("  \t  abc   def");
    std::cout << skipres->second << '\n';

    auto ip = int_parser();

    auto ip_res = ip("1203");
    static_assert(std::is_same_v<decltype(ip_res->first), int>, "");
    std::cout << ip_res->first << '\n';

    return 0;
}
