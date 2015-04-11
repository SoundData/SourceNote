#ifndef __NOTETONE_H_INCLUDED__
#define __NOTETONE_H_INCLUDED__

#include "Tone.h"
#include <vector>
#include <string>

enum WaveForm{
	kSine = 0,
	kSquare,
	kTraingle,
	kSawtooth,
};

class NoteTone: public Tone{
	private:
		unsigned short int endBeatPosition;
		/* Must be an int from 1 to 32. EndBeatPosition is the beat position where the tone will be played in a 2 measure interval.
		 * Measures are always in 4/4 time with each measure containing 16 beats, making 2 measures contain 32 beats. */
		WaveForm waveform;
		float frequency;
		std::string noteName;
		/* In the format <note><b/#/""><octave>
		 * Ex: "C1" "Bb2" "C#2" */

	public:
		NoteTone(unsigned short int startBeat, unsigned short int endBeat, WaveForm waveform, float frequency, std::string name);
		NoteTone(){}; //Required to access a NoteTone in a vector of NoteTones. My research suggests its a Clang specific issue.
		unsigned short int getEndBeatPosition();
		WaveForm getWaveform();
		float getFrequency();
		unsigned int getRepeatCount();
		std::string getNoteName();
};

#endif 