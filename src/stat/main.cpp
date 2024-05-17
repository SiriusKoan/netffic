#include "pcapplusplus/ArpLayer.h"
#include "pcapplusplus/HttpLayer.h"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/PcapFileDevice.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/RawPacket.h"
#include "pcapplusplus/TcpLayer.h"
#include "pcapplusplus/UdpLayer.h"
#include "utils.hpp"
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

IPv4Stats ipStats = {};
HTTPStats httpStats = {};
PacketStats stats = {};
std::unordered_map<std::string, std::string> ipToPlace;

bool running = true;

void receive_data() {
    if (access(getDataPipePath().c_str(), F_OK) == -1) {
        if (mkfifo(getDataPipePath().c_str(), 0666) == -1) {
            std::cerr << "Failed to create FIFO" << std::endl;
            return;
        }
    }
    pcpp::PcapFileReaderDevice pcapReader(getDataPipePath());
    if (!pcapReader.open()) {
        std::cerr << "Error opening the pcap file" << std::endl;
        return;
    }
    pcpp::RawPacket rawPacket;
    while (running && pcapReader.getNextPacket(rawPacket)) {
        pcpp::Packet packet(&rawPacket);
        // Increment the corresponding counter
        stats.totalPacketCount++;
        // L3
        if (packet.isPacketOfType(pcpp::ARP)) {
            stats.arpPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::GRE)) {
            stats.grePacketCount++;
        }
        if (packet.isPacketOfType(pcpp::ICMP)) {
            stats.icmpPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::IPv4)) {
            stats.ipv4PacketCount++;
            auto ipv4Layer = packet.getLayerOfType<pcpp::IPv4Layer>();
            ipStats.srcIpCounter[ipv4Layer->getSrcIPAddress().toString()]++;
            ipStats.dstIpCounter[ipv4Layer->getDstIPAddress().toString()]++;
        }
        if (packet.isPacketOfType(pcpp::IPv6)) {
            stats.ipv6PacketCount++;
        }
        if (packet.isPacketOfType(pcpp::VRRP)) {
            stats.vrrpPacketCount++;
        }
        // L4
        if (packet.isPacketOfType(pcpp::TCP)) {
            stats.tcpPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::UDP)) {
            stats.udpPacketCount++;
        }
        // L7
        if (packet.isPacketOfType(pcpp::DHCP)) {
            stats.dhcpPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::DNS)) {
            stats.dnsPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::FTP)) {
            stats.ftpPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::HTTP)) {
            stats.httpPacketCount++;
            if (packet.isPacketOfType(pcpp::HTTPRequest)) {
                auto httpLayer = packet.getLayerOfType<pcpp::HttpRequestLayer>();
                httpStats.methodCounter[httpLayer->getFirstLine()->getMethod()]++;
            } else if (packet.isPacketOfType(pcpp::HTTPResponse)) {
                auto httpLayer = packet.getLayerOfType<pcpp::HttpResponseLayer>();
                httpStats.statusCounter[httpLayer->getFirstLine()->getStatusCode()]++;
            }
        }
        if (packet.isPacketOfType(pcpp::NTP)) {
            stats.ntpPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::SSH)) {
            stats.sshPacketCount++;
        }
        if (packet.isPacketOfType(pcpp::GenericPayload)) {
            stats.genericPayloadPacketCount++;
        }
    }
    std::cout << "Pipe closed" << std::endl;
    std::cout << "Total packets: " << stats.totalPacketCount << std::endl;
    std::cout << "IPv4 packets: " << stats.ipv4PacketCount << std::endl;
    std::cout << "==================== src ====================" << std::endl;
    std::vector<std::pair<std::string, uint16_t>> srcIpCounterVec(
        ipStats.srcIpCounter.begin(), ipStats.srcIpCounter.end());
    std::sort(srcIpCounterVec.begin(), srcIpCounterVec.end(),
              [](auto const &a, auto const &b) { return a.second > b.second; });
    for (auto const &[ip, count] : srcIpCounterVec) {
        std::cout << "from " << ip << " -> " << count << std::endl;
    }
    std::cout << "==================== dst ====================" << std::endl;
    std::vector<std::pair<std::string, uint16_t>> dstIpCounterVec(
        ipStats.dstIpCounter.begin(), ipStats.dstIpCounter.end());
    std::sort(dstIpCounterVec.begin(), dstIpCounterVec.end(),
              [](auto const &a, auto const &b) { return a.second > b.second; });
    for (auto const &[ip, count] : dstIpCounterVec) {
        std::cout << "to " << ip << " -> " << count << std::endl;
    }
}

int main() {
    std::thread t1(receive_data);
    t1.join();
    return 0;
}
