#include "utils.hpp"
#include <cstdlib>
#include <string>

std::string getDataPipePath() { return std::getenv("DATA_SOCKET_PATH"); }

DataPipe::DataPipe(std::string path) {
    path_ = path;
    if (mkfifo(path.c_str(), 0666) == -1) {
        throw std::runtime_error("Failed to create FIFO");
    }
    sockfd_ = open(path_.c_str(), O_RDWR);
    if (sockfd_ == -1) {
        throw std::runtime_error("Failed to open socket");
    }
}

DataPipe::~DataPipe() {
    close(sockfd_);
    unlink(path_.c_str());
}

void DataPipe::send(std::string message) {
    if (write(sockfd_, message.c_str(), message.size()) == -1) {
        throw std::runtime_error("Failed to send message");
    }
}

std::string DataPipe::receive() {
    char buffer[1024];
    int n = read(sockfd_, buffer, sizeof(buffer));
    if (n == -1) {
        throw std::runtime_error("Failed to receive message");
    }
    return std::string(buffer, n);
}
