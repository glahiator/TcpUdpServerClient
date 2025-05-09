#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <syncstream>

namespace net = boost::asio;

// Запускает функцию fn на n потоках, включая текущий
template <typename Fn>
void RunWorkers(unsigned n, const Fn& fn) {
    n = std::max(1u, n);
    std::vector<std::jthread> workers;
    workers.reserve(n - 1);
    // Запускаем n-1 рабочих потоков, выполняющих функцию fn
    while (--n) {
        workers.emplace_back(fn);
    }
    fn();
}

int main() {
    using osync = std::osyncstream;

    net::io_context io;
    std::cout << "Eat. Thread id: " << std::this_thread::get_id() << std::endl;

    net::post(io, [] {  // Моем посуду
        osync(std::cout) << "Wash dishes. Thread id: " << std::this_thread::get_id() << std::endl;
    });

    net::post(io, [] {  // Прибираемся на столе
        osync(std::cout) << "Cleanup table. Thread id: " << std::this_thread::get_id() << std::endl;
    });

    net::post(io, [&io] {  // Пылесосим комнату
        osync(std::cout) << "Vacuum-clean room. Thread id: " << std::this_thread::get_id()
                         << std::endl;

        net::post(io, [] {  // После того, как пропылесосили, асинхронно моем пол
            osync(std::cout) << "Wash floor. Thread id: " << std::this_thread::get_id()
                             << std::endl;
        });

        net::post(io, [] {  // Асинхронно опустошаем пылесборник пылесоса
            osync(std::cout) << "Empty vacuum cleaner. Thread id: " << std::this_thread::get_id()
                             << std::endl;
        });
    });

    std::cout << "Work. Thread id: " << std::this_thread::get_id() << std::endl;

    RunWorkers(2, [&io] {  // Асинхронные операции выполняются двумя потоками
        io.run();
    });
    std::cout << "Sleep" << std::endl;
}
