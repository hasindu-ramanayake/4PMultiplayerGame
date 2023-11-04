#ifndef MESSAGE_HPP

#include <CommonInclude.hpp>

//message namespace = mpgame::
namespace mpgame {
    namespace net {

        //message header
        template <typename T>
        struct MessageHeader {
            T id{};
            uint32_t size =0;
        };

        template <typename T>
        struct Message {
            MessageHeader<T> header;
            std::vector<uint8_t> body;

            size_t size() {
                return body.size();
            } 

            // good for cout
            friend std::ostream& operator << ( std::ostream& ss_, Message<T>& msg_ ) {
                ss_ << "id: " << static_cast<int>(msg_.header.id) << " size: " << msg_.header.size;
                return ss_;
            }

            // rewrite operator so its easy to add things to message
            template <typename U>
            friend Message<T>& operator << ( Message<T>& msg_, U& data_ ) {
                size_t i = msg_.body.size();
                msg_.body.resize( msg_.body.size() + sizeof(U) );
                std::memcpy( msg_.body.data() +i , &data_ , sizeof(U) );
                msg_.header.size = msg_.size();
                return msg_;
            }

            // rewrite operator so its easy to get things from message
            // extract data from back like a stack
            template <typename U>
            friend Message<T>& operator >> ( Message<T>& msg_, U& data_ ) {

                size_t i = msg_.body.size() - sizeof(U);
                std::memcpy( &data_, msg_.body.data()+i , sizeof(U) );
                msg_.body.resize(i);
                msg_.header.size = msg_.size();
                return msg_;
            }

        };
        
        template <typename T>
        class Connection;

        template <typename T>
        struct MessageOwned {
            std::shared_ptr<Connection<T> > remote = nullptr;
            Message<T> msg;

            friend std::ostream& operator << ( std::ostream& ss_, MessageOwned<T>& msg_ ) {
                ss_ << msg_.msg;
                return ss_;
            }

        };

    }
}

#endif