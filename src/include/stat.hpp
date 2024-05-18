#include "nlohmann/json.hpp"
#include "utils.hpp"

nlohmann::json getAllStats(PacketStats &stats);
nlohmann::json getIPv4Stats(IPv4Stats &stats);
nlohmann::json getIPv6Stats(IPv6Stats &stats);
nlohmann::json getTCPStats(TCPStats &stats);
nlohmann::json getUDPStats(UDPStats &stats);
nlohmann::json getHTTPStats(HTTPStats &stats);