#pragma once

#include "IPv4Layer.h"
#include "Packet.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"

// DELAY defined the capture delay in microseconds between each packet
// Lower value results in more precise capture but more CPU usage
#define DELAY 30000

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
    int smtpPacketCount;
    int sshPacketCount;
    int genericPayloadPacketCount;
} PacketStats;

class PacketStatsCollector {
  public:
    PacketStatsCollector();
    void printResult();
    void consumePacket(pcpp::Packet &packet);

  private:
    PacketStats stats;
};
