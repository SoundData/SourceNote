#ifndef _SNCORE_H_INCLUDED
#define _SNCORE_H_INCLUDED

// SNCore.h
//
// SNCore is the only class that an event-emitting program should be using.
// It recieves all events and decides how particular events should affect the music.
// It's the underlying "brain" of SourceNote's dynamic music generation.

#include <string>
#include <mutex>
#include <unistd.h>

#include "GameMessage.h"
#include "ToneCreator.h"
#include "Tempo.h"

class SNCore{
	private:
		ToneCreator toneCreator;
		Tempo tempo;
		int getBPMForClass(const GameMessage *classMessage);
		void setInitialMusicForClass(const GameMessage *classMessage);

	public:
		SNCore(std::mutex &mtx);
		void start(const GameMessage *classMessage);
		void decodeMessage(const GameMessage* message);
};

#endif