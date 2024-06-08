#include "Synchro.h"
#include "IServer.h"
#include "ServerSocket.cpp"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <random>
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <functional>
#pragma once


namespace firstProj
{
	class SecondThread
	{
	private:
		std::string& _buffer;
		Synchronize& _sync;
		IServer& _server;
		bool WaitFromBuffer(std::string& fromBuffer, int msTimeOut)
		{
			bool waited = false;
			std::unique_lock<std::mutex> lock(_sync.mutex);
			waited = _sync.condVar.wait_for(lock, std::chrono::milliseconds(msTimeOut), [&] { return _sync.dataReady == true; });
			if (waited)
			{
				fromBuffer = _buffer;
				_buffer = "";
				_sync.dataReady = false;
				lock.unlock();
				_sync.condVar.notify_one();
			}
			return waited;
		}
		int SumDigitsInString(std::string& str)
		{
			int sum = 0;
			for (char chr : str)
				if (isdigit(chr))
					sum += chr - '0';
			return sum;
		}


	public:
		SecondThread(std::string& buffer, Synchronize& sync, IServer& server) : _buffer(buffer), _sync(sync), _server(server) {}
		void Run()
		{
			try
			{
				_server.Run();
				while (true)
				{
					std::string fromBuffer;
					if (WaitFromBuffer(fromBuffer, 100))
					{
						_server.AcceptNewListiners();
						if (fromBuffer.length() > 0) {
							int sum = SumDigitsInString(fromBuffer);
							std::string tosend = std::to_string(sum);
							_server.SendForAll(tosend);
						}
					}
					else
						_server.AcceptNewListiners();
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
			catch (std::string& err)
			{
				std::cout << err << std::endl;
			}
		}
	};
}