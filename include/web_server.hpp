#pragma once

#include <boost/asio.hpp>

void HandleConnection(boost::asio::ip::tcp::socket& socket);
