#include "CLI/CLI.hpp"
#include "monitor.hpp"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/SystemUtils.h"
#include <chrono>
#include <csignal>
#include <format>
#include <iostream>
#include <thread>

pcpp::PcapLiveDevice *dev = nullptr;
PacketStatsCollector *collector = nullptr;

void finalize() {
    std::cout << "Stopping capture..." << std::endl;
    if (dev != nullptr) {
        dev->stopCapture();
        dev->close();
    }
    if (collector != nullptr) {
        delete collector;
    }
}

void signalHandler(int) {
    finalize();
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
        [](pcpp::RawPacket *packet, pcpp::PcapLiveDevice *, void *) {
            pcpp::Packet parsedPacket(packet);
            collector->consumePacket(parsedPacket);
            usleep(DELAY);
        },
        nullptr);
    std::this_thread::sleep_until(
        std::chrono::time_point<std::chrono::system_clock>::max());
    finalize();
    return 0;
}