#include "utils.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string getDataPipePath() {
    return std::getenv("DATA_PIPE_PATH") ? std::getenv("DATA_PIPE_PATH")
                                         : "/tmp/netffic";
}

DataPipe::DataPipe(std::string path, mode_t mode) {
    path_ = path;
    create();
    sockfd_ = open(path_.c_str(), mode);
    if (sockfd_ == -1) {
        perror("Failed to open FIFO");
    }
}

DataPipe::~DataPipe() {
    close(sockfd_);
    unlink(path_.c_str());
}

void DataPipe::create() {
    if (access(path_.c_str(), F_OK) == -1) {
        if (mkfifo(path_.c_str(), 0666) == -1) {
            std::cerr << "Failed to create FIFO" << std::endl;
            return;
        }
    }
}

void DataPipe::send(std::string message) {
    if (write(sockfd_, message.c_str(), message.size()) == -1) {
        throw std::runtime_error("Failed to send message");
    }
}

void DataPipe::send(const void *data, size_t size) {
    // std::cout << "Sending data of size: " << size << std::endl;
    if (write(sockfd_, data, size) == -1) {
        throw std::runtime_error("Failed to send data");
    }
}

std::string DataPipe::receive() {
    char buffer[4096];
    int n = read(sockfd_, buffer, sizeof(buffer));
    if (n == -1) {
        return "";
    }
    return std::string(buffer, n);
}

int DataPipe::receive(void *data) {
    int n = read(sockfd_, data, BUFFER_SIZE);
    if (n == -1) {
        return -1;
    }
    return n;
}
