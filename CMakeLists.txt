cmake_minimum_required(VERSION 3.21)

project(EchoServer LANGUAGES CXX)

# Включаем автоматическую генерацию MOC, RCC и UIC для Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt
find_package(Qt6 COMPONENTS Network Core QUIET)
if(Qt6_FOUND)
    set(USE_QT6 TRUE)
else()
    find_package(Qt5 REQUIRED COMPONENTS Network Core)
    set(USE_QT6 FALSE)
endif()

# Указываем директорию с заголовочными файлами
include_directories(include)

# Определяем список исходных файлов проекта
set(SOURCES
    src/main.cpp
    src/mytcpserver.cpp
    include/mytcpserver.h
    src/func2serv.cpp
    include/func2serv.h
    src/databasemanager.cpp
    include/databasemanager.h
)

# Создаем исполняемый файл
add_executable(EchoServer ${SOURCES})

# Линковка Qt
if(USE_QT6)
    target_link_libraries(EchoServer PRIVATE Qt6::Network Qt6::Core)
else()
    target_link_libraries(EchoServer PRIVATE Qt5::Network Qt5::Core)
endif()

# Настройки для Windows
if(WIN32)
    target_link_options(EchoServer PRIVATE
        "$<$<CONFIG:Debug>:-Wl,--subsystem,console>"
    )
endif()
