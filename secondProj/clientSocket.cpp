#include <vector>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <functional>
#pragma comment(lib,"WS2_32")

class ClientSocket
{
public:
	ClientSocket(const char* ip, int port)
	{
		_serverAddr.sin_family = AF_INET;
		_serverAddr.sin_addr.s_addr = inet_addr(ip);
		_serverAddr.sin_port = port;
	}
	void Run()
	{
		InitWSA();
		InitSocket();
		Connect();
	}
	void CloseSocket()
	{
		closesocket(_socket);
		WSACleanup();
	}
	void ListenNewMessages()
	{
		char buffer[512];
		int bytesRead = recv(_socket, buffer, sizeof(buffer), 0);
		if (bytesRead == SOCKET_ERROR) 
			throw std::string("Disconnected");
		else
		{
			std::string message(buffer, bytesRead);
			for (auto& f : _callbacks)
				f(message);
		}
	}
	void AddCallBack(std::function<void(std::string)> func)
	{
		_callbacks.push_back(func);
	}

private:
	WSADATA _wsaData;
	SOCKET _socket;
	sockaddr_in _serverAddr;
	std::vector<std::function<void(std::string)>> _callbacks;

	void InitWSA()
	{
		if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
			throw std::string("Failed to initialize WSA");
	}

	void InitSocket()
	{
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET)
		{
			WSACleanup();
			throw std::string("Failed to create socket");
		}
	}

	void Connect()
	{
		if (connect(_socket, reinterpret_cast<sockaddr*>(&_serverAddr), sizeof(_serverAddr)) < 0) {
			closesocket(_socket);
			WSACleanup();
			throw std::string("Failed to connect");
		}
	}
};