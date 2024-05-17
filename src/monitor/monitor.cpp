#include "monitor.hpp"
#include "utils.hpp"

#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/SystemUtils.h"
#include "pcapplusplus/TcpLayer.h"
#include "pcapplusplus/UdpLayer.h"
#include <fcntl.h>
#include <format>
#include <iostream>

PacketStatsCollector::PacketStatsCollector() {
    pipe = new DataPipe(getDataPipePath(), O_WRONLY);
}

PacketStatsCollector::~PacketStatsCollector() { delete pipe; }

void PacketStatsCollector::consumePacket(pcpp::Packet &packet) {
    // Called when a packet is captured
    // Send the packet to the named pipe for advanced statistics
    pipe->send(packet.getRawPacketReadOnly()->getRawData(),
               packet.getRawPacketReadOnly()->getRawDataLen());
}
