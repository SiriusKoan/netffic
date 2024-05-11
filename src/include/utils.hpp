#pragma once

#include <string>

std::string getDataPipePath();

class DataPipe {
  public:
    DataPipe(std::string path);
    ~DataPipe();
    void send(std::string message);
    std::string receive();

  private:
    int sockfd_;
    std::string path_;
};