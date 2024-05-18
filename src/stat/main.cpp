#include "httplib.h"
#include "nlohmann/json.hpp"
#include "pcapplusplus/ArpLayer.h"
#include "pcapplusplus/HttpLayer.h"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"
#include "pcapplusplus/IcmpLayer.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/PcapFileDevice.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/RawPacket.h"
#include "pcapplusplus/TcpLayer.h"
#include "pcapplusplus/UdpLayer.h"
#include "stat.hpp"
#include "utils.hpp"
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

PacketStats stats = {};
IPv4Stats ipv4Stats = {};
IPv6Stats ipv6Stats = {};
TCPStats tcpStats = {};
UDPStats udpStats = {};
HTTPStats httpStats = {};

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
        std::cerr << "Error opening the FIFO" << std::endl;
        return;
    }
    std::cout << "Pipe opened" << std::endl;
    std::cout << "Starting packet processing" << std::endl;
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
            ipv4Stats.srcIpCounter[ipv4Layer->getSrcIPAddress().toString()]++;
            ipv4Stats.dstIpCounter[ipv4Layer->getDstIPAddress().toString()]++;
        }
        if (packet.isPacketOfType(pcpp::IPv6)) {
            stats.ipv6PacketCount++;
            auto ipv6Layer = packet.getLayerOfType<pcpp::IPv6Layer>();
            ipv6Stats.srcIpCounter[ipv6Layer->getSrcIPAddress().toString()]++;
            ipv6Stats.dstIpCounter[ipv6Layer->getDstIPAddress().toString()]++;
        }
        if (packet.isPacketOfType(pcpp::VRRP)) {
            stats.vrrpPacketCount++;
        }
        // L4
        if (packet.isPacketOfType(pcpp::TCP)) {
            stats.tcpPacketCount++;
            auto tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
            tcpStats.srcPortCounter[tcpLayer->getSrcPort()]++;
            tcpStats.dstPortCounter[tcpLayer->getDstPort()]++;
        }
        if (packet.isPacketOfType(pcpp::UDP)) {
            stats.udpPacketCount++;
            auto udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
            udpStats.srcPortCounter[udpLayer->getSrcPort()]++;
            udpStats.dstPortCounter[udpLayer->getDstPort()]++;
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
                auto httpLayer =
                    packet.getLayerOfType<pcpp::HttpRequestLayer>();
                httpStats.requestCount++;
                auto method = httpLayer->getFirstLine()->getMethod();
                if (method == pcpp::HttpRequestLayer::HttpGET) {
                    httpStats.methodCounter["GET"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpPOST) {
                    httpStats.methodCounter["POST"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpHEAD) {
                    httpStats.methodCounter["HEAD"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpPUT) {
                    httpStats.methodCounter["PUT"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpDELETE) {
                    httpStats.methodCounter["DELETE"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpOPTIONS) {
                    httpStats.methodCounter["OPTIONS"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpTRACE) {
                    httpStats.methodCounter["TRACE"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpCONNECT) {
                    httpStats.methodCounter["CONNECT"]++;
                } else if (method == pcpp::HttpRequestLayer::HttpPATCH) {
                    httpStats.methodCounter["PATCH"]++;
                } else {
                    httpStats.methodCounter["UNKNOWN"]++;
                }
                httpStats.userAgentCounter[httpLayer
                                               ->getFieldByName(
                                                   PCPP_HTTP_USER_AGENT_FIELD)
                                               ->getFieldValue()]++;
                httpStats
                    .hostCounter[httpLayer->getFieldByName(PCPP_HTTP_HOST_FIELD)
                                     ->getFieldValue()]++;
            } else if (packet.isPacketOfType(pcpp::HTTPResponse)) {
                auto httpLayer =
                    packet.getLayerOfType<pcpp::HttpResponseLayer>();
                httpStats.responseCount++;
                httpStats.statusCounter[httpLayer->getFirstLine()
                                            ->getStatusCode()]++;
                httpStats.contentTypeCounter
                    [httpLayer->getFieldByName(PCPP_HTTP_CONTENT_TYPE_FIELD)
                         ->getFieldValue()]++;
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
}

void http_server(std::string addr, int port) {
    std::cout << "Starting HTTP server on " << addr << ":" << port << std::endl;
    httplib::Server svr;
    svr.Get("/stats/all", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(getAllStats(stats).dump(), "application/json");
    });
    svr.Get(
        "/stats/ipv4", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(getIPv4Stats(ipv4Stats).dump(), "application/json");
        });
    svr.Get(
        "/stats/ipv6", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(getIPv6Stats(ipv6Stats).dump(), "application/json");
        });
    svr.Get("/stats/tcp", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(getTCPStats(tcpStats).dump(), "application/json");
    });
    svr.Get("/stats/udp", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(getUDPStats(udpStats).dump(), "application/json");
    });
    svr.Get(
        "/stats/http", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(getHTTPStats(httpStats).dump(), "application/json");
        });
    svr.listen(addr.c_str(), port);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <address> <port>" << std::endl;
        return 1;
    }
    std::string addr = argv[1];
    int port = std::stoi(argv[2]);
    std::thread t1(receive_data);
    std::thread t2(http_server, addr, port);
    t1.join();
    // kill t2
    std::cout << "Stopping HTTP server" << std::endl;
    t2.detach();
    return 0;
}
