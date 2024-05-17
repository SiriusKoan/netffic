#pragma once

#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/Packet.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/SystemUtils.h"
#include "utils.hpp"

class PacketStatsCollector {
  public:
    PacketStatsCollector();
    ~PacketStatsCollector();
    void consumePacket(pcpp::Packet &packet);

  private:
    DataPipe *pipe;
};
