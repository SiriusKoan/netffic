#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include "utils.hpp"

nlohmann::json getAllStats(PacketStats& stats) {
    nlohmann::json j;
    j["totalPacketCount"] = stats.totalPacketCount;
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

nlohmann::json getIPv4Stats(IPv4Stats& stats) {
    nlohmann::json j;
    j["srcIpCounter"] = stats.srcIpCounter;
    j["dstIpCounter"] = stats.dstIpCounter;
    return j;
}

nlohmann::json getIPv6Stats(IPv6Stats& stats) {
    nlohmann::json j;
    j["srcIpCounter"] = stats.srcIpCounter;
    j["dstIpCounter"] = stats.dstIpCounter;
    return j;
}

nlohmann::json getHTTPStats(HTTPStats& stats) {
    nlohmann::json j;
    j["methodCounter"] = {};
    for (auto& [key, value] : stats.methodCounter) {
        j["methodCounter"][key] = value;
    }
    j["statusCounter"] = {};
    for (auto& [key, value] : stats.statusCounter) {
        j["statusCounter"][std::to_string(key)] = value;
    }
    return j;
}