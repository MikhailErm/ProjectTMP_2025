# Указываем минимально необходимую версию CMake
cmake_minimum_required(VERSION 3.21)

# Название проекта и используемый язык программирования
project(EchoServer LANGUAGES CXX)

# Включаем автоматическую генерацию MOC, RCC и UIC для Qt
set(CMAKE_AUTOMOC ON)   # Автоматическая обработка файлов с макросами Q_OBJECT
set(CMAKE_AUTORCC ON)   # Автоматическая обработка ресурсов (если есть .qrc)
set(CMAKE_AUTOUIC ON)   # Автоматическая обработка .ui файлов (для GUI, не используется здесь)

# Пробуем найти Qt6, если не найден — пробуем Qt5
find_package(Qt6 COMPONENTS Network Core QUIET)  # Ищем Qt6 модули Network и Core
if(Qt6_FOUND)
    set(USE_QT6 TRUE)                            # Если Qt6 найден, устанавливаем флаг
else()
    find_package(Qt5 REQUIRED COMPONENTS Network Core)  # Иначе обязательно ищем Qt5
    set(USE_QT6 FALSE)                           # Устанавливаем флаг Qt5
endif()

# Указываем директорию, где находятся заголовочные файлы
include_directories(include)

# Определяем список исходных файлов проекта
set(SOURCES
    src/main.cpp           # Основной файл с функцией main
    src/mytcpserver.cpp    # Реализация TCP-сервера
    include/mytcpserver.h  # Заголовок TCP-сервера
    src/func2serv.cpp
    include/func2serv.h
)

# Создаем исполняемый файл из исходников
add_executable(EchoServer ${SOURCES}
    include/func2serv.h
    src/func2serv.cpp)


# Линкуем необходимые Qt-библиотеки в зависимости от версии Qt
if(USE_QT6)
    target_link_libraries(EchoServer PRIVATE Qt6::Network Qt6::Core)
else()
    target_link_libraries(EchoServer PRIVATE Qt5::Network Qt5::Core)
endif()

# Специальные настройки линковки для Windows
# Обеспечивает вывод в консоль при использовании MinGW (особенно в режиме Debug)
if(WIN32)
    target_link_options(EchoServer PRIVATE
        "$<$<CONFIG:Debug>:-Wl,--subsystem,console>"  # Указывает использовать консольное окно
    )
endif()
