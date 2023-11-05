#ifndef WEB_QUEUE_CPP
#define WEB_QUEUE_CPP

#include "CommonInclude.hpp"
#include "WebQueue.hpp"

template <typename T>
mpgame::net::WebQueue<T>::WebQueue() {
    std::scoped_lock lock(mutexQ);
    if ( !q.empty() ) {
        while ( !q.empty() ) q.pop();
    }
}

template <typename T>
mpgame::net::WebQueue<T>::~WebQueue() {
    std::scoped_lock lock(mutexQ);
    if ( !q.empty() ) {
        mpgame::Log::printInfoLog("Web Queue is not empty..!! cleaing it anyway");
    }
    while ( !q.empty() ) q.pop();
}

template <typename T>
const T& mpgame::net::WebQueue<T>::front() {
    std::scoped_lock lock(mutexQ);
    return q.front();
}

template <typename T>
const T& mpgame::net::WebQueue<T>::back() {
    std::scoped_lock lock(mutexQ);
    return q.back();
}

template <typename T>
void mpgame::net::WebQueue<T>::push(const T&  item_) {
    std::scoped_lock lock(mutexQ);
    q.push(item_ );
}

template <typename T>
T mpgame::net::WebQueue<T>::pop() {
    std::scoped_lock lock(mutexQ);
    auto backItem = std::move(q.front());
    q.pop();
    return backItem;
}

template <typename T>
int mpgame::net::WebQueue<T>::size() {
    std::scoped_lock lock(mutexQ);
    return q.size();
}

template <typename T>
bool mpgame::net::WebQueue<T>::empty() {
    std::scoped_lock lock(mutexQ);
    return q.empty();
}

#endif