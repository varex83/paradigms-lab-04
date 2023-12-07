# Use an official Ubuntu runtime as a parent image
FROM --platform=linux/amd64 ubuntu:20.04

# Set the working directory in the container
WORKDIR /usr/src/app

# Install a C++ compiler and necessary tools
RUN apt-get update && \
    apt-get install -y g++ wget

# Install CMake from the official website
RUN wget https://github.com/Kitware/CMake/releases/download/v3.26.0/cmake-3.26.0-Linux-x86_64.sh && \
    chmod +x cmake-3.26.0-Linux-x86_64.sh && \
    ./cmake-3.26.0-Linux-x86_64.sh --skip-license --prefix=/usr/local

# Copy the current directory contents into the container at /usr/src/app
COPY . .

RUN g++ -o main main.cpp

RUN chmod +x main

CMD ["./main"]
