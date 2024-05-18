#pragma once

#include "pcapplusplus/HttpLayer.h"
#include <string>
#include <unordered_map>

// DELAY defined the capture delay in microseconds between each packet
// Lower value results in more precise capture but more CPU usage
#define DELAY 1000
// BUFFER_SIZE defines the size of the buffer used to read data from the pipe
#define BUFFER_SIZE 4096

std::string getDataPipePath();

class DataPipe {
  public:
    DataPipe(std::string path, mode_t mode);
    ~DataPipe();
    void create();
    void send(std::string message);
    void send(const void *data, size_t size);
    std::string receive();
    int receive(void *data);

  private:
    int sockfd_;
    std::string path_;
};

typedef struct {
    int totalPacketCount;
    // L3
    int arpPacketCount;
    int grePacketCount;
    int icmpPacketCount;
    int ipv4PacketCount;
    int ipv6PacketCount;
    int vrrpPacketCount;
    // L4
    int tcpPacketCount;
    int udpPacketCount;
    // L7
    int dhcpPacketCount;
    int dnsPacketCount;
    int ftpPacketCount;
    int httpPacketCount;
    int ntpPacketCount;
    int sshPacketCount;
    int genericPayloadPacketCount;
} PacketStats;

typedef struct {
    std::unordered_map<std::string, int> srcIpCounter;
    std::unordered_map<std::string, int> dstIpCounter;
} IPv4Stats;

typedef struct {
    std::unordered_map<std::string, int> srcIpCounter;
    std::unordered_map<std::string, int> dstIpCounter;
} IPv6Stats;

typedef struct {
    std::unordered_map<int, int> srcPortCounter;
    std::unordered_map<int, int> dstPortCounter;
} TCPStats;

typedef struct {
    std::unordered_map<int, int> srcPortCounter;
    std::unordered_map<int, int> dstPortCounter;
} UDPStats;

typedef struct {
    int requestCount;
    int responseCount;
    std::unordered_map<std::string, int> methodCounter;
    std::unordered_map<int, int> statusCounter;
    std::unordered_map<std::string, int> userAgentCounter;
    std::unordered_map<std::string, int> hostCounter;
    std::unordered_map<std::string, int> contentTypeCounter;
} HTTPStats;