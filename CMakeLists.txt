cmake_minimum_required(VERSION 3.21)


project(EchoServer LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Пробуем найти Qt6, если не найден — пробуем Qt5
find_package(Qt6 COMPONENTS Network Core Sql Test QUIET) #  добавлен Sql, добавлен Test
if(Qt6_FOUND)
    set(USE_QT6 TRUE)
else()
    find_package(Qt5 REQUIRED COMPONENTS Network Core Sql Test)#  добавлен Sql, добавлен Test
    set(USE_QT6 FALSE)
endif()

# Указываем директорию, где находятся заголовочные файлы
include_directories(include)

# Определяем список исходных файлов проекта
set(SOURCES
    src/main.cpp
    src/mytcpserver.cpp
    src/func2serv.cpp
    src/databasemanager.cpp
    include/mytcpserver.h
    include/func2serv.h
    include/databasemanager.h
)

# Создаем исполняемый файл из исходников
add_executable(EchoServer ${SOURCES})

# Линкуем необходимые Qt-библиотеки
if(USE_QT6)
    target_link_libraries(EchoServer PRIVATE Qt6::Network Qt6::Core Qt6::Sql)  # ← Sql добавлен
else()
    target_link_libraries(EchoServer PRIVATE Qt5::Network Qt5::Core Qt5::Sql)  # ← Sql добавлен
endif()

# Специальные настройки линковки для Windows
if(WIN32)
    target_link_options(EchoServer PRIVATE
        "$<$<CONFIG:Debug>:-Wl,--subsystem,console>"
    )
endif()

# Тесты
enable_testing()

file(GLOB UNITTEST_SOURCES UnitTest/*.cpp)
source_group("UnitTest Files" FILES ${UNITTEST_SOURCES})

add_executable(UnitTest
    ${UNITTEST_SOURCES}
    src/func2serv.cpp
    src/databasemanager.cpp
    include/func2serv.h
    include/databasemanager.h
)


if(USE_QT6)
    target_link_libraries(UnitTest PRIVATE
        Qt6::Test
        Qt6::Core
        Qt6::Sql
        Qt6::Network
    )
else()
    target_link_libraries(UnitTest PRIVATE
        Qt5::Test
        Qt5::Core
        Qt5::Sql
        Qt5::Network
    )
endif()



add_test(NAME UnitTest COMMAND UnitTest)
