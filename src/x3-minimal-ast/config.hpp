/*=============================================================================
    Copyright (c) 2001-2018 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <boost/spirit/home/x3.hpp>

namespace client::parser
{
    namespace x3 = boost::spirit::x3;

    using iterator_type = std::string::const_iterator;
    using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
}

