#pragma once
#include "hamburger.hpp"
#include <functional>
#include <boost/asio.hpp>
namespace sys = boost::system;

// Функция, которая будет вызвана по окончании обработки заказа
using OrderHandler = std::function<void(sys::error_code ec, int id, Hamburger* hamburger)>;
