cmake_minimum_required(VERSION 3.11)

# Проект называется TCP_UDP_Practice и написан на C++
project(TCP_UDP_Practice CXX)
# Исходый код будет компилироваться с поддержкой стандарта С++ 20
set(CMAKE_CXX_STANDARD 20)

# Подключаем сгенерированный скрипт conanbuildinfo.cmake, созданный Conan
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/../conan_toolchain.cmake)

# Ищем Boost версии 1.87
find_package(Boost 1.87.0 REQUIRED)
if(Boost_FOUND)
  # Boost найден, добавляем к каталогам заголовочных файлов проекта путь к
  # заголовочным файлам Boost
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# Boost.Beast будет использовать std::string_view вместо boost::string_view
add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)

include_directories(include)

# Добавляем четыре проекта
add_executable(TCPServer tcp_server.cpp)
add_executable(TCPClient tcp_client.cpp)
add_executable(UDPServer udp_server.cpp)
add_executable(UDPClient udp_client.cpp)
add_executable(WebServer web_server.cpp src/web_server.cpp)

# Просим компоновщик подключить библиотеку для поддержки потоков
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(TCPServer PRIVATE Threads::Threads)
target_link_libraries(TCPClient PRIVATE Threads::Threads)
target_link_libraries(UDPServer PRIVATE Threads::Threads)
target_link_libraries(UDPClient PRIVATE Threads::Threads)
target_link_libraries(WebServer PRIVATE Threads::Threads)
