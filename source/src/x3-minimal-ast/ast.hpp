/*=============================================================================
    Copyright (c) 2002-2018 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>

namespace client::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  Our employee AST struct
    ///////////////////////////////////////////////////////////////////////////
    struct employee
    {
        int age;
        std::string forename;
        std::string surname;
        double salary;
    };

    using boost::fusion::operator<<;
}
