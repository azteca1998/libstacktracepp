#include <stacktracepp/stack_trace.hpp>

#include <cxxabi.h>
#include <ostream>
#include <stdlib.h>
#include <unistd.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <elfutils/libdwfl.h>

namespace stacktrace {

stack_trace::stack_trace( unsigned int frames_skipped_top,
    unsigned int frames_skipped_bottom )
{
    unw_context_t uc;
    unw_getcontext(&uc);

    unw_cursor_t cursor;
    unw_init_local(&cursor, &uc);

    while ( unw_step(&cursor) > 0 )
    {
        if ( frames_skipped_top != 0 )
        {
            frames_skipped_top--;
            continue;
        }

        stack_frame frame;

        unw_get_reg(&cursor, UNW_REG_IP, &frame._ip);
        if ( frame._ip == 0 )
        {
            break;
        }

        char symbol[256];
        if ( unw_get_proc_name(&cursor, symbol, sizeof( symbol ),
            &frame._offset) == 0 )
        {
            frame._symbol = symbol;

            {
                int status;
                char* demangled_symbol = abi::__cxa_demangle(symbol, NULL, NULL,
                    &status);
                if ( status != 0 && status != -2 )
                {
                    // TODO: Throw error (See: https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html)
                }
                else if ( status == -2 )  // Name cannot be demangled
                {
                    frame._demangled_symbol = symbol;
                }
                else
                {
                    frame._demangled_symbol = demangled_symbol;
                    free(demangled_symbol);
                }
            }

            char *debuginfo_path = NULL;

            Dwfl_Callbacks callbacks;
            callbacks.find_elf = dwfl_linux_proc_find_elf;
            callbacks.find_debuginfo = dwfl_standard_find_debuginfo;
            callbacks.debuginfo_path = &debuginfo_path;

            Dwfl* dwfl = dwfl_begin(&callbacks);
            if ( dwfl == NULL )
            {
                // TODO: Throw error
            }

            if ( dwfl_linux_proc_report(dwfl, getpid()) != 0 )
            {
                // TODO: Throw error
            }

            if ( dwfl_report_end(dwfl, NULL, NULL) != 0 )
            {
                // TODO: Throw error
            }

            Dwarf_Addr addr = (uintptr_t)frame._ip;
            Dwfl_Module* module = dwfl_addrmodule(dwfl, addr);
            frame._function_name = dwfl_module_addrname(module, addr);

            Dwfl_Line* line = dwfl_getsrc(dwfl, addr);
            if ( line != NULL )
            {
                Dwarf_Addr addr;
                frame._source_file = dwfl_lineinfo(line, &addr,
                    &frame._source_line, NULL, NULL, NULL);
            }

            dwfl_end(dwfl);
        }

        _stack.push_back(frame);
    }

    while ( frames_skipped_top != 0 )
    {
        _stack.pop_back();
        frames_skipped_top--;
    }
}

stack_trace::~stack_trace()
{
}

stack_trace::const_iterator stack_trace::begin() const
{
    return _stack.begin();
}

stack_trace::const_iterator stack_trace::end() const
{
    return _stack.end();
}

stack_trace::const_reverse_iterator stack_trace::rbegin() const
{
    return _stack.rbegin();
}

stack_trace::const_reverse_iterator stack_trace::rend() const
{
    return _stack.rend();
}

const stack_frame& stack_trace::operator[]( unsigned int frame_number ) const
{
    return _stack.operator [](frame_number);
}

const stack_frame& stack_trace::at( unsigned int frame_number ) const
{
    return _stack.at(frame_number);
}

void stack_trace::write( std::ostream& out, std::string error )
{
    if ( !error.empty() )
    {
        out << error << "" << std::endl;
    }

    for ( const_iterator it = _stack.begin(); it != _stack.end(); ++it )
    {
        std::string prefix = it->demangled_symbol();
        std::string suffix = ")";

        if ( it->demangled_symbol().find_first_of('(') != std::string::npos )
        {
            prefix = it->demangled_symbol().substr(0,
                it->demangled_symbol().find_first_of('(') + 1);
        }
        else
        {
            prefix.append("(");
        }

        if ( it->demangled_symbol().find_last_of(')') != std::string::npos )
        {
            suffix = it->demangled_symbol().substr(
                it->demangled_symbol().find_last_not_of(')') + 1);
        }

        std::string filename = it->source_file().substr(
            it->source_file().find_last_of('/') + 1);

        out << "\tat " << prefix;
        if ( filename.empty() )
        {
            out << "Unknown Source";
        }
        else
        {
            out << filename << ":";
            if ( it->source_line() == -1 )
            {
                out << "??";
            }
            else
            {
                out << it->source_line();
            }
        }
        out << suffix << std::endl;
    }
}

} /* namespace stacktrace */
