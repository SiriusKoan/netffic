#include "CLI/CLI.hpp"
#include "IPv4Layer.h"
#include "Packet.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include "monitor.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <signal.h>
#include <thread>

pcpp::PcapLiveDevice *dev;
PacketStatsCollector *collector;

void signalHandler(__attribute__((unused)) int signum) {
    std::cout << "Stopping capture..." << std::endl;
    dev->stopCapture();
    std::cout << "Printing results..." << std::endl;
    if (collector != nullptr) {
        collector->printResult();
    } else {
        std::cerr << "No collector found" << std::endl;
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    CLI::App app{"netffic"};
    std::string interface;
    app.add_option("-i,--interface", interface, "Interface to capture on")
        ->required();
    CLI11_PARSE(app, argc, argv);

    signal(SIGINT, signalHandler);

    dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(
        interface);
    if (dev == nullptr) {
        std::cerr << "Cannot find interface: " << interface << std::endl;
        return 1;
    }
    if (!dev->open()) {
        std::cerr << "Cannot open device" << std::endl;
        return 1;
    }

    std::cout << "Interface info:" << std::endl
              << "  Name: " << dev->getName() << std::endl
              << "  Description: " << dev->getDesc() << std::endl
              << "  MAC address: " << dev->getMacAddress() << std::endl
              << "  Default gateway: " << dev->getDefaultGateway() << std::endl
              << "  IP address: " << dev->getIPv4Address() << std::endl
              << "  MTU: " << dev->getMtu() << std::endl;
    std::cout << "Capturing..." << std::endl;

    collector = new PacketStatsCollector();
    dev->startCapture(
        [](pcpp::RawPacket *packet,
           __attribute__((unused)) pcpp::PcapLiveDevice *dev,
           __attribute__((unused)) void *cookie) {
            pcpp::Packet parsedPacket(packet);
            collector->consumePacket(parsedPacket);
            usleep(DELAY);
        },
        nullptr);
    std::this_thread::sleep_until(
        std::chrono::time_point<std::chrono::system_clock>::max());
    dev->stopCapture();
    collector->printResult();
    return 0;
}