cmake_minimum_required(VERSION 3.21)

project(EchoServer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# === Qt Setup ===
find_package(Qt6 COMPONENTS Network Core Sql Test QUIET)
if(Qt6_FOUND)
    set(USE_QT6 TRUE)
else()
    find_package(Qt5 REQUIRED COMPONENTS Network Core Sql Test)
    set(USE_QT6 FALSE)
endif()

# === Include path ===
include_directories(include)

# === Общие исходники ===
set(ECHOSERVER_SRC
    src/func2serv.cpp
    src/databasemanager.cpp
    src/gradientdescent.cpp
    src/taskmanager.cpp
    src/spline.cpp          # Добавлен spline.cpp
    src/RSA.cpp
)

set(ECHOSERVER_HDR
    include/func2serv.h
    include/databasemanager.h
    include/gradientdescent.h

    include/taskmanager.h
    include/spline.h        # Добавлен spline.h
    include/RSA.h
)

# === Общая библиотека с логикой ===
add_library(EchoServerLib ${ECHOSERVER_SRC} ${ECHOSERVER_HDR})

if(USE_QT6)
    target_link_libraries(EchoServerLib PRIVATE Qt6::Core Qt6::Sql Qt6::Network)
else()
    target_link_libraries(EchoServerLib PRIVATE Qt5::Core Qt5::Sql Qt5::Network)
endif()

# === Исполняемый файл сервера ===
add_executable(EchoServer
    src/main.cpp
    src/mytcpserver.cpp
    include/mytcpserver.h


    # spline.cpp сюда НЕ включаем — он в библиотеке
)

target_link_libraries(EchoServer PRIVATE EchoServerLib)

if(USE_QT6)
    target_link_libraries(EchoServer PRIVATE Qt6::Network Qt6::Core Qt6::Sql)
else()
    target_link_libraries(EchoServer PRIVATE Qt5::Network Qt5::Core Qt5::Sql)
endif()

if(WIN32)
    target_link_options(EchoServer PRIVATE "$<$<CONFIG:Debug>:-Wl,--subsystem,console>")
endif()

# === Юнит-тесты ===
enable_testing()

file(GLOB UNITTEST_SOURCES UnitTest/*.cpp)
source_group("UnitTest Files" FILES ${UNITTEST_SOURCES})

add_executable(UnitTest ${UNITTEST_SOURCES})

target_link_libraries(UnitTest PRIVATE EchoServerLib)

if(USE_QT6)
    target_link_libraries(UnitTest PRIVATE Qt6::Test Qt6::Core Qt6::Sql Qt6::Network)
else()
    target_link_libraries(UnitTest PRIVATE Qt5::Test Qt5::Core Qt5::Sql Qt5::Network)
endif()

add_test(NAME UnitTest COMMAND UnitTest)
