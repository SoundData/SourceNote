#ifndef TOOLKIT_H
#define TOOLKIT_H

#define MAX_VOICES 10
#define N_CHANNELS 1

// std lib
#include <iostream>
#include <string>
#include <vector>

// stk
#include "RtAudio.h"
#include "Voicer.h"
#include "BeeThree.h"
#include "SineWave.h"
#include "FileRead.h"

// our stuff
#include "NoteTone.h"
#include "PercussionTone.h"

// ToneData is a helper struct that can be accessed from within the RtAudio tick function call
struct ToneData {
	// stk::Voicer *voicer;
	unsigned int samplesLeft[MAX_VOICES];
	stk::SineWave *sines[MAX_VOICES];
	// stk::Instrmnt *instruments[MAX_VOICES];
	bool isPlaying;
	
	
	ToneData()
		: isPlaying(false) {}
};

class Toolkit {	
	private:
		stk::StkFloat samplesBetweenBeats;
		RtAudio dac;
		ToneData *data;
		void addVoice(std::string type, float freq, unsigned int duration, int group);
		static void removeVoice(ToneData *tdata, int group);
		
		
	public:
		Toolkit(int microSecondsBetween);
		~Toolkit(void);
		int startStream(void);
		int stopStream(void);
		void playTone(unsigned int start, int duration, std::string waveform, float freq);
		void playNoteTone(NoteTone *t);
		void playNoteTones(std::vector<NoteTone> tones);
		void playPercussionTone(PercussionTone *t);
		static int mainTick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer);
};



#endif