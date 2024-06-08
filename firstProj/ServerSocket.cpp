#include "IServer.h"
#include <vector>
#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib,"WS2_32")
#pragma once


namespace firstProj
{
	class ServerSocket : public IServer
	{
	public:
		ServerSocket(int port, u_long mode)
		{
			_port = port;
			_mode = mode;
		}
		void Run() override
		{
			InitWSA();
			InitSocket();
			BindSocket();
		}
		void Close() override
		{
			closesocket(_socket);
			WSACleanup();
		}
		void AcceptNewListiners() override
		{
			SOCKET clientSocket = accept(_socket, nullptr, nullptr);
			if (clientSocket != INVALID_SOCKET)
				_listeners.push_back(clientSocket);
		}
		void SendForAll(std::string& message) override
		{
			if (message == "")
				return;
			const char* msg = message.c_str();
			for (std::vector<SOCKET>::iterator it = _listeners.begin(); it != _listeners.end();)
			{
				SOCKET& listiner = *it;
				int bytesSend = send(listiner, msg, strlen(msg), 0);
				if (bytesSend == SOCKET_ERROR)
					it = _listeners.erase(it);
				else it++;
			}
		}

	private:
		WSADATA _wsaData;
		SOCKET _socket;
		std::vector<SOCKET> _listeners;
		u_long _mode;
		int _port;
		sockaddr_in _serverAddr;

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
				throw std::string("Failed to create listening socket");
			}

			if (ioctlsocket(_socket, FIONBIO, &_mode) != 0)
			{
				closesocket(_socket);
				WSACleanup();
				throw std::string("ioctlsocket failed with error: " + WSAGetLastError());
			}

			_serverAddr.sin_family = AF_INET;
			_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
			_serverAddr.sin_port = _port;
		}

		void BindSocket()
		{
			if (bind(_socket, reinterpret_cast<sockaddr*>(&_serverAddr), sizeof(_serverAddr)) == SOCKET_ERROR) {
				closesocket(_socket);
				WSACleanup();
				throw std::string("Bind failed with error: " + WSAGetLastError());
			}

			if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
				closesocket(_socket);
				WSACleanup();
				throw std::string("Listen failed with error: " + WSAGetLastError());
			}
		}
	};
}