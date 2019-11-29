#include <gtest/gtest.h>
#include <Server.h>
#include <Initializer.h>
#include <NetworkUtility.h>

using namespace Network;

TEST(NetworkUtilityTest, Int_32Conversion)
{
	unsigned char buf[4] = { 0 };
	uint32_t val = 31242;
	DecomposeInt_32(buf, val);
	uint32_t ret_val = ComposeInt_32(buf);
	ASSERT_EQ(val, ret_val);

	val = 1;
	DecomposeInt_32(buf, val);
	ret_val = ComposeInt_32(buf);
	ASSERT_EQ(val, ret_val);

	val = -100;
	DecomposeInt_32(buf, val);
	ret_val = ComposeInt_32(buf);
	ASSERT_EQ(val, ret_val);
}

TEST(NetworkUtilityTest, SetSocketStatus)
{
	Initialize();
	WSAPOLLFD fd;
	TCPSocket sock;
	ASSERT_TRUE(sock.Create());
	SetSocketStatus(sock, fd);
	ASSERT_EQ(fd.events, POLLRDNORM);
	ASSERT_EQ(fd.revents, 0);
	ASSERT_EQ(fd.fd, sock.GetHandle());
	Shutdown();
}

TEST(NetworkUtilityTest, ReventsError)
{
	Initialize();
	//Server server;
	//ASSERT_TRUE(server.Start(IPAddress("127.0.0.1", 1444)));
	WSAPOLLFD fd;
	TCPSocket sock;
	sock.Create();
	SetSocketStatus(sock, fd);
	std::string status;
	ReventsError(fd, status);
	ASSERT_EQ(status, "");
	fd.revents = POLLERR;
	ReventsError(fd, status);
	ASSERT_EQ(status, "POLLERR");
	fd.revents = POLLHUP;
	ReventsError(fd, status);
	ASSERT_EQ(status, "POLLHUP");
	fd.revents = POLLNVAL;
	ReventsError(fd, status);
	ASSERT_EQ(status, "POLLNVAL");
	Shutdown();
}

//TEST(NetworkUtilityTest, ReceiveData)
//{
//	Initialize();
//	WSAPOLLFD fd;
//	TCPSocket sock;
//	TCPSocket accepted;
//	TCPSocket client;
//	IPAddress ip("127.0.0.1", 7897);
//	IPAddress acceptedIp;
//	sock.Create();
//	client.Create();
//	sock.Listen(ip);
//	client.Connect(ip);
//	Sleep(50);
//	sock.Accept(accepted, &acceptedIp);
//	SetSocketStatus(sock, fd);
//	TCPConnection tcp(accepted, acceptedIp);
//	ASSERT_EQ(ReceiveData(tcp, fd), 0);
//
//	Shutdown();
//}


