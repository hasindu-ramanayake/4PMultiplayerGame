#ifndef WEB_QUEUE_HPP
#define WEB_QUEUE_HPP

#include "CommonInclude.hpp"

namespace mpgame {
    namespace net { 
        template <typename T>
        class WebQueue {

            public:
                WebQueue();
                ~ WebQueue();

                const T& front();
                const T& back();
                void push(const T& item_ );
                T pop();
                int size();
                bool empty();

            private:
                std::queue<T> q;
                std::mutex mutexQ;

        };
    }
    
}




#endif