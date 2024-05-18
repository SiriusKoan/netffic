# netffic

A simple network traffic statistics server for real-time network traffic monitoring.

## Setup

```shell
$ mkdir build
$ conan install . --build=missing --output-folder=build
```

## Build

```shell
$ cmake --preset conan-release
$ cd build
$ make
```

## Run

```shell
$ cd build
$ sudo ./bin/monitor -i [interface]
# Or use tcpdump
$ sudo tcpdump [your filters] -w - > [fifo path]
```

```shell
# Another window
$ cd build
$ ./bin/stat [host] [port]
```

And you can see the statistics of the network traffic by querying the server.

Assume the server is running on `localhost:8080`.

```shell
# all stats
$ curl http://localhost:8080/stats/all | jq
# IPv4 stats
$ curl http://localhost:8080/stats/ipv4 | jq
# IPv6 stats
$ curl http://localhost:8080/stats/ipv6 | jq
# TCP stats
$ curl http://localhost:8080/stats/tcp | jq
# UDP stats
$ curl http://localhost:8080/stats/udp | jq
# HTTP stats
$ curl http://localhost:8080/stats/http | jq
```

## Styling

```shell
$ find ./src -regex '.*\.\(cpp\|h\|hpp\|cc\|cxx\)' -exec clang-format -i -style=file {} \;
```