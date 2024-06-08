#pragma once
#include <mutex>
#include <condition_variable>


namespace firstProj {
	struct Synchronize {
		std::mutex mutex;
		std::condition_variable condVar;
		bool dataReady = false;
	};
}