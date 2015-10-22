//            Copyright Esteve Soler Arderiu 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STACK_FRAME_HPP_
#define STACK_FRAME_HPP_

#include <stdint.h>
#include <string>

namespace stacktrace {

class stack_frame
{
    friend class stack_trace;

public:
    ~stack_frame();

    const std::string& symbol() const;
    const std::string& demangled_symbol() const;

    const std::string& source_file() const;
    const int& source_line() const;
    const std::string& function_name() const;

    const unw_word_t& reg_ip() const;
    const unw_word_t& offset() const;

private:
    stack_frame();

    std::string _symbol;
    std::string _demangled_symbol;

    std::string _source_file;
    int _source_line;
    std::string _function_name;

    uint64_t _ip; // instruction pointer
    uint64_t _offset; // offset
};

} /* namespace stacktrace */

#endif /* STACK_FRAME_HPP_ */
