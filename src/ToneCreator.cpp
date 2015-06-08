#include "ToneCreator.h"

#include <unordered_map>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <string>

#define DEBUG 0

std::string fileNames[] = {"clap.wav", "snare.wav", "kick.wav", "chat.wav", "cowbell.wav", "cymbal.wav", "hconga.wav", "htom.wav", 
"lconga.wav", "ltom.wav", "mconga.wav", "ohat.wav"};

int findIndexOfNoteInKey(NoteTone note, std::vector<std::string> keyNotes);

ToneCreator::ToneCreator(){
	initializeNoteData();
	srand(time(NULL));
}

void ToneCreator::initializeNoteData(){
	// Current version of Clang on Yosemite doesn't support C++ initializer lists!
	// keyNotes["C"] = {"C", "D", "E", "F", "G", "A", "B"}; -- Will not compile...
	this->noteFrequencies = {
		{"Ab1", 207.65}, 
		{"A1", 220.0}, 
		{"A#1", 233.08}, 
		{"Bb1", 233.08}, 
		{"B1", 246.94}, 
		{"C1", 261.63}, 
		{"C#1", 277.18}, 
		{"Dd1", 277.18}, 
		{"D1", 293.66}, 
		{"D#1", 311.13}, 
		{"Eb1", 311.13}, 
		{"E1", 329.63}, 
		{"F1", 349.23}, 
		{"F#1", 369.99}, 
		{"G1", 369.99}, 
		{"G#1", 415.30}, 

		{"Ab2", 415.30}, 
		{"A2", 440.00}, 
		{"A#2", 466.16}, 
		{"Bb2", 466.16}, 
		{"B2", 493.88}, 
		{"C2", 523.25}, 
		{"C#2", 554.37}, 
		{"Dd2", 554.37}, 
		{"D2", 587.33}, 
		{"D#2", 622.25}, 
		{"Eb2", 622.25}, 
		{"E2", 659.25}, 
		{"F2", 698.46}, 
		{"F#2", 739.99}, 
		{"G2", 783.99}, 
		{"G#2", 830.61}, 
	};

	/* Major scale */

	std::vector<std::string> cMajorNotes = {"C", "D", "E", "F", "G", "A", "B"};
	majorKeyNotes["C"] = cMajorNotes;
	this->allKeys.push_back("C");

	std::vector<std::string> dMajorNotes = {"D", "E", "F#", "G", "A", "B", "C#"};
	majorKeyNotes["D"] = dMajorNotes;
	this->allKeys.push_back("D");

	std::vector<std::string> gMajorNotes = {"G", "A", "B", "C", "D", "E", "F#"};
	majorKeyNotes["G"] = gMajorNotes;
	this->allKeys.push_back("G");

	std::vector<std::string> aMajorNotes = {"A", "B", "C#", "D", "E", "F#", "G#"};
	majorKeyNotes["A"] = aMajorNotes;
	this->allKeys.push_back("A");

	std::vector<std::string> eMajorNotes = {"E", "F#", "G#", "A", "B", "C#", "D#"};
	majorKeyNotes["E"] = eMajorNotes;
	this->allKeys.push_back("E");

	/* Minor scale */

	std::vector<std::string> cMinorNotes = {"C", "D", "Eb", "F", "G", "Ab", "Bb"};
	minorKeyNotes["C"] = cMinorNotes;
	this->allKeys.push_back("C");

	std::vector<std::string> dMinorNotes = {"D", "E", "F", "G", "A", "Bb", "C"};
	minorKeyNotes["D"] = dMinorNotes;
	this->allKeys.push_back("D");

	std::vector<std::string> gMinorNotes = {"G", "A", "Bb", "C", "D", "Eb", "F"};
	minorKeyNotes["G"] = gMinorNotes;
	this->allKeys.push_back("G");

	std::vector<std::string> aMinorNotes = {"A", "B", "C", "D", "E", "F", "G"};
	minorKeyNotes["A"] = aMinorNotes;
	this->allKeys.push_back("A");

	std::vector<std::string> eMinorNotes = {"E", "F#", "G", "A", "B", "C", "D"};
	minorKeyNotes["E"] = eMinorNotes;
	this->allKeys.push_back("E");

	fileNames[kSnare] = "snare.wav";
	fileNames[kKick] = "kick.wav";
	fileNames[kClap] = "clap.wav";

	currentScale = kMajor;
}

bool ToneCreator::mainMelodyExists(){
	return mainMelody.continous ? true : false;
}

NoteTone ToneCreator::makeToneWithNote(std::string note){
	note += "1"; //In octave 1
	return NoteTone(1,5, kSine, noteFrequencies.at(note), note);
}

NoteTone ToneCreator::makeRandomNoteInScaleAndKeyAndOctave(ScaleType scale, std::string key, unsigned short int octave){
	currentKey = key;
	std::vector<std::string> notesInKey = (scale == kMajor) ? majorKeyNotes[key] : minorKeyNotes[key];
	int randomNoteInKeyIndex = rand() % notesInKey.size();
	if (DEBUG){
		std::cout << notesInKey.size() << " random: " << randomNoteInKeyIndex << "\n";
	}
	std::string randomNote = notesInKey[randomNoteInKeyIndex];
	randomNote += ('0' + octave);
	return NoteTone(1,2,kSine,noteFrequencies[randomNote], randomNote);
}

NoteTone ToneCreator::makeRandomNoteInTimeFrameAndScaleAndKeyAndOctave(unsigned short int startTime, unsigned short int endTime, ScaleType scale, std::string key, unsigned short int octave){
	//The startBeat and endBeat of the note can be on, or between, startTime and endTime 
	std::vector<std::string> notesInKey = (scale == kMajor) ? majorKeyNotes[key] : minorKeyNotes[key];
	int randomNoteIndex = rand() % notesInKey.size();
	std::string randomNote = notesInKey[randomNoteIndex];
	randomNote += ('0' + octave);
	int startBeat = rand() % endTime + 1;
	//For now, every note will be a 16th note long. So the endBeat will be startBeat+1
	if (DEBUG){
		std::cout << "Random melody note: " << randomNote << " on beat:" << startBeat << "\n";
	}
	return NoteTone(startBeat,startBeat+1,kSine,noteFrequencies[randomNote], randomNote);
}

NoteTrack ToneCreator::makeRandomMelodyNotesInScaleAndKeyAndOctave(ScaleType scale, std::string key, unsigned short int octave){
	//For now, a melody will consist of 15 - 20 notes, 2 measures in length 
	int randomNumberOfNotes = rand() % 6 + 15;
	std::vector<NoteTone> notes;
	currentKey = key;

	for (int i = 0; i < randomNumberOfNotes; i++){
		NoteTone newNote = makeRandomNoteInTimeFrameAndScaleAndKeyAndOctave(1, 32, scale, "C", 1);
		notes.push_back(newNote);
	}
	return NoteTrack(notes);
}

NoteTrack ToneCreator::makeRandomMelodyNotesInRandomKeyWithOctave(int octave, bool isMainMelody){
	//For now, a melody will consist of 15 - 20 notes, 2 measures in length 
	int randomNumberOfNotes = rand() % 6 + 15;
	std::vector<NoteTone> notes;
	std::string randomKey = allKeys[rand() % allKeys.size()];
	currentKey = randomKey;
	if (DEBUG){
		std::cout << "Random melody in key:" << randomKey << " and octave:" << octave << "\n";
	}
	for (int i = 0; i < randomNumberOfNotes; i++){
		NoteTone newNote = makeRandomNoteInTimeFrameAndScaleAndKeyAndOctave(1,32, kMajor, randomKey, octave);
		notes.push_back(newNote);
	}
	NoteTrack melody = NoteTrack(notes);
	if (isMainMelody){
		mainMelody = melody;
		melody.continous = true;
	}
	return melody;
}

NoteTrack ToneCreator::makeRandomMelodyNotesInRandomKeyAndRandomOctave(bool isMainMelody){
	//For now, a melody will consist of 15 - 20 notes, 2 measures in length 
	int randomNumberOfNotes = rand() % 6 + 15;
	std::vector<NoteTone> notes;
	std::string randomKey = allKeys[rand() % allKeys.size()];
	currentKey = randomKey;
	int randomOctave = rand() % 2 + 1;
	if (DEBUG){
		std::cout << "Random melody in key:" << randomKey << " and octave:" << randomOctave << "\n";
	}
	for (int i = 0; i < randomNumberOfNotes; i++){
		NoteTone newNote = makeRandomNoteInTimeFrameAndScaleAndKeyAndOctave(1,32, kMajor, randomKey, randomOctave);
		notes.push_back(newNote);
	}
	NoteTrack melody = NoteTrack(notes);
	if (isMainMelody){
		mainMelody = melody;
		melody.continous = true;
	}
	return melody;
}

NoteTrack ToneCreator::makeMainMelodyScaleInNewScale(ScaleType newScale, ScaleType oldScale){
	if(newScale == oldScale || newScale == oldScale){
		return mainMelody;
	}
	if (mainMelody.isEmpty()){
		std::cout << "Cannot change scale of nonexistent main melody.";
		exit(1);
	}
	std::vector<NoteTone> oldNotes;
	std::unordered_map<unsigned short int, NoteTone> noteMap = mainMelody.tones();
	for (unsigned short int i = 1; i <= 32; i++){
		if(noteMap.count(i) != 0){
			oldNotes.push_back(noteMap[i]);
		}
	}
	std::vector<NoteTone> newNotes;
	std::vector<std::string> oldKeyNotes;
	std::vector<std::string> newKeyNotes;

	if(newScale == kMajor){
		currentScale = kMajor;
		newKeyNotes = majorKeyNotes[currentKey];
		oldKeyNotes = minorKeyNotes[currentKey];
	}else{
		currentScale = kMinor;
		oldKeyNotes = majorKeyNotes[currentKey];
		newKeyNotes = minorKeyNotes[currentKey];
	}
	if (DEBUG) std::cout << "Current key " << currentKey << "\n";

	for(int i = 0; i < oldNotes.size(); i++){
		NoteTone note = oldNotes[i];
		int noteNameSize = note.getNoteName().size();
		std::string octave = note.getNoteName().substr((noteNameSize-1),1);
		std::string newNoteName = newKeyNotes[findIndexOfNoteInKey(note,oldKeyNotes)];
		newNoteName += octave;
		newNotes.push_back(NoteTone(note.getStartBeatPosition(), note.getEndBeatPosition(), note.getWaveform(), noteFrequencies.at(newNoteName), newNoteName));
	}
	return NoteTrack(newNotes);
}

int findIndexOfNoteInKey(NoteTone note, std::vector<std::string> keyNotes){
	std::string noteNameWithoutOctave = note.getNoteName().substr(0,note.getNoteName().size()-1);
	std::string noteWithOnlyLetter = (noteNameWithoutOctave.size() > 1) ? noteNameWithoutOctave.substr(0,1) : noteNameWithoutOctave;
	if (DEBUG) std::cout << "Searching for " << noteWithOnlyLetter << "\n";
	for (int i = 0 ; i < keyNotes.size(); i++){
		if (DEBUG) std::cout << noteWithOnlyLetter << " == " << keyNotes[i] << " ?\n";
		std::string currentPossibleNoteMatch = keyNotes[i];
		if (currentPossibleNoteMatch.size() > 1){
			currentPossibleNoteMatch = currentPossibleNoteMatch.substr(0,1);
		}
		if(noteWithOnlyLetter == currentPossibleNoteMatch){
			return i;
		}
	}
	//This should never be encountered...
	std::cout << "Uh Oh!\n"; 
	return -1;
}

PercussionTrack ToneCreator::makePercussionTrackWithTypeAndBeats(PercussionToneType type, std::vector<unsigned short int> beats){
	if (DEBUG){
		std::cout << "Generating " << beats.size() << " beats\n";
	}
	std::vector<PercussionTone> percussions;
	for(int i = 0; i < beats.size(); i++){
		percussions.push_back(PercussionTone(beats[i], fileNames[type]));
		PercussionTone tone = percussions[i];
		if (DEBUG){
			std::cout << "PercussionTone: " << tone.getStartBeatPosition() << " " << tone.getFileName() << "\n";
		}
	}
	return PercussionTrack(percussions);
}

PercussionTrack ToneCreator::makeRandomBeatWithPercussionType(PercussionToneType type){
	// Chooses randomly between half notes, quarter notes, or 8th notes.
	int random = rand() % 3;
	std::vector<PercussionTone> percussions;
	std::string fileName = fileNames[type];
	switch (random){
		case 0:{
			// half notes, aka every other quarter note
			// choose randomly between starting on beat 1 or 2
			int randomStartBeat = rand() % 2;
			if (DEBUG){
				std::cout << "Generating half notes, ";
				if(randomStartBeat == 1){
					std::cout << "starting on beat 1\n";
				}else{
					std::cout << "starting on beat 2\n";
				}
			}
			for (int i = 1; i <= 32; i++){
				if(randomStartBeat == 1){
					// start on beat 1
					if(((i - 1) % 2) == 0){
						PercussionTone tone = PercussionTone(i, fileName);
						percussions.push_back(tone);
					}
				}else{
					// start on beat 2
					if((i % 2) == 0){
						PercussionTone tone = PercussionTone(i, fileName);
						percussions.push_back(tone);
					}
				}
			}
			break;
		}

		case 1:{
			// quarter notes
			if (DEBUG) std::cout << "Generating quarter notes\n";
			for (int i = 1; i <= 32; i++){
				if(((i - 1) % 4) == 0){
					PercussionTone tone = PercussionTone(i, fileName);
					percussions.push_back(tone);
				}
			}
			break;
		}

		case 2:{
			// 8th notes, aka every half quarter note
			if (DEBUG) std::cout << "Generating 8th notes\n";
			for (int i = 1; i <= 32; i++){
				if(((i - 1) % 2) == 0){
					PercussionTone tone = PercussionTone(i, fileName);
					percussions.push_back(tone);
				}
			}
			break;
		}
	}
	if (DEBUG) std::cout << "\n";
	return PercussionTrack(percussions);

}