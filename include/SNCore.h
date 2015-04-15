#ifndef _SNCORE_H_INCLUDED
#define _SNCORE_H_INCLUDED

#include "GameMessage.h"
#include "ToneCreator.h"
#include "Tempo.h"
#include <string>
#include <mutex>
#include <unistd.h>

class SNCore{
	private:
		ToneCreator toneCreator;
		Tempo tempo;
		std::mutex &mutex;
		int getBPMForClass(const GameMessage *classMessage);
		void setInitialMusicForClass(const GameMessage *classMessage);

	public:
		SNCore(std::mutex &mtx);
		void start(const GameMessage *classMessage);
		void decodeMessage(const GameMessage* message);
};

#endif