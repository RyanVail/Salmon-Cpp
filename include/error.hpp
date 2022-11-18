#include<iostream>

#ifndef ERROR_HPP
#define ERROR_HPP

// TODO: These errors need to be more advanced
namespace error
{
    inline void send_error(std::string error_message)
    {
        std::cout << error_message;
        exit(-1);
    }
}

#endif