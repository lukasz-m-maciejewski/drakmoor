/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include "error_handler.hpp"

#include <boost/spirit/home/x3.hpp>

namespace rexpr::parser
{
// Our Iterator Type
using iterator_type = std::string::const_iterator;

// The Phrase Parse Context
using phrase_context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;

// Our Error Handler
using error_handler_type = error_handler<iterator_type>;

// Combined Error Handler and Phrase Parse Context
using context_type = x3::context<error_handler_tag,
                                std::reference_wrapper<error_handler_type> const,
                                phrase_context_type>;
}

