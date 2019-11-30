#pragma once
#include "Include.hpp"

void DecomposeInt_32(unsigned char* buf, int32_t val);
int32_t ComposeInt_32(unsigned char* buf);
void SetSocketStatus(Network::TCPSocket& connected, WSAPOLLFD& connectedFD);
void ReventsError(WSAPOLLFD& fd, std::string& status);
int ReceiveData(Network::TCPConnection& connection, WSAPOLLFD& use_fd);
void ReceivedBytesError(int receivedBytes, std::string& status);
void ProcessPacketSize(Network::TCPConnection& connection, uint8_t& conditionFlag);
void ProcessPacketContent(Network::TCPConnection& connection);
void SendSizeData(Network::PacketManager& pm, WSAPOLLFD& use_fd, uint8_t& flag);
void SendContentData(Network::PacketManager& pm, WSAPOLLFD& use_fd, uint8_t& flag);




