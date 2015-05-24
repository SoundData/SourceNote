#ifndef __PERCUSSIONTONE_H_INCLUDED__
#define __PERCUSSIONTONE_H_INCLUDED__

// PercussionTrack.h
//
// A Tone subclass that encapsulates a sound from a file.

#include <string>

#include "Tone.h"

class PercussionTone: public Tone{
	private:
		std::string fileName;

	public:
		PercussionTone(unsigned int startTime, std::string fileName);
		PercussionTone(){};
		std::string getFileName();
};

#endif 