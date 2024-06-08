#include "Synchro.h"
#include "FirstThread.cpp"
#include "SecondThread.cpp"
#include <thread>

int main() {
	std::string buffer;
	firstProj::Synchronize synchro;
	firstProj::ServerSocket SS(8080, 1);
	firstProj::FirstThread firstThread(buffer, synchro);
	firstProj::SecondThread secondThread(buffer, synchro, SS);

	std::thread thread2(&firstProj::SecondThread::Run, std::ref(secondThread));
	firstThread.Run();
	thread2.join();

	return 0;
}



