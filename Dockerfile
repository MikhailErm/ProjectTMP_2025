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

# Указываем имя исполняемого файла
CMD ["./build/EchoServer"]

# Пошаговый запуск Dockerfile
# 1) Заходим в каталог с Dockerfile, и запускаем cmd из этого каталога
# 2) В командной строке вводим команду для сборки образа: docker build -t Задаем название(Допустим server):Пишем любой тег(Допустим 1.0) . (Пример: docker build -t server:1.0 .)
# 3) Запускаем контейнер командой: docker run -d -p 33333:33333 --name пишем имя контейнера(Допустим server) (Название образа):(Тег образа) (Пример: docker run -d -p 33333:33333 --name server server:1.0)
# 4) Остановить работу можно командой: docker stop название контейнера (Пример: docker stop server)