FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    libcurl4-openssl-dev \
    git \
    pkg-config \
    python3 \
    doxygen \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
    libasio-dev

# Install Crow
RUN git clone https://github.com/CrowCpp/crow.git /tmp/crow && \
    cd /tmp/crow && mkdir build && cd build && \
    cmake .. && make install && \
    rm -rf /tmp/crow

# Create the project directory with the same structure and data files
WORKDIR /app
COPY . .

RUN mkdir build
WORKDIR /app/build
RUN cmake .. && cmake --build .

RUN chmod +x /app/entrypoint.sh

CMD ["/app/entrypoint.sh"]
