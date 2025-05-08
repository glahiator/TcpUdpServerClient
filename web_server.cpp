#include <boost/asio.hpp>
#include <string>
#include <thread>
#include "web_server.hpp"

int main() {
    boost::asio::io_context ioc;

    const auto address = boost::asio::ip::make_address("0.0.0.0");
    constexpr unsigned short port = 8080;

    boost::asio::ip::tcp::acceptor acceptor(ioc, {address, port});

    while (true) {
        boost::asio::ip::tcp::socket socket(ioc);
        acceptor.accept(socket);

        // Запускаем обработку взаимодействия с клиентом в отдельном потоке
        std::thread t(
            // Лямбда-функция будет выполняться в отдельном потоке
            [](boost::asio::ip::tcp::socket socket) {
                HandleConnection(socket);
            },
            std::move(socket));  // Сокет нельзя скопировать, но можно переместить

        // После вызова detach поток продолжит выполняться независимо от объекта t
        t.detach();
    }
}