include_directories(../include)

add_executable(monitor main.cpp monitor.cpp ../common/utils.cpp)
find_package(PcapPlusPlus REQUIRED)
target_link_libraries(monitor PcapPlusPlus::PcapPlusPlus)
find_package(CLI11 REQUIRED)
target_link_libraries(monitor CLI11::CLI11)
