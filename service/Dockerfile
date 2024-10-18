FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
  apt-get install -y \
  build-essential \
  cmake \
  git \
  libopencv-dev \
  libboost-all-dev \
  && rm -rf /var/lib/apt/lists/*

COPY . /app

WORKDIR /app

RUN mkdir build && cd build && \
    cmake .. && \
    make && \
    mv BRCli /app

WORKDIR /app

RUN chmod +x BRCli

CMD ["bash", "-c", "while true; do ./BRCli 0.0.0.0 8080; sleep 1; done"]

# CMD ["./BRCli", "--port" "8080"]

EXPOSE 8080