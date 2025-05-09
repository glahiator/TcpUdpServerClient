#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
// Запрос, тело которого представлено в виде строки
using StringRequest = http::request<http::string_body>;
// Ответ, тело которого представлено в виде строки
using StringResponse = http::response<http::string_body>;

void HandleConnection(boost::asio::ip::tcp::socket& socket);

StringResponse HandleRequest(StringRequest&& req);
