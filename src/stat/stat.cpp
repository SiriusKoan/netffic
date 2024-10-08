#include "nlohmann/json.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>

nlohmann::json getMeta() {
    nlohmann::json j;
    j["l3"] = std::vector<std::string>{"ipv4", "ipv6"};
    j["l4"] = std::vector<std::string>{"tcp", "udp"};
    j["l7"] = std::vector<std::string>{"http"};
    return j;
}

nlohmann::json getAllStats(PacketStats &stats) {
    nlohmann::json j;
    j["total"] = {
        {"totalPacketCount", stats.totalPacketCount},
        {"totalByteCount", stats.totalByteCount},
    };
    j["l3"] = {
        {"arpPacketCount", stats.arpPacketCount},
        {"grePacketCount", stats.grePacketCount},
        {"icmpPacketCount", stats.icmpPacketCount},
        {"ipv4PacketCount", stats.ipv4PacketCount},
        {"ipv6PacketCount", stats.ipv6PacketCount},
        {"vrrpPacketCount", stats.vrrpPacketCount},
    };
    j["l4"] = {
        {"tcpPacketCount", stats.tcpPacketCount},
        {"udpPacketCount", stats.udpPacketCount},
    };
    j["l7"] = {
        {"dhcpPacketCount", stats.dhcpPacketCount},
        {"dnsPacketCount", stats.dnsPacketCount},
        {"ftpPacketCount", stats.ftpPacketCount},
        {"httpPacketCount", stats.httpPacketCount},
        {"ntpPacketCount", stats.ntpPacketCount},
        {"sshPacketCount", stats.sshPacketCount},
        {"genericPayloadPacketCount", stats.genericPayloadPacketCount},
    };
    return j;
}

nlohmann::json getIPv4Stats(IPv4Stats &stats) {
    nlohmann::json j;
    j["srcIpCounter"] = stats.srcIpCounter;
    j["dstIpCounter"] = stats.dstIpCounter;
    return j;
}

nlohmann::json getIPv6Stats(IPv6Stats &stats) {
    nlohmann::json j;
    j["srcIpCounter"] = stats.srcIpCounter;
    j["dstIpCounter"] = stats.dstIpCounter;
    return j;
}

nlohmann::json getTCPStats(TCPStats &stats) {
    nlohmann::json j;
    j["srcPortCounter"] = {};
    for (auto &[key, value] : stats.srcPortCounter) {
        std::string port = std::to_string(key);
        j["srcPortCounter"][port] = value;
    }
    j["dstPortCounter"] = {};
    for (auto &[key, value] : stats.dstPortCounter) {
        std::string port = std::to_string(key);
        j["dstPortCounter"][port] = value;
    }
    return j;
}

nlohmann::json getUDPStats(UDPStats &stats) {
    nlohmann::json j;
    j["srcPortCounter"] = {};
    for (auto &[key, value] : stats.srcPortCounter) {
        std::string port = std::to_string(key);
        j["srcPortCounter"][port] = value;
    }
    j["dstPortCounter"] = {};
    for (auto &[key, value] : stats.dstPortCounter) {
        std::string port = std::to_string(key);
        j["dstPortCounter"][port] = value;
    }
    return j;
}

nlohmann::json getHTTPStats(HTTPStats &stats) {
    nlohmann::json j;
    j["requestCount"] = stats.requestCount;
    j["responseCount"] = stats.responseCount;
    j["methodCounter"] = {};
    for (auto &[key, value] : stats.methodCounter) {
        j["methodCounter"][key] = value;
    }
    j["statusCounter"] = {};
    for (auto &[key, value] : stats.statusCounter) {
        j["statusCounter"][std::to_string(key)] = value;
    }
    j["userAgentCounter"] = {};
    for (auto &[key, value] : stats.userAgentCounter) {
        j["userAgentCounter"][key] = value;
    }
    j["hostCounter"] = {};
    for (auto &[key, value] : stats.hostCounter) {
        j["hostCounter"][key] = value;
    }
    j["contentTypeCounter"] = {};
    for (auto &[key, value] : stats.contentTypeCounter) {
        j["contentTypeCounter"][key] = value;
    }
    return j;
}