#include <print>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <array>

#pragma comment(lib, "Ws2_32.lib")

std::array<uint8_t, 102> Create_packet(std::array<uint8_t, 6> &mac_address) {
    std::array<uint8_t, 102> packet{};
    for (int i = 0; i < 6; ++i)
        packet[i] = 0xff;
    for (int i = 0; i < 16; ++i)
        std::copy(mac_address.begin(), mac_address.end(), packet.begin() + 6 * (i + 1));
    return packet;
}

bool Send_packet(std::array<uint8_t, 102> &packet,
                 const std::string &ip_address = "255.255.255.255",
                 const int &port = 9) {
    WSAData wsaData{};

    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::println(stderr, "WSAStartup failed: {}", iResult);
        return false;
    }

    // Create a SOCKET
    SOCKET connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (connect_socket == INVALID_SOCKET) {
        std::println(stderr, "Error at socket(): {}" , WSAGetLastError());
        WSACleanup();
        return false;
    }

    BOOL bBroadcast = TRUE;
    setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&bBroadcast, sizeof(bBroadcast));

    struct sockaddr_in addr_dst{};
    addr_dst.sin_family = AF_INET;
    addr_dst.sin_port= htons(port);
    // 'inet_addr' is deprecated: Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings :1833:1:
    // addr_dst.sin_addr.S_un.S_addr = inet_addr(ip_address);
    InetPton(AF_INET,ip_address.data(),&addr_dst.sin_addr);

    int bytesSent = sendto(connect_socket, reinterpret_cast<const char *>(packet.data()), 102, 0, (sockaddr*)&addr_dst, sizeof(addr_dst));
    if (bytesSent == SOCKET_ERROR) {
        std::println(stderr,"sendto failed with error: {}", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return false;
    }


    closesocket(connect_socket);
    WSACleanup();
    return true;
}

int main() {
    std::array<uint8_t, 6> mac_address = {0x98, 0xfa, 0x9b, 0xf6, 0x6f, 0x5e};

    std::array<uint8_t, 102> packet = Create_packet(mac_address);


    std::println(stdout, "Mac address: {:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}",
                 mac_address[0], mac_address[1], mac_address[2],
                 mac_address[3], mac_address[4], mac_address[5]);

    if (Send_packet(packet))
        std::println(stdout, "Magic packet sent successfully.");
    else
        std::print(stderr, "Failed to send magic packet.");

    return 0;
}
