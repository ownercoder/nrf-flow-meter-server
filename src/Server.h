//
// Created by Сергей Квартников on 2019-06-30.
//

#ifndef NRFPROXY_SERVER_H
#define NRFPROXY_SERVER_H

#include <cstdio>
#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "Session.h"

using boost::asio::local::stream_protocol;

class Server {
public:
    Server(boost::asio::io_service &io_service, const std::string &file);

    void handle_accept(session_ptr new_session,
                       const boost::system::error_code &error);

private:
    boost::asio::io_service &io_service_;
    stream_protocol::acceptor acceptor_;
};


#endif //NRFPROXY_SERVER_H
