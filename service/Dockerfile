# Stage 1: Build stage
FROM ubuntu:20.04 as builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  cmake \
  git \
  libopencv-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY src/ /app/src
COPY include/ /app/include
COPY CMakeLists.txt /app

RUN mkdir build && cd build && cmake .. && make

# Stage 2: Run stage
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
  apt-get install -y \
  libopencv-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/bin/BRCli /app/bin/BRCli

RUN chmod +x /app/bin/BRCli

CMD ["./bin/BRCli"]