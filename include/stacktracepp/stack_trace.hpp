//            Copyright Esteve Soler Arderiu 2015
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STACK_TRACE_HPP_
#define STACK_TRACE_HPP_

#include <deque>

#include "stack_frame.hpp"

namespace stacktrace {

class stack_trace
{
public:
    stack_trace( unsigned int frames_skipped = 0,
        unsigned int frames_skipped_ignored = 0 );
    ~stack_trace();

    typedef std::deque<stack_frame>::const_iterator const_iterator;
    typedef std::deque<stack_frame>::const_reverse_iterator const_reverse_iterator;

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    const stack_frame& operator[]( unsigned int frame_number ) const;
    const stack_frame& at( unsigned int frame_number ) const;

    void write( std::ostream& out, std::string error = "" );  // Writes a java-style traceback to the stream

private:
    std::deque<stack_frame> _stack;
};

} /* namespace stacktrace */

#endif /* STACK_TRACE_HPP_ */
