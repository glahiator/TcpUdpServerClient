#include "web_server.hpp"

#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;
namespace net = boost::asio;
using net::ip::tcp;


// Структура ContentType задаёт область видимости для констант,
// задающий значения HTTP-заголовка Content-Type
struct ContentType {
    ContentType() = delete;
    constexpr static std::string_view TEXT_HTML = "text/html"sv;
    // При необходимости внутрь ContentType можно добавить и другие типы контента
};
namespace {
    // Создаёт StringResponse с заданными параметрами
    StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version,
                                    bool keep_alive,
                                    std::string_view content_type = ContentType::TEXT_HTML) {
        StringResponse response(status, http_version);
        // Добавляем заголовок Content-Type:
        response.set(http::field::content_type, content_type);
        response.body() = body;
        // Формируем заголовок Content-Length, сообщающий длину тела ответа
        response.content_length(body.size());
        // Формируем заголовок Connection в зависимости от значения заголовка в запросе
        response.keep_alive(keep_alive);
        return response;
    }

    std::optional<StringRequest> ReadRequest(tcp::socket& socket, beast::flat_buffer& buffer) {
        beast::error_code ec;
        StringRequest req;
        // Считываем из socket запрос req, используя buffer для хранения данных.
        // В ec функция запишет код ошибки.
        http::read(socket, buffer, req, ec);

        if (ec == http::error::end_of_stream) {
            return std::nullopt;
        }
        if (ec) {
            throw std::runtime_error("Failed to read request: "s.append(ec.message()));
        }
        return req;
    }

    void DumpRequest(const StringRequest& req) {
        std::cout << req.method_string() << ' ' << req.target() << std::endl;
        // Выводим заголовки запроса
        for (const auto& header : req) {
            std::cout << "  "sv << header.name_string() << ": "sv << header.value() << std::endl;
        }
    }
}


void HandleConnection(boost::asio::ip::tcp::socket& socket) {
    try {
        // Буфер для чтения данных в рамках текущей сессии.
        beast::flat_buffer buffer;

        // Продолжаем обработку запросов, пока клиент их отправляет
        while (auto request = ReadRequest(socket, buffer)) {
            // Обрабатываем запрос и формируем ответ сервера
            DumpRequest(*request);

            StringResponse response = HandleRequest(*std::move(request));

            // Отправляем ответ сервера клиенту
            http::write(socket, response);

            // Прекращаем обработку запросов, если семантика ответа требует это
            if (response.need_eof()) {
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    beast::error_code ec;
    // Запрещаем дальнейшую отправку данных через сокет
    socket.shutdown(tcp::socket::shutdown_send, ec);
}

StringResponse HandleRequest(StringRequest&& req) {
    const auto text_response = [&req](http::status status, std::string_view text) {
        return MakeStringResponse(status, text, req.version(), req.keep_alive());
    };

    // Здесь можно обработать запрос и сформировать ответ, но пока всегда отвечаем: Hello
    return text_response(http::status::ok, "<strong>Hello</strong>"sv);
}
