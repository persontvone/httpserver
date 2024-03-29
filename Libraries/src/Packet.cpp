#include <Packet.hpp>
#include <Constants.hpp>
#include <iostream>
#include <NetworkUtility.hpp>

struct PacketHash
{
	std::size_t operator()(Network::Packet p) const
	{
		std::hash<int> hashVal1;
		return hashVal1(p.m_extractionOffset);
	}
};

bool Network::operator==(const Packet& lhs, const Packet& rhs)
{
	return lhs.m_buffer == rhs.m_buffer && lhs.m_extractionOffset == rhs.m_extractionOffset;
}

Network::Packet::Packet(PacketType packetType)
{
	Clear();
	AssignPacketType(packetType);
}

Network::PacketType Network::Packet::GetPacketType() 
{
	PacketType* packet_ptr = reinterpret_cast<PacketType*>(&m_buffer[0]);
	return static_cast<PacketType>(ntohs((uint16_t)*packet_ptr));
}

void Network::Packet::AssignPacketType(PacketType packetType)
{
	PacketType* packet_ptr = reinterpret_cast<PacketType*>(&m_buffer[0]);
	*packet_ptr = static_cast<PacketType>(htons((uint16_t)packetType));
}

void Network::Packet::Clear()
{
	m_buffer.resize(sizeof(PacketType));
	AssignPacketType(PacketType::PT_Invalid);
	m_extractionOffset = sizeof(PacketType);
}

void Network::Packet::Append(const void* data, uint32_t size)
{
	if ((m_buffer.size() + size) > Network::g_maxPacketSize)
	{
		std::cerr << "Packet size exceeds max buffer size.\n";
	}
	m_buffer.insert(m_buffer.end(), (char*)data, (char*)data + size);
}

void Network::Packet::AppendInteger(const uint32_t* data, uint32_t size)
{
	if ((m_buffer.size() + size) > Network::g_maxPacketSize)
	{
		std::cerr << "Packet size exceeds max buffer size.\n";
	}
	for (uint32_t i = 0; i < size; i++)
	{
		char tmp[4] = { 0 };
		DecomposeInt_32(tmp, data[i]);
		m_buffer.insert(m_buffer.end(), &tmp[0], &tmp[4]);
	}
}

//data is 4 bytes. It's size variable.
//how many bytes will be send, and how many bytes need to recieve
//Write 4 bytes to packet class
Network::Packet& Network::Packet::operator<<(uint32_t data)
{
	data = htonl(data);
	Append(&data, sizeof(uint32_t));
	return *this;
}

//read size to variable(data)
//how many bytes need to read
//data is 4 bytes, so we we'll read 4 bytes
//start to read from offset and after that
//we increase offset for further reading
Network::Packet& Network::Packet::operator>>(uint32_t& data)
{
	if ((m_extractionOffset + sizeof(uint32_t)) > m_buffer.size())
	{
		std::cerr << "\nOffset exceeds max buffer size.\n";
	}
	data = *reinterpret_cast<uint32_t*>(&m_buffer[m_extractionOffset]);
	data = ntohl(data);
	m_extractionOffset += sizeof(uint32_t);
	return *this;
}

Network::Packet& Network::Packet::operator<<(const std::string& data)
{
	//using Packet& Packet::operator<<(uint32_t data)
	*this << (uint32_t)data.size();
	Append(data.data(), data.size());
	return *this;
}
Network::Packet& Network::Packet::operator>>(std::string& data)
{
	//clear old data
	data.clear();
	uint32_t stringSize = 0;
	//retrieve string size from Packet& Packet::operator>>(uint32_t& data)
	*this >> stringSize;
	if ((m_extractionOffset + stringSize) > m_buffer.size())
	{
		std::cerr << "\nOffset exceeds max buffer size.\n";
	}
	data.resize(stringSize);
	for (uint32_t i = 0; i < stringSize; i++)
	{
		data[i] = m_buffer[i + m_extractionOffset];
	}
	m_extractionOffset += stringSize;
	return *this;
}

//write to Packet 
Network::Packet& Network::Packet::operator<<(const std::vector< char > & data)
{
	//using Packet& Packet::operator<<(uint32_t data)
	*this << (uint32_t)data.size();
	Append(data.data(), (uint32_t)data.size());
	return *this;
}

Network::Packet& Network::Packet::operator>>(std::vector< char > & data)
{
	//clear old data
	data.clear();
	uint32_t size = 0;
	//retrieve string size from Packet& Packet::operator>>(uint32_t& data)
	*this >> size;
	if ((m_extractionOffset + size) > m_buffer.size())
		std::cerr << "[Packet::operator >>(std::string &)] - Extraction offset exceeded buffer size.\n";
	data.resize(size);
	
	for (uint32_t i = 0; i < size; i++)
	{
		data[i] = m_buffer[i + m_extractionOffset];
	}
	m_extractionOffset += size;
	return *this;
}

Network::Packet& Network::Packet::operator << (const std::vector< uint32_t >& data)
{
	*this << (uint32_t)data.size();
	AppendInteger(data.data(), data.size());
	return *this;
}
Network::Packet& Network::Packet::operator >> (std::vector < uint32_t >& data)
{
	data.clear();
	uint32_t size = 0;
	*this >> size;
	if ((m_extractionOffset + size) > m_buffer.size())
		std::cerr << "[Packet::operator >>(std::string &)] - Extraction offset exceeded buffer size.\n";
	data.resize(size);

	for (uint32_t i = 0; i < size; i++)
	{
		char tmp[4] = 
		{
			m_buffer[m_extractionOffset],
			m_buffer[m_extractionOffset + 1],
			m_buffer[m_extractionOffset + 2],
			m_buffer[m_extractionOffset + 3]
		};
		data[i] = ComposeInt_32(tmp);
		m_extractionOffset += sizeof(uint32_t);
	}
	return *this;
}

