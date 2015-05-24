#include "SNCore.h"

#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

#include "GameMessage.h"
#include "DataProvider.h"
#include "Toolkit.h"

#define MSEC_PROCESS_DELAY 100 // ms to wait between processing events waiting in the message queue
#define SERVER_ADDR_DEFAULT "tcp://127.0.0.1:7633"
#define S_RATE 44100

using namespace stk;

void ProcessItem(const GameMessage*);
bool DropIrrelavantMessage(const GameMessage*, const std::string&);

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "No server address specified - try using tcp://104.236.52.206:7633" << std::endl;
		std::cout << "You also need to specify a username" << std::endl;
		std::cout << "Exiting" << std::endl;
		return 0;
	}
	
	Stk::setSampleRate( (float) S_RATE );
	Stk::setRawwavePath( "samples/rawwaves/" );

	std::mutex mutex;
	SNCore sncore = SNCore(mutex);
	
	std::string username(argv[2]);
	std::cout << "Hello, " << username << "!" << std::endl;

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
			GameMessage *gm = q.front();
			if (DropIrrelavantMessage(gm, username))
			{
				delete gm;
				q.pop();
				continue;
			}
			// Process this item 
			ProcessItem(gm);
			sncore.decodeMessage(gm);
			delete gm;
			q.pop();
		}
		mtx.unlock();
	}
}

bool DropIrrelavantMessage(const GameMessage* gm, const std::string& username)
{
	// evaluate cases for which we may need to drop this message
	
	if (gm->eventType.compare("PLAYER_DEATH") == 0)
		return !(gm->info->at("Attacker").compare(username) == 0 ||
				 gm->info->at("Victim").compare(username) == 0 ||
				 gm->info->at("Assister").compare(username) == 0);
	
	if (gm->eventType.compare("PLAYER_SPAWN") == 0)
		return !(gm->info->at("PlayerName").compare(username) == 0);
		
	if (gm->eventType.compare("PLAYER_CHANGECLASS") == 0)
		return !(gm->info->at("PlayerName").compare(username) == 0);
		
	if (gm->eventType.compare("PLAYER_CHANGE_TEAM") == 0)
		return !(gm->info->at("PlayerName").compare(username) == 0);
		
	if (gm->eventType.compare("PLAYER_HURT") == 0)
		return !(gm->info->at("VictimName").compare(username) == 0 ||
				 gm->info->at("AttackerName").compare(username) == 0);
				 
	if (gm->eventType.compare("DEMOMAN_STICKY_JUMP") == 0)
		return !(gm->info->at("PlayerName").compare(username) == 0);
		
	if (gm->eventType.compare("SOLDIER_ROCKET_JUMP") == 0)
		return !(gm->info->at("PlayerName").compare(username) == 0);
		
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