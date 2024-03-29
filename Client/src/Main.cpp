#include <TCPClient.hpp>
#include <thread>

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
		return TRUE;
	case CTRL_C_EVENT:
		return TRUE;
	default:
		return FALSE;
	}
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (Network::Initialize())
	{
		SetConsoleCtrlHandler(CtrlHandler, TRUE);
		Network::TCPClient client;
		if (client.Connect(Network::IPAddress("192.168.0.104", 8080)))
		{
			std::thread th([&client]()
				{
					while (true)
					{
						client.ChatFrame();
					}
				});
			th.detach();
			while (client.IsConnected())
			{
				client.Frame();
				
			}
		}
	}
	Network::Shutdown();
	system("pause");
	return 0;
}