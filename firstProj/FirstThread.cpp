#include "Synchro.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>


namespace firstProj
{
	class FirstThread
	{
	private:
		std::string& _buffer;
		Synchronize& _sync;
		void UpdateBufferWithLock(std::string& update)
		{
			std::unique_lock<std::mutex> lock(_sync.mutex);
			_sync.condVar.wait(lock, [&] { return _sync.dataReady == false; });
			_buffer = update;
			_sync.dataReady = true;
			lock.unlock();
			_sync.condVar.notify_one();
		}
		std::string BlockingConsoleInput()
		{
			std::string input;
			std::cout << "Enter: ";
			std::getline(std::cin, input);
			return input;
		}
		bool ValidateInput(std::string& str)
		{
			if (str.empty() || str.length() > 64)
				return false;
			for (char digit : str)
				if (!isdigit(digit))
					return false;
			return true;
		}
		std::string TransformString(std::string str)
		{
			std::sort(str.rbegin(), str.rend());

			std::ostringstream oss;
			for (char& ch : str)
				if ((ch - '0') % 2 == 0)
					oss << "KB";
				else
					oss << ch;
			return oss.str();
		}
	public:
		FirstThread(std::string& buffer, Synchronize& sync) : _buffer(buffer), _sync(sync) {}

		void Run()
		{
			try
			{
				while (true) {
					std::string input = BlockingConsoleInput();
					if (!ValidateInput(input))
						continue;
					std::string transformed = TransformString(input);
					UpdateBufferWithLock(transformed);
				}
			}
			catch (std::string& err)
			{
				std::cout << err << std::endl;
			}
		}
	};
}