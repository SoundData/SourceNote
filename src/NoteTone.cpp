#include "NoteTone.h"

NoteTone::NoteTone(unsigned short int startBeat, unsigned short int endBeat, WaveForm waveform, float frequency, std::string name) : Tone(startBeat), waveform(waveform), frequency(frequency), noteName(name) {
	endBeatPosition = (endBeat <= 32 || startBeat >= endBeat) ? endBeat : 0;
	// Invalid end beat is set to 0 and the tone will not be played
}

unsigned short int NoteTone::getEndBeatPosition(){
	return endBeatPosition;
}

WaveForm NoteTone::getWaveform(){
	return waveform;
}

float NoteTone::getFrequency(){
	return frequency;
}

std::string NoteTone::getNoteName(){
	return noteName;
}