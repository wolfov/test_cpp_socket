#include "clientSocket.cpp"
#include <string>
#include <thread>

void FinalMethod(std::string str)
{
	if (str.length() == 2 || stoi(str) % 32 == 0)
		std::cout << "Data valid, result: " << str << std::endl;
	else std::cout << "Data, isn't valid" << std::endl;
}

void SocketConnect(ClientSocket& socket)
{
	bool connected = false;
	while (!connected) {
		try
		{
			socket.Run();
			connected = true;
		}
		catch(...)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

int main() {
	try
	{
		ClientSocket cs("127.0.0.1", 8080);
		cs.AddCallBack(FinalMethod);
		SocketConnect(cs);
		while (true)
		{
			try
			{
				cs.ListenNewMessages();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			catch (const std::string& err)
			{
				SocketConnect(cs);
			}
		}
	}
	catch (const std::string& err)
	{
		std::cout << err << std::endl;
	}
	return 0;
}

