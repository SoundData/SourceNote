#ifndef _SNCORE_H_INCLUDED
#define _SNCORE_H_INCLUDED

#include "GameMessage.h"
#include "ToneCreator.h"
#include "Tempo.h"
#include <string>
#include <mutex>

class SNCore{
	private:
		ToneCreator toneCreator;
		Tempo *tempo;
		std::mutex mutex;
		int getBPMForClass(GameMessage *classMessage);

	public:
		SNCore();
		void start(GameMessage *classMessage);
		void decodeMessage(GameMessage* message);
};

#endif