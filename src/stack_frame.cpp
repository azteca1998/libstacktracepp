//            Copyright Esteve Soler Arderiu 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <stacktracepp/stack_frame.hpp>

namespace stacktrace {

stack_frame::stack_frame() :
    _symbol(), _demangled_symbol(), _source_file(), _source_line(-1), _function_name(), _ip(
        0), _offset(0)
{
}

stack_frame::~stack_frame()
{
}

const std::string& stack_frame::symbol() const
{
    return _symbol;
}

const std::string& stack_frame::demangled_symbol() const
{
    return _demangled_symbol;
}

const std::string& stack_frame::source_file() const
{
    return _source_file;
}

const int& stack_frame::source_line() const
{
    return _source_line;
}

const std::string& stack_frame::function_name() const
{
    return _function_name;
}

const uint64_t& stack_frame::reg_ip() const
{
    return _ip;
}

const uint64_t& stack_frame::offset() const
{
    return _offset;
}

} /* namespace stacktrace */
