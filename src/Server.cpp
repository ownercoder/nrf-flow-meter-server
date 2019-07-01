//
// Created by Сергей Квартников on 2019-06-30.
//

#include "Server.h"

Server::Server(boost::asio::io_service &io_service, const std::string &file)
        : io_service_(io_service),
          acceptor_(io_service, stream_protocol::endpoint(file)) {
    session_ptr new_session(new Session(io_service_));
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&Server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}

void Server::handle_accept(session_ptr new_session, const boost::system::error_code &error) {
    if (!error) {
        new_session->start();
    }

    new_session.reset(new Session(io_service_));
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&Server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}
