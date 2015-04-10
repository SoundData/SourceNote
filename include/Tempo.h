#ifndef __TEMPO_H_INCLUDED__
#define __TEMPO_H_INCLUDED__
#include "NoteTone.h"
#include "PercussionTone.h"
#include "NoteTrack.h"
#include "PercussionTrack.h"
#include <Vector>
#include <mutex>

class Tempo{
	private:
		unsigned int bpm;
		bool isRunning;
		static void* run(void*);
		std::vector<NoteTone> getNoteTonesForBeatPosition(unsigned short int beatPosition);
		std::vector<PercussionTone> getPercussionTonesForBeatPosition(unsigned short int beatPosition);

	public:
		std::vector<NoteTrack> noteTracks;
		std::vector<PercussionTrack> percussionTracks;
		std::mutex &mtx;
		Tempo(int beatsPerMinute, std::mutex& mutex);
		void start();
		void stop();
		void addNoteTones(std::vector<NoteTone> tones);
		void addPercussionTones(std::vector<PercussionTone> tones);
		//static void* testStuff(void*temp);  For testing
};

#endif