# Use an Ubuntu base image
FROM amd64/ubuntu:24.04

# Set environment variables to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update the package list and install essential packages
RUN apt-get update && apt-get install -y \
    software-properties-common \
    wget \
    build-essential \
    apt-transport-https \
    ca-certificates \
    gnupg \
    lsb-release \
    curl \
    zlib1g-dev \
    libssl-dev \
    libffi-dev \
    libbz2-dev \
    libreadline-dev \
    libsqlite3-dev \
    libncurses5-dev \
    libncursesw5-dev \
    xz-utils \
    netcat-traditional \
    libgtest-dev \
    tk-dev && \
    rm -rf /var/lib/apt/lists/*

#install

# Add the PPA for GCC 11 and install it
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update && \
    apt-get install -y gcc-11 g++-11 && \
    rm -rf /var/lib/apt/lists/*

# Set GCC 11 as the default compiler
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100

    
# Install CMake 3.22.1
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1-linux-x86_64.sh && \
    chmod +x cmake-3.22.1-linux-x86_64.sh && \
    ./cmake-3.22.1-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.22.1-linux-x86_64.sh

RUN apt-get update && apt-get install -y libgtest-dev && \
    cd /usr/src/googletest/googletest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

# Install Boost 1.74.0
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.74.0/source/boost_1_74_0.tar.gz && \
    tar xzf boost_1_74_0.tar.gz && \
    cd boost_1_74_0 && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    cd .. && \
    rm -rf boost_1_74_0.tar.gz boost_1_74_0

# Install Python 3.10.6 from source
RUN wget https://www.python.org/ftp/python/3.10.6/Python-3.10.6.tgz && \
    tar xzf Python-3.10.6.tgz && \
    cd Python-3.10.6 && \
    ./configure --enable-optimizations && \
    make -j$(nproc) && \
    make altinstall && \
    cd .. && \
    rm -rf Python-3.10.6.tgz Python-3.10.6

# Verify the installations
RUN gcc --version && g++ --version && cmake --version && /usr/local/bin/python3.10 --version

WORKDIR /usr/src/app

# Set the default command to bash
CMD ["bash"]
