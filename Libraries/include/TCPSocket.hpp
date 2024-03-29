#pragma once
#include <WinSock2.h>
#include <IPAddress.hpp>
#include <Packet.hpp>

namespace Network
{
	struct TCPSocket
	{
		//API
		TCPSocket(IPVersion version = IPVersion::IPv4, int type = SOCK_STREAM, int protocol = IPPROTO_TCP, SOCKET handle = INVALID_SOCKET);
		~TCPSocket();

		bool Create();
		bool Close();
		SOCKET GetHandle() const;
		IPVersion GetIPVersion() const;
		bool Bind(IPAddress& endpoint);
		bool Listen(IPAddress& endpoint, int backlog = 5);
		bool Accept(TCPSocket& outSocket, IPAddress* ip = nullptr);
		bool Connect(IPAddress& endpoint);
		int GetType();
		int GetProtocol();
		bool SetBlocking(bool isBlocking);
		bool IsBlocking();
		friend bool operator==(const TCPSocket& lhs, const TCPSocket& rhs);
	private:
		IPVersion m_ipVersion;
		int m_type;
		int m_protocol;
		SOCKET m_handle;
		bool m_isBlocking = false;
	};
}