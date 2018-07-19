/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include "ast.hpp"

#include <ostream>

namespace rexpr::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  Print out the rexpr tree
    ///////////////////////////////////////////////////////////////////////////
    int const tabsize = 4;

    struct rexpr_printer
    {
        typedef void result_type;

        rexpr_printer(std::ostream& p_out, int p_indent = 0)
          : out(p_out), indent(p_indent) {}

        void operator()(rexpr const& ast) const
        {
            out << '{' << std::endl;
            for (auto const& entry : ast.entries)
            {
                tab(indent+tabsize);
                out << '"' << entry.first << "\" = ";
                boost::apply_visitor(rexpr_printer(out, indent+tabsize), entry.second);
            }
            tab(indent);
            out << '}' << std::endl;
        }

        void operator()(std::string const& text) const
        {
            out << '"' << text << '"' << std::endl;
        }

        void tab(int spaces) const
        {
            for (int i = 0; i < spaces; ++i)
                out << ' ';
        }

        std::ostream& out;
        int indent;
    };
}
