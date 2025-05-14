# Базовый образ Ubuntu
FROM ubuntu:20.04

# Установка зависимостей
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    wget \
    ninja-build \
    qtbase5-dev \
    qttools5-dev-tools \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/Kitware/CMake/releases/download/v3.27.9/cmake-3.27.9-linux-x86_64.sh && \
    chmod +x cmake-3.27.9-linux-x86_64.sh && \
    ./cmake-3.27.9-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.27.9-linux-x86_64.sh

# Установка рабочей директории
WORKDIR /app

# Копируем исходники
COPY . .

# Сборка проекта
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release

# Указываем порт, который слушает сервер
EXPOSE 33333

# Уточни имя исполняемого файла (предположим "MyTcpServerApp")
CMD ["./build/EchoServer"]
