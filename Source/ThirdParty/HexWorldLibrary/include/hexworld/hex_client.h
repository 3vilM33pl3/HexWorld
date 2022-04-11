#pragma once

#include <string>
#include <vector>
#include <hexworld/hex_com_state.h>
#include <hexworld/hex_lib.h>

class HexagonClientImpl;
class HexagonClient {
public:
    HexagonClient();
    HexagonClient(const std::string server_address, bool ConnectEncrypted);

    hw_conn_state ConnectToServer();

    hw_conn_state GetConnectionState();

    std::vector<Hexagon> GetHexagonRing(Hexagon *hex, const int64_t radius, bool fill);
private:
    HexagonClientImpl* impl;
};