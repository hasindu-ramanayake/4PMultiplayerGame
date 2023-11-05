#ifndef COMMON_INCLUDE_HPP
#define COMMON_INCLUDE_HPP

// remove this later and add one by one
#include <bits/stdc++.h>  

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

namespace mpgame {

    enum class OWNER {
        SERVER, 
        CLIENT
    };

    class Log {
        public:    
            static void printInfoLog( const std::string &log_ ){
                std::cout << "INFO: " << log_ << std::endl;
            }

            static void printErrorLog( const std::string &log_ ){
                std::cout << "ERROR: " << log_ << std::endl;
            }
    }; 
}

#endif