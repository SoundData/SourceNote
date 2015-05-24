#ifndef __NOTETONE_H_INCLUDED__
#define __NOTETONE_H_INCLUDED__

// NoteTone.h
//
// A Tone subclass that encapsulates a sine wave sound.

#include <vector>
#include <string>

#include "Tone.h"

enum WaveForm{
	kSine = 0,
	kSquare,
	kTraingle,
	kSawtooth,
};

class NoteTone: public Tone{
	private:
		// Must be an int from 1 to 32. EndBeatPosition is the beat position where the tone will be played in a 2 measure interval.
		// Measures are always in 4/4 time with each measure containing 16 beats, making 2 measures contain 32 beats.
		unsigned short int endBeatPosition;
		WaveForm waveform;
		float frequency;
		// noteName is in the format <note><b/#/""><octave>
		// Ex: "C1" "Bb2" "C#2"
		std::string noteName;

	public:
		NoteTone(unsigned short int startBeat, unsigned short int endBeat, WaveForm waveform, float frequency, std::string name);
		NoteTone(){};
		unsigned short int getEndBeatPosition();
		WaveForm getWaveform();
		float getFrequency();
		unsigned int getRepeatCount();
		std::string getNoteName();
};

#endif 