# Используем базовый образ Ubuntu
FROM ubuntu:latest

# Установка необходимых пакетов
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    ninja-build \
    cmake \
    qt5-qmake \
    qtbase5-dev \
    qtchooser \
    qt5-qmake-bin \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем исходный код в контейнер
COPY . .

# Собираем проект
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release

# Открываем порт 33333
EXPOSE 33333

# Запускаем приложение
CMD ["./build/EchoServer"]
