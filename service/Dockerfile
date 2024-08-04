FROM gcc:11

WORKDIR /app

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  cmake \
  git \
  libopencv-dev \
  && rm -rf /var/lib/apt/lists/*

COPY src/ /app/src
COPY include/ /app/include
COPY CMakeLists.txt /app

RUN mkdir build && cd build \
cmake .. \
make

RUN chmod +x /app/build/BRCli

EXPOSE 8080

CMD ["/app/bin/BRCli"]
