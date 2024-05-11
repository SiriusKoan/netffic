#include "monitor.hpp"
#include "IPv4Layer.h"
#include "Packet.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <format>
#include <iostream>

PacketStatsCollector::PacketStatsCollector() { stats = {}; }

void PacketStatsCollector::printResult() {
    std::cout << std::format("Total packet count: {}\n",
                             stats.totalPacketCount);
    int l3_cnt = stats.arpPacketCount + stats.grePacketCount +
                 stats.icmpPacketCount + stats.ipv4PacketCount +
                 stats.ipv6PacketCount + stats.vrrpPacketCount;
    std::cout << std::format("===== L3: {} =====\n", l3_cnt);
    std::cout << std::format("  ARP packet count: {} ({:.2f}%)\n",
                             stats.arpPacketCount,
                             (double)stats.arpPacketCount / l3_cnt * 100);
    std::cout << std::format("  GRE packet count: {} ({:.2f}%)\n",
                             stats.grePacketCount,
                             (double)stats.grePacketCount / l3_cnt * 100);
    std::cout << std::format("  ICMP packet count: {} ({:.2f}%)\n",
                             stats.icmpPacketCount,
                             (double)stats.icmpPacketCount / l3_cnt * 100);
    std::cout << std::format("  IPv4 packet count: {} ({:.2f}%)\n",
                             stats.ipv4PacketCount,
                             (double)stats.ipv4PacketCount / l3_cnt * 100);
    std::cout << std::format("  IPv6 packet count: {} ({:.2f}%)\n",
                             stats.ipv6PacketCount,
                             (double)stats.ipv6PacketCount / l3_cnt * 100);
    std::cout << std::format("  VRRP packet count: {} ({:.2f}%)\n",
                             stats.vrrpPacketCount,
                             (double)stats.vrrpPacketCount / l3_cnt * 100);
    int l4_cnt = stats.tcpPacketCount + stats.udpPacketCount;
    std::cout << std::format("===== L4: {} =====\n", l4_cnt);
    std::cout << std::format("  TCP packet count: {} ({:.2f}%)\n",
                             stats.tcpPacketCount,
                             (double)stats.tcpPacketCount / l4_cnt * 100);
    std::cout << std::format("  UDP packet count: {} ({:.2f}%)\n",
                             stats.udpPacketCount,
                             (double)stats.udpPacketCount / l4_cnt * 100);
    int l7_cnt = stats.dhcpPacketCount + stats.dnsPacketCount +
                 stats.ftpPacketCount + stats.httpPacketCount +
                 stats.ntpPacketCount + stats.smtpPacketCount +
                 stats.sshPacketCount + stats.genericPayloadPacketCount;
    std::cout << std::format("===== L7: {} =====\n", l7_cnt);
    std::cout << std::format("  DHCP packet count: {} ({:.2f}%)\n",
                             stats.dhcpPacketCount,
                             (double)stats.dhcpPacketCount / l7_cnt * 100);
    std::cout << std::format("  DNS packet count: {} ({:.2f}%)\n",
                             stats.dnsPacketCount,
                             (double)stats.dnsPacketCount / l7_cnt * 100);
    std::cout << std::format("  FTP packet count: {} ({:.2f}%)\n",
                             stats.ftpPacketCount,
                             (double)stats.ftpPacketCount / l7_cnt * 100);
    std::cout << std::format("  HTTP packet count: {} ({:.2f}%)\n",
                             stats.httpPacketCount,
                             (double)stats.httpPacketCount / l7_cnt * 100);
    std::cout << std::format("  NTP packet count: {} ({:.2f}%)\n",
                             stats.ntpPacketCount,
                             (double)stats.ntpPacketCount / l7_cnt * 100);
    std::cout << std::format("  SMTP packet count: {} ({:.2f}%)\n",
                             stats.smtpPacketCount,
                             (double)stats.smtpPacketCount / l7_cnt * 100);
    std::cout << std::format("  SSH packet count: {} ({:.2f}%)\n",
                             stats.sshPacketCount,
                             (double)stats.sshPacketCount / l7_cnt * 100);
    std::cout << std::format("  Generic payload packet count: {} ({:.2f}%)\n",
                             stats.genericPayloadPacketCount,
                             (double)stats.genericPayloadPacketCount / l7_cnt *
                                 100);
}

void PacketStatsCollector::consumePacket(pcpp::Packet &packet) {
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
    }
    if (packet.isPacketOfType(pcpp::NTP)) {
        stats.ntpPacketCount++;
    }
    if (packet.isPacketOfType(pcpp::SMTP)) {
        stats.smtpPacketCount++;
    }
    if (packet.isPacketOfType(pcpp::SSH)) {
        stats.sshPacketCount++;
    }
    if (packet.isPacketOfType(pcpp::GenericPayload)) {
        stats.genericPayloadPacketCount++;
    }
}
