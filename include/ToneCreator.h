#ifndef __TONECREATOR_H_INCLUDED__
#define __TONECREATOR_H_INCLUDED__

// ToneCreator.h
//
// ToneCreator supplies functions for generating Tones and Tracks (groups of Tones).
// It can generate random and nonrandom Tones that are customized by using different functions whose arguments
// allow different types of customizability.
//
// While generating new Tracks (nicknamed "melodies"), it automatically keeps track of specific attributes 
// that should be global to all generated melodies such as a key, scale, and the main melody.
// Keeping track of the main melody is important for generating slightly altered versions of the main melody
// so the main melody can be altered in real time (most commonly, when an event changes the scale of the main melody).

#include <unordered_map>
#include <vector>
#include <iostream>

#include "Tone.h"
#include "NoteTone.h"
#include "PercussionTone.h"
#include "NoteTrack.h"
#include "PercussionTrack.h"


 enum PercussionToneType{
	kClap = 0,
	kSnare,
	kKick,
	kChat,
	kCowbell,
	kCymbal,
	kHconga,
	kHatom,
	kLconga,
	kLtom,
	kMconga,
	kOhat
};

enum ScaleType{
	kMajor = 0,
	kMinor
};

class ToneCreator{
	private:
		std::unordered_map<std::string, float> noteFrequencies;
		// Map keys are in the format of "<Note letter A-G><#,d,or nothing><1,2,or 3>"
		// Examples:
		// "A#1" - 'A' sharp first octave
		// "C1" - 'C' first octave
		// "Bb2" - 'B' flat octave 2
		// "Ab3" - 'A' flat octave 3 
		std::unordered_map< std::string, std::vector <std::string> > majorKeyNotes;
		std::unordered_map< std::string, std::vector <std::string> > minorKeyNotes;
		std::string currentKey;
		ScaleType currentScale;
		std::vector <std::string> allKeys;
		NoteTrack mainMelody;
		void initializeNoteData();
		bool mainMelodyExists();

	public:
		ToneCreator();
		NoteTone makeToneWithNote(std::string note);
		// For the "octave" arguments in the following functions: there are 3 octaves, so the octave should be a number from 1 to 3.
		// Examples of key: "C", "A", "D"
		NoteTone makeRandomNoteInScaleAndKeyAndOctave(ScaleType scale, std::string key, unsigned short int octave);
		// StartTime and endTime should be a 16th note beat within a 2 measure interval, so between 1 and 32.
		NoteTone makeRandomNoteInTimeFrameAndScaleAndKeyAndOctave(unsigned short int startTime, unsigned short int endTime, ScaleType scale, std::string key, unsigned short int octave);
		NoteTrack makeRandomMelodyNotesInScaleAndKeyAndOctave(ScaleType scale, std::string key, unsigned short int octave);
		// Octave can be 1 through 2
		NoteTrack makeRandomMelodyNotesInRandomKeyWithOctave(int octave, bool isMainMelody);
		// Major scale by default
		NoteTrack makeRandomMelodyNotesInRandomKeyAndRandomOctave(bool isMainMelody);
		NoteTrack makeMainMelodyScaleInNewScale(ScaleType newScale, ScaleType oldScale);
		// Following function not yet implemented
		NoteTrack makeRandomMelodyInCurrentKeyAndInRange(unsigned short int minimumNumberOfNotes, unsigned short int maximumNumberOfNotes);

		// Percussion generation functions
		// Assuming Tempo's samples new tones 4 times for every quarter note, beats should contain numbers ranging from 1 to 32.
		PercussionTrack makePercussionTrackWithTypeAndBeats(PercussionToneType type, std::vector<unsigned short int> beats);
		// Creates a steady beat of percussion tones. It randomly chooses between making half notes, quarter notes, or 8th notes (only one type is chosen).
		PercussionTrack makeRandomBeatWithPercussionType(PercussionToneType type);
};

#endif