#include "ToneCreator.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <time.h>
#include <string>


std::string fileNames[] = {"clap.wav", "snare.wav", "kick.wav", "chat.wav", "cowbell.wav", "cymbal.wav", "hconga.wav", "htom.wav", 
"lconga.wav", "ltom.wav", "mconga.wav", "ohat.wav"};
int findIndexOfNoteInKey(NoteTone note, std::vector<std::string> keyNotes);

//ToneCreator::ToneCreator(std::vector<GameMessage> initialParameters) : intialParameters(intialParameters)
ToneCreator::ToneCreator(){
	initializeNoteData();
	srand(time(NULL));
}

void ToneCreator::initializeNoteData(){
	/* Current version of Clang on Yosemite doesn't support C++ initializer lists!
	 * keyNotes["C"] = {"C", "D", "E", "F", "G", "A", "B"}; -- Will not compile... */
	this->noteFrequencies["Ab1"] = 207.65;
	this->noteFrequencies["A1"] = 220.0;
	this->noteFrequencies["A#1"] = 233.08;
	this->noteFrequencies["Bb1"] = 233.08;
	this->noteFrequencies["B1"] = 246.94;
	this->noteFrequencies["C1"] = 261.63;
	this->noteFrequencies["C#1"] = 277.18;
	this->noteFrequencies["Dd1"] = 277.18;
	this->noteFrequencies["D1"] = 293.66;
	this->noteFrequencies["D#1"] = 311.13;
	this->noteFrequencies["Eb1"] = 311.13;
	this->noteFrequencies["E1"] = 329.63;
	this->noteFrequencies["F1"] = 349.23;
	this->noteFrequencies["F#1"] = 369.99;
	this->noteFrequencies["G1"] = 369.99;
	this->noteFrequencies["G#1"] = 415.30;

	this->noteFrequencies["Ab2"] = 415.30;
	this->noteFrequencies["A2"] = 440.00;
	this->noteFrequencies["A#2"] = 466.16;
	this->noteFrequencies["Bb2"] = 466.16;
	this->noteFrequencies["B2"] = 493.88;
	this->noteFrequencies["C2"] = 523.25;
	this->noteFrequencies["C#2"] = 554.37;
	this->noteFrequencies["Dd2"] = 554.37;
	this->noteFrequencies["D2"] = 587.33;
	this->noteFrequencies["D#2"] = 622.25;
	this->noteFrequencies["Eb2"] = 622.25;
	this->noteFrequencies["E2"] = 659.25;
	this->noteFrequencies["F2"] = 698.46;
	this->noteFrequencies["F#2"] = 739.99;
	this->noteFrequencies["G2"] = 783.99;
	this->noteFrequencies["G#2"] = 830.61;

	/* Major scale */

	std::vector<std::string> cMajorNotes;
	cMajorNotes.push_back("C");
	cMajorNotes.push_back("D");
	cMajorNotes.push_back("E");
	cMajorNotes.push_back("F");
	cMajorNotes.push_back("G");
	cMajorNotes.push_back("A");
	cMajorNotes.push_back("B");
	majorKeyNotes["C"] = cMajorNotes;
	this->allKeys.push_back("C");

	std::vector<std::string> dMajorNotes;
	dMajorNotes.push_back("D");
	dMajorNotes.push_back("E");
	dMajorNotes.push_back("F#");
	dMajorNotes.push_back("G");
	dMajorNotes.push_back("A");
	dMajorNotes.push_back("B");
	dMajorNotes.push_back("C#");
	majorKeyNotes["D"] = dMajorNotes;
	this->allKeys.push_back("D");

	std::vector<std::string> gMajorNotes;
	gMajorNotes.push_back("G");
	gMajorNotes.push_back("A");
	gMajorNotes.push_back("B");
	gMajorNotes.push_back("C");
	gMajorNotes.push_back("D");
	gMajorNotes.push_back("E");
	gMajorNotes.push_back("F#");
	majorKeyNotes["G"] = gMajorNotes;
	this->allKeys.push_back("G");

	std::vector<std::string> aMajorNotes;
	aMajorNotes.push_back("A");
	aMajorNotes.push_back("B");
	aMajorNotes.push_back("C#");
	aMajorNotes.push_back("D");
	aMajorNotes.push_back("E");
	aMajorNotes.push_back("F#");
	aMajorNotes.push_back("G#");
	majorKeyNotes["A"] = aMajorNotes;
	this->allKeys.push_back("A");

	std::vector<std::string> eMajorNotes;
	eMajorNotes.push_back("E");
	eMajorNotes.push_back("F#");
	eMajorNotes.push_back("G#");
	eMajorNotes.push_back("A");
	eMajorNotes.push_back("B");
	eMajorNotes.push_back("C#");
	eMajorNotes.push_back("D#");
	majorKeyNotes["E"] = eMajorNotes;
	this->allKeys.push_back("E");

	/* Minor scale */

	std::vector<std::string> cMinorNotes;
	cMinorNotes.push_back("C");
	cMinorNotes.push_back("D");
	cMinorNotes.push_back("Eb");
	cMinorNotes.push_back("F");
	cMinorNotes.push_back("G");
	cMinorNotes.push_back("Ab");
	cMinorNotes.push_back("Bb");
	minorKeyNotes["C"] = cMinorNotes;
	this->allKeys.push_back("C");

	std::vector<std::string> dMinorNotes;
	dMinorNotes.push_back("D");
	dMinorNotes.push_back("E");
	dMinorNotes.push_back("F");
	dMinorNotes.push_back("G");
	dMinorNotes.push_back("A");
	dMinorNotes.push_back("Bb");
	dMinorNotes.push_back("C");
	minorKeyNotes["D"] = dMinorNotes;
	this->allKeys.push_back("D");

	std::vector<std::string> gMinorNotes;
	gMinorNotes.push_back("G");
	gMinorNotes.push_back("A");
	gMinorNotes.push_back("Bb");
	gMinorNotes.push_back("C");
	gMinorNotes.push_back("D");
	gMinorNotes.push_back("Eb");
	gMinorNotes.push_back("F");
	minorKeyNotes["G"] = gMinorNotes;
	this->allKeys.push_back("G");

	std::vector<std::string> aMinorNotes;
	aMinorNotes.push_back("A");
	aMinorNotes.push_back("B");
	aMinorNotes.push_back("C");
	aMinorNotes.push_back("D");
	aMinorNotes.push_back("E");
	aMinorNotes.push_back("F");
	aMinorNotes.push_back("G");
	minorKeyNotes["A"] = aMinorNotes;
	this->allKeys.push_back("A");

	std::vector<std::string> eMinorNotes;
	eMinorNotes.push_back("E");
	eMinorNotes.push_back("F#");
	eMinorNotes.push_back("G");
	eMinorNotes.push_back("A");
	eMinorNotes.push_back("B");
	eMinorNotes.push_back("C");
	eMinorNotes.push_back("D");
	minorKeyNotes["E"] = eMinorNotes;
	this->allKeys.push_back("E");

	fileNames[kSnare] = "snare.wav";
	fileNames[kKick] = "kick.wav";
	fileNames[kClap] = "clap.wav";

}

NoteTone ToneCreator::makeToneWithNote(std::string note){
	note += "1"; //In octave 1
	return NoteTone(1,5, kSine, noteFrequencies.at(note), note);
}

NoteTone ToneCreator::makeRandomNoteInScaleAndKeyAndOctave(ScaleType scale, std::string key, unsigned short int octave){
	currentKey = key;
	std::vector<std::string> notesInKey = (scale == kMajor) ? majorKeyNotes[key] : minorKeyNotes[key];
	int randomNoteInKeyIndex = rand() % notesInKey.size();
	std::cout << notesInKey.size() << " random: " << randomNoteInKeyIndex << "\n";
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
	std::cout << "Random melody note: " << randomNote << " on beat:" << startBeat << "\n";
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
	std::cout << "Random melody in key:" << randomKey << " and octave:" << octave << "\n";
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
	std::cout << "Random melody in key:" << randomKey << " and octave:" << randomOctave << "\n";
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

NoteTrack ToneCreator::changeMainMelodyScale(ScaleType newScale, ScaleType oldScale){
	std::cout << currentKey << "\n";
	std::vector<NoteTone> oldNotes;
	std::unordered_map<unsigned short int, NoteTone>& noteMap = mainMelody.tones;
	for (unsigned short int i = 1; i <= 32; i++){
		if(noteMap.count(i) != 0){
			oldNotes.push_back(noteMap[i]);
		}
	}
	std::vector<NoteTone> newNotes;
	/* use pointer later  */
	std::vector<std::string> oldKeyNotes;
	std::vector<std::string> newKeyNotes;

	if(newScale == kMajor){
		newKeyNotes = majorKeyNotes[currentKey];
		oldKeyNotes = minorKeyNotes[currentKey];
	}else{
		oldKeyNotes = majorKeyNotes[currentKey];
		newKeyNotes = minorKeyNotes[currentKey];
	}

	for(int i = 0; i < oldNotes.size(); i++){
		NoteTone note = oldNotes[i];
		int noteNameSize = note.getNoteName().size();
		std::string octave = note.getNoteName().substr((noteNameSize-1),1);
		// std::string noteLetter = note.getNoteName().substr(0,1); //ignore sharps or flats
		std::string newNoteName = newKeyNotes[findIndexOfNoteInKey(note,oldKeyNotes)];
		newNoteName += octave;
		//std::cout << "New note: " << newNoteName << "\n";
		newNotes.push_back(NoteTone(note.getStartBeatPosition(), note.getEndBeatPosition(), note.getWaveform(), noteFrequencies.at(newNoteName), newNoteName));
	}
	return NoteTrack(newNotes);
}

int findIndexOfNoteInKey(NoteTone note, std::vector<std::string> keyNotes){
	std::string noteNameWithoutOctave = note.getNoteName().substr(0,note.getNoteName().size()-1);
	//std::cout << "Searching for " << noteNameWithoutOctave << "\n";
	for (int i = 0 ; i < keyNotes.size(); i++){
		if(noteNameWithoutOctave == keyNotes[i]){
			return i;
		}
	}
	std::cout << "Uh Oh!\n"; 
	return -1;
}

PercussionTrack ToneCreator::makePercussionTrackWithTypeAndBeats(PercussionToneType type, std::vector<unsigned short int> beats){
	std::cout << "Generating " << beats.size() << " beats\n";
	std::vector<PercussionTone> percussions;
	for(int i = 0; i < beats.size(); i++){
		percussions.push_back(PercussionTone(beats[i], fileNames[type]));
		PercussionTone tone = percussions[i];
		std::cout << "PercussionTone: " << tone.getStartBeatPosition() << " " << tone.getFileName() << "\n";
	}
	std::cout << "\n";
	return PercussionTrack(percussions);
}

PercussionTrack ToneCreator::makeRandomBeatWithPercussionType(PercussionToneType type){
	/* chooses randomly between half notes, quarter notes, or 8th notes. */
	int random = rand() % 3;
	std::vector<PercussionTone> percussions;
	std::string fileName = fileNames[type];
	switch (random){
		case 0:{
			/* half notes, aka every other quarter note */
			/* choose randomly between starting on beat 1 or 2 */
			std::cout << "Generating half notes, ";
			int randomStartBeat = rand() % 2;
			if(randomStartBeat == 1){
				std::cout << "starting on beat 1\n";
			}else{
				std::cout << "starting on beat 2\n";
			}
			for (int i = 1; i <= 32; i++){
				if(randomStartBeat == 1){
					/* start on beat 1 */
					if(((i - 1) % 2) == 0){
						PercussionTone tone = PercussionTone(i, fileName);
						percussions.push_back(tone);
					}
				}else{
					/* start on beat 2 */
					if((i % 2) == 0){
						PercussionTone tone = PercussionTone(i, fileName);
						percussions.push_back(tone);
					}
				}
			}
			break;
		}

		case 1:{
			/* quarter notes */
			std::cout << "Generating quarter notes\n";
			for (int i = 1; i <= 32; i++){
				if(((i - 1) % 4) == 0){
					PercussionTone tone = PercussionTone(i, fileName);
					percussions.push_back(tone);
				}
			}
			break;
		}

		case 2:{
			/* 8th notes, aka every half quarter note */
			std::cout << "Generating 8th notes\n";
			for (int i = 1; i <= 32; i++){
				if(((i - 1) % 2) == 0){
					PercussionTone tone = PercussionTone(i, fileName);
					percussions.push_back(tone);
				}
			}
			break;
		}
	}
	std::cout << "\n";
	return PercussionTrack(percussions);

}


/*
int main(){
	ToneCreator tc = ToneCreator();
	*/

	/***** Uncomment to try note functions *********
	NoteTrack melody = tc.makeRandomMelodyNotesInKeyAndOctave("C", 1);
	std::cout << "Above melody includes " << melody.tones.size() << " notes\n\n";

	NoteTrack melody2 = tc.makeRandomMelodyNotesInRandomKeyAndRandomOctave();
	std::cout << "Above melody includes " << melody2.tones.size() << " notes\n\n";
	***************/


	/***** Uncomment to try percussion functions ****
	std::vector<unsigned short int> beats;
	beats.push_back(1);
	beats.push_back(2);
	beats.push_back(3);
	beats.push_back(4);
	beats.push_back(5);

	PercussionTrack track1 = tc.makePercussionTrackWithTypeAndBeats(kClap, beats);
	PercussionTrack track2 = tc.makeRandomBeatWithPercussionType(kKick);
	**************/

	/*
	NoteTrack coolMelody = tc.makeRandomMelodyNotesInRandomKeyAndRandomOctave(true);
	std::cout << "\n\nNow the scale change:\n";
	NoteTrack coolMelodyInMinor = tc.changeMainMelodyScale(kMinor, kMajor);


	return 0;
}
*/

