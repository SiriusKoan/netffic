include_directories(../include)

add_executable(stat main.cpp stat.cpp ../common/utils.cpp)
find_package(PcapPlusPlus REQUIRED)
target_link_libraries(stat PcapPlusPlus::PcapPlusPlus)
find_package(httplib REQUIRED)
target_link_libraries(stat httplib::httplib)
find_package(nlohmann_json REQUIRED)
target_link_libraries(stat nlohmann_json::nlohmann_json)