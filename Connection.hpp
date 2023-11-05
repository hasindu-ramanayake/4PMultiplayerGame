#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Message.hpp"
#include "WebQueue.hpp"
#include "CommonInclude.hpp"

namespace mpgame {
    
    namespace net {

        template <typename T>
        class Connection: public std::enable_shared_from_this<Connection<T>> {
            
            public:
                Connection( OWNER parent_, asio::io_context& asioContext_, asio::ip::tcp::socket socket_, WebQueue<MessageOwned<T>>& qIn_  );
                virtual ~Connection();

                uint32_t getId() const { return _id; }
                OWNER getOwner() const { return _ownerType; }

                void connectToClient(uint32_t uid_ );
                void connectToServer(const asio::ip::tcp::resolver::results_type& endPoint_ );
                void disconnect();
                bool isConnected();
                void StartListening();

                void send(const Message<T>& msg_ );
            
            private:
                void addToIncomingMsgQ();
                void writeMsgHeader();
                void readMsgHeader();
                void writeMsgBody();
                void readMsgBody();

                asio::ip::tcp::socket _socket;
                asio::io_context& _ioContext;
                WebQueue< Message<T> > _msgOutQ;
                WebQueue< Message<T> > _msgInQ;
                Message<T> _tmpMsg;
                OWNER _ownerType;
                uint32_t _id;


        };

    }
}


#endif