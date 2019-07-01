//
// Created by Сергей Квартников on 2019-06-30.
//

#ifndef NRFPROXY_SESSION_H
#define NRFPROXY_SESSION_H

#include <cstdio>
#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::local::stream_protocol;

class Session : public boost::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_service &io_service);

    stream_protocol::socket &socket();

    void start();

    void handle_read(const boost::system::error_code &error,
                     size_t bytes_transferred);

    void handle_write(const boost::system::error_code &error);

private:
    // The socket used to communicate with the client.
    stream_protocol::socket socket_;

    // Buffer used to store data received from the client.
    boost::array<char, 1024> data_;
};

typedef boost::shared_ptr<Session> session_ptr;

#endif //NRFPROXY_SESSION_H
