#include "restaurant.hpp"
#include "hamburger.hpp"
#include <boost/asio.hpp>
namespace net = boost::asio;

int main() {
    net::io_context io;

    Restaurant restaurant{io};

    Logger logger{"main"s};
    auto print_result = [&logger](sys::error_code ec, int order_id, Hamburger* hamburger) {
        std::ostringstream os;
        if (ec) {
            os << "Order "sv << order_id << "failed: "sv << ec.what();
            return;
        }
        os << "Order "sv << order_id << " is ready. "sv << *hamburger;
        logger.LogMessage(os.str());
    };

    for (int i = 0; i < 4; ++i) {
        restaurant.MakeHamburger(i % 2 == 0, print_result);
    }
    io.run();
}