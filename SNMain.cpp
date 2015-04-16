#include "GameMessage.h"
#include "DataProvider.h"
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include "SNCore.h"
#include "Toolkit.h"

#define MSEC_PROCESS_DELAY 100 // ms to wait between processing events waiting in the message queue
#define SERVER_ADDR_DEFAULT "tcp://127.0.0.1:7633"
#define S_RATE 44100

using namespace stk;

void ProcessItem(const GameMessage*);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "No server address specified - using default (localhost:7633)" << std::endl;
	}
	Stk::setSampleRate( (float) S_RATE );
	Stk::setRawwavePath( "samples/rawwaves/" );

	std::mutex mutex;
	SNCore sncore = SNCore(mutex);

	// Protects access to the GameMessage queue that will be added to asynchronously
	std::mutex mtx;
	// Holds important GameMessages from the game server
	std::queue<GameMessage*> q;

	// Start the thread that will provide data to the GameMessage queue
	DataProvider ddp(mtx, q, argc < 2 ? SERVER_ADDR_DEFAULT : argv[1]);
	ddp.BeginListening();

	// Main processing loop
	// Wait for queue to be populated and perform some work on it when items are available
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(MSEC_PROCESS_DELAY));
		mtx.lock();
		//std::cout << "Main thread got lock, processing all available items" << std::endl;
		while (!q.empty())
		{
			// Process this item 
			ProcessItem(q.front());
			sncore.decodeMessage(q.front());
			delete q.front();
			q.pop();
		}
		mtx.unlock();
	}
}

void ProcessItem(const GameMessage* gm)
{
	// Speed up BPM, queue a bass kick on the next downbeat, decrease synth freq..
	std::cout << "Processing item [" << gm->eventType << "]: " << std::endl;
	if (gm->info == NULL)
	{
		std::cout << "\tgm.info == NULL" << std::endl;
		return;
	}

	for (auto& item : *gm->info)
	{
		// for now, just print out to the console all the events that happen
		std::cout << "\tgm.info[\"" << item.first << "\"]: " << item.second << std::endl;
	}

}