//
// Created by Сергей Квартников on 2019-06-30.
//

#include "Session.h"

Session::Session(boost::asio::io_service &io_service) : socket_(io_service) {

}

stream_protocol::socket &Session::socket() {
    return socket_;
}

void Session::start() {
    socket_.async_read_some(boost::asio::buffer(data_),
                            boost::bind(&Session::handle_read,
                                        shared_from_this(),
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
    if (!error) {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(data_, bytes_transferred),
                                 boost::bind(&Session::handle_write,
                                             shared_from_this(),
                                             boost::asio::placeholders::error));
    }
}

void Session::handle_write(const boost::system::error_code &error) {
    if (!error) {
        socket_.async_read_some(boost::asio::buffer(data_),
                                boost::bind(&Session::handle_read,
                                            shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
}