FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  cmake \
  git \
  libopencv-dev \
  && rm -rf /var/lib/apt/lists/*

COPY . /app

WORKDIR /app

RUN mkdir build && cd build && \
    cmake .. && \
    make && \
    mv BRCli ..

EXPOSE 8080

WORKDIR /app

RUN chmod +x BRCli

CMD ["./BRCli"]
