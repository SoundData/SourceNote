#include "Toolkit.h"
#include <cmath>
#define TOOLKIT_DEBUG 0

using std::min;
using namespace stk;

Toolkit::Toolkit(int microSecondsBetween) {
	samplesBetweenBeats = (StkFloat) microSecondsBetween * (Stk::sampleRate() / 1000000);
	
	// Initialize the ToneData
	data = new ToneData();
	for(int i=0; i < MAX_VOICES; i++) {
		data->samplesLeft[i] = 0;
	}
	
	// Setup the RtAudio parameters
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = N_CHANNELS;
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	unsigned int bufferFrames = RT_BUFFER_SIZE;
	
	// Initialize the DAC
	try {
		dac.openStream(&parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &Toolkit::mainTick, (void *)data);
	} catch(StkError &e) {
		std::cout << "Error starting the stream, sorry man.\n";
		return;
	}
	
}

Toolkit::~Toolkit() {
	// dac.closeStream();
}

// Turn up!
int Toolkit::startStream(void) {
	std::cout << "Starting up that audio!\n";
	try {
		dac.startStream();
	} catch(StkError &e) {
		std::cout << "Error starting the stream, sorry man.\n";
		return 0;
	}
	
	return 1;
}

int Toolkit::mainTick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer) {
	
	ToneData *tData = (ToneData *) dataPointer;
	StkFloat *samples = (StkFloat *) outputBuffer;
	unsigned int nTicks = (int) nBufferFrames;
	
	// What we're doing now:
	// - Naively adding the sum of each tick to the current sample, in a double for loop
	
	// What we might want to consider trying
	// - Using StkFrames to handle the samples and ticks, then use a for loop to set each sample, i, equal to StkFrames[i]
	
	for(int i = 0; i < nTicks; i++) {
		//if(!data->isPlaying) break;
	
		//samples[i] = tData->voicer->tick();
		//samples[i] +=
		samples[i] = 0;
		
		for(int j = 0; j < MAX_VOICES; j++) {
			if(tData->samplesLeft[j] != 0)
				samples[i] += tData->sines[j]->tick();
		}
	}
	
	for(int i = 0; i < MAX_VOICES; i++) {
		
		if(tData->samplesLeft[i] > 0) {
			// Subtract number of samples we output, or set to zero if we'd  go negative.
			tData->samplesLeft[i] = (nTicks >= tData->samplesLeft[i]) ? 0 : tData->samplesLeft[i] - nTicks;
			
			// Stop the instrument if we don't have any more samples to play
			if(tData->samplesLeft[i] == 0) {
				Toolkit::removeVoice(tData, i);
			}
			
			if(TOOLKIT_DEBUG) std::cout << "instrument " << i << ": " << tData->samplesLeft[i] << " samples left.\n";
		}
		
		
	}
	
	
	
	return 0;
}

int Toolkit::stopStream(void) {
	std::cout << "stopping the stream\n";
	try {
		dac.closeStream();
	} catch ( RtAudioError &error ) {
		error.printMessage();
		return 1;
	}
	return 0;
}

void Toolkit::playNoteTone(NoteTone *t) {
	std::cout << "Playing waveform " << t->getWaveform() << " at frequency " << t->getFrequency() << "\n";
	
	// Find the next available space for our instrument
	for(int i = 0; i < MAX_VOICES; i++) {
		if(data->samplesLeft[i] == 0) {
			
			int duration = (int) samplesBetweenBeats * std::abs(t->getEndBeatPosition() - t->getStartBeatPosition());
			
			std::cout << "The duration of this note is " << duration << " samples.\n";
			
			Toolkit::addVoice("Sine", t->getFrequency(), duration, i);
			
			break;
		} else if(i == MAX_VOICES-1) {
			// All of the available spots are taken
			std::cout << "There was no room for this instrument.\n";
			
			for(int j = 0; j < MAX_VOICES; j++) {
				std::cout << "Voice #" << j+1 << ": " << data->samplesLeft[i] << "\n";
			}
			
		}
	}
}

void Toolkit::addVoice(std::string type, float freq, unsigned int duration, int group) {
	
	std::cout << "Adding new Sine in group "<< group <<".\n";
	data->samplesLeft[group] = duration;
	
	data->sines[group] = new SineWave();
	data->sines[group]->setFrequency( freq );
	
	if(!data->isPlaying) data->isPlaying = true;
	
}

void Toolkit::removeVoice(ToneData *tdata, int group) {
	
	tdata->sines[group] = NULL;
	
}

/*void Toolkit::playNoteTomes(std::vector<NoteTone> tones) {
	int i;
	for(i=0; i < tones.size(); i++) {
		// playNoteTone()
	}
}*/

void Toolkit::playPercussionTone(PercussionTone *t) {
	std::cout << "Playing perc tone '" << t->getFileName() << "'.\n";
}