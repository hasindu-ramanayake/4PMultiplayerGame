#ifndef CONNECTION_CPP


#include "Connection.hpp"

template <typename T>
mpgame::net::Connection<T>::Connection( OWNER parent_ , 
                                        asio::io_context& asioContext_, 
                                        asio::ip::tcp::socket socket_,
                                        WebQueue<MessageOwned<T>>& qIn_  )
                                        : _ioContext(asioContext_),
                                          _socket( std::move(socket_) ),
                                          _msgInQ(qIn_), 
                                          _ownerType(mpgame::OWNER::SERVER), 
                                          _id(0) {

    _ownerType = parent_;
}

template <typename T>
mpgame::net::Connection<T>::~Connection() {} // for now keep this empty

template <typename T>
void mpgame::net::Connection<T>::connectToClient(uint32_t uid_ ) {
    if ( _ownerType == OWNER::SERVER ) {
        if ( _socket.is_open() ) {
            _id = uid_;
            readMsgHeader();
        }
    }
}

template <typename T>
void mpgame::net::Connection<T>::connectToServer(const asio::ip::tcp::resolver::results_type& endPoint_ ) {
    if ( _ownerType == OWNER::CLIENT ) {
        asio::async_connect( _socket, endPoint_, [this]( std::error_code ec, asio::ip::tcp::endpoint ep) {
            if ( !ec ) readMsgHeader();
        } );
    }
}

template <typename T>
void mpgame::net::Connection<T>::disconnect() {
    if ( isConnected() ) asio::post(_ioContext, [this]() {_socket.close(); });
}

template <typename T>
bool mpgame::net::Connection<T>::isConnected() {
    return _socket.is_open();
}

template <typename T>
void mpgame::net::Connection<T>::StartListening() {

}

template <typename T>
void mpgame::net::Connection<T>::send(const Message<T>& msg_ ) {
    asio::post( _ioContext, [this, msg_]() { 
        
        _msgOutQ.push(msg_);
        if ( !_msgOutQ.empty()) writeMsgHeader();
    });
}


template <typename T>
void mpgame::net::Connection<T>::addToIncomingMsgQ() {
    if ( _ownerType == OWNER::SERVER ) {
        _msgInQ.push({this->shared_from_this(), _tmpMsg });
    } else {
        _msgInQ.push({nullptr, _tmpMsg });
    }
    readMsgHeader();

}

template <typename T>
void mpgame::net::Connection<T>::writeMsgHeader() {
    asio::async_write( _socket, asio::buffer( &_msgOutQ.front().header, sizeof(MessageHeader<T>) ), 
    [this] (std::error_code ec, std::size_t len) {
        if ( !ec ) {
            if ( _msgOutQ.front().body.size > 0 ) {
                writeMsgBody();
            } else {
                _msgOutQ.pop();
                if ( !_msgOutQ.empty() ) writeMsgHeader();
            }
        } else {
            std::string err= "Write Header Fail" + ec.message();
            mpgame::Log::printErrorLog(err);
            _socket.close();
        }
    });
}

template <typename T>
void mpgame::net::Connection<T>::readMsgHeader() {
    asio::async_read( _socket, asio::buffer( _tmpMsg.header, sizeof(MessageHeader<T>) ), 
    [this] (std::error_code ec, std::size_t len) {
        if ( !ec ) {
            if ( _tmpMsg.header.size > 0 ) {
                _tmpMsg.body.resize(_tmpMsg.header.size);
                readMsgBody();
            } else {
                addToIncomingMsgQ();
            }
        } else {
            std::string err= "Read Header Fail" + ec.message();
            mpgame::Log::printErrorLog(err);
            _socket.close();
        }
    });

}

template <typename T>
void mpgame::net::Connection<T>::writeMsgBody() {
    asio::async_write( _socket, asio::buffer( _msgOutQ.front().body.data(), _msgOutQ.front().body.size() ), 
    [this] (std::error_code ec, std::size_t len) {
        if ( !ec ) { 
            _msgOutQ.pop();
            if ( !_msgOutQ.empty() ) writeMsgHeader();

        } else {
            std::string err= "Read Body Fail" + ec.message();
            mpgame::Log::printErrorLog(err);
            _socket.close();
        }
    });
}

template <typename T>
void mpgame::net::Connection<T>::readMsgBody() {
    asio::async_read( _socket, asio::buffer( _tmpMsg.body.data(), _tmpMsg.body.size()), 
    [this] (std::error_code ec, std::size_t len) {
        if ( !ec ) { 
            addToIncomingMsgQ();
        } else {
            std::string err= "Read Body Fail" + ec.message();
            mpgame::Log::printErrorLog(err);
            _socket.close();
        }
    });

}


#endif