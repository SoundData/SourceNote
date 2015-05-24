#ifndef __TONE_H_INCLUDED__
#define __TONE_H_INCLUDED__

// Tone.h
//
// Tone is a base class for usable Tone subclasses, and should probably never be instantiated by itself without subclassing.
// Tones encapsulate data for a sound. Subclasses may include data such as the start, end beat, sound type, sound file, etc.

class Tone{
	private:
		unsigned int startTime;
		// startBeatPosition be an int from 1 to 32. It's the beat position where the tone will be played in a 2 measure interval.
		// Measures are always in 4/4 time with each measure containing 16 beats, making 2 measures contain 32 beats.
		unsigned short int startBeatPosition; 

	public:
		Tone(unsigned short int startBeat);
		Tone(){};
		unsigned short int getStartBeatPosition();
};

#endif 