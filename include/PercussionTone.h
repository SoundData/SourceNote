#ifndef __PERCUSSIONTONE_H_INCLUDED__
#define __PERCUSSIONTONE_H_INCLUDED__

#include "Tone.h"
#include <string>

class PercussionTone: public Tone{
	private:
		std::string fileName;

	public:
		PercussionTone(unsigned int startTime, std::string fileName);
		PercussionTone(){};
		std::string getFileName();
};

#endif 