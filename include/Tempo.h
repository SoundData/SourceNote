#ifndef __TEMPO_H_INCLUDED__
#define __TEMPO_H_INCLUDED__

// Tempo.h
//
// Tempo keeps track of time and the song's Tracks, and sends the Tones to Toolkit at the
// particular time that they should be played.
// It's given an intial BPM (beats per measure) that it uses to know when to send particular
// Tones to Toolkit.

#include <Vector>
#include <mutex>

#include "NoteTone.h"
#include "PercussionTone.h"
#include "NoteTrack.h"
#include "PercussionTrack.h"

class Tempo{
	private:
		unsigned int bpm;
		bool isRunning;
		NoteTrack mainMelodyTrack;
		// PermanentMainMelodyTrack is used as a "back up" main melody when the mainMelodyTrack is replaced with
		// a new non-continous track using replaceMainMelodyTrack(). Once this new main melody runs out of repeats
		// (because its non-continous), the permanentMainMelody will replace it (which is always continous)
		NoteTrack permanentMainMelodyTrack;
		static void* run(void*);
		std::vector<NoteTone> getNoteTonesForBeatPosition(unsigned short int beatPosition);
		std::vector<PercussionTone> getPercussionTonesForBeatPosition(unsigned short int beatPosition);
		void checkMainMelodyRepition(unsigned short int beatPosition);

	public:
		std::vector<NoteTrack> noteTracks;
		std::vector<PercussionTrack> percussionTracks;
		std::mutex &mtx;
		Tempo(int beatsPerMinute, std::mutex& mutex);
		void start();
		void stop();
		void setNewBPM(int bpm); // set initial bpm in the constructor
		void addNoteTones(std::vector<NoteTone> tones);
		void addPercussionTones(std::vector<PercussionTone> tones);
		void addNoteTrack(NoteTrack track);
		void addPercussionTrack(PercussionTrack track, bool removeAllExistingPercussionTracks);
		void setMainMelodyTrack(NoteTrack newMelody);
};

#endif