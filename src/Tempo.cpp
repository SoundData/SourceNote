#include "Tempo.h"

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "Toolkit.h"

#define S_RATE 44100
#define DEBUG 0

using std::min;
using namespace stk;


Tempo::Tempo(int beatsPerMinute, std::mutex& mutex) : bpm(beatsPerMinute), mtx(mutex) {};

void Tempo::start(){
	if (mainMelodyTrack.isEmpty() || (noteTracks.empty() && percussionTracks.empty())){
		std::cout << "Cannot start because no initial music was generated.";
		exit(1);
	}
	isRunning = true;
	pthread_t thread;
	int threadResult = pthread_create(&thread, NULL, &Tempo::run, this);
	if (threadResult){
		std::cout << "Error: Cannot create thread" << threadResult;
		exit(1);
	}
}

void Tempo::stop(){
	isRunning = false;
}

void Tempo::setNewBPM(int bpm){
	this->bpm = bpm;
}

void Tempo::addNoteTones(std::vector<NoteTone> tones){
	while(!mtx.try_lock()){
		continue;
	}
	NoteTrack track = NoteTrack(tones);
	noteTracks.push_back(track);
	mtx.unlock();
}

void Tempo::addPercussionTones(std::vector<PercussionTone> tones){
	while(!mtx.try_lock()){
		continue;
	}
	PercussionTrack track = PercussionTrack(tones);
	percussionTracks.push_back(track);
	mtx.unlock();
}

void Tempo::addNoteTrack(NoteTrack track){
	while(!mtx.try_lock()){
		continue;
	}
	noteTracks.push_back(track);
	mtx.unlock();
}

void Tempo::addPercussionTrack(PercussionTrack track, bool removeAllExistingPercussionTracks){
	while(!mtx.try_lock()){
		continue;
	}
	if (removeAllExistingPercussionTracks){
		/* make this better later ! */
		std::vector<PercussionTrack> newPercussionTracks;
		newPercussionTracks.push_back(track);
		percussionTracks = newPercussionTracks;
	}else{
		percussionTracks.push_back(track);
	}

	mtx.unlock();
}

void Tempo::setMainMelodyTrack(NoteTrack newMelody){
	while(!mtx.try_lock()){
		continue;
	}
	mainMelodyTrack = newMelody;
	if(newMelody.continous){
		permanentMainMelodyTrack = newMelody;
	}
	mtx.unlock();
}

void* Tempo::run(void* temp){
	/* Toolkit setup */
	Stk::setSampleRate((float) S_RATE);
	Stk::setRawwavePath("samples/rawwaves/");

	Tempo *tempo = (Tempo*)temp;
	unsigned short int beatsInAQuarterNote = 4;
	// 'beatsInAQuarterNote' defines how many times we fetch new tones between every quarter note.
	// If equal to 4, a single measure will have 16 beats (4 quater notes with each having 4 16th notes)
	// If its equal to 4, we fetch every 16th note in a measure (fetch 16 times per measure).
	// 1 would be only once per quarter note (fetch 4 times a measure).
	// 2 would be twice per beat which would be 8th notes (fetch 8 times per measure)

	 /* Sample length calculation */
	 // Terminology:
	 // "Sample length" is referring to the time between each new note fetch, or note "sample".
	 // "Sample beat" is referring to the points in time when new notes are fetched to play. Since the song is most likely in
	 //		4/4 time signature, you can also call this a 16th note beat.
	 // "Beat" is referring to a quarter note. */
	double sampleLengthDouble = (tempo->bpm/60);  // beats per second.
	sampleLengthDouble = 1/sampleLengthDouble; // time length in seconds between quarter notes
	sampleLengthDouble *= 1000000; // time length in microseconds between beats (quarter notes)
	sampleLengthDouble /= beatsInAQuarterNote; // time length in microseconds between sample beats, AKA "samples" (or note fetches)
	int sampleLength = (int)sampleLengthDouble;

	/* Loop counters */
	unsigned int totalSampleBeatsProduced = 1; // total number of subBeats produced (Assuming beatsInAQuarterNote == 4, 32 for every 2 measures, 4 per beat). AKA "sample" rate
	unsigned short int currentSampleBeatPosition = 1; // gives the current subBeat position in the standard two measure interval (loops from 1 to 32 when beatsInAQuarterNote = 4)
	// Similar to written music, beat 1 starts at time 0 and there is no beat 0.

	Toolkit tk = Toolkit(sampleLength);
	tk.startStream();

	/* Time keepers */
	auto sampleLengthMicros = std::chrono::microseconds(sampleLength);
	auto beatZeroTime = std::chrono::high_resolution_clock::now();
	auto nextBeatTime = beatZeroTime + 1 * sampleLengthMicros;


	while(tempo->isRunning){
		if (std::chrono::high_resolution_clock::now() >= nextBeatTime){
			// On a sample beat (Or 16th note assuming beatsInAQuarterNote == 4)
			totalSampleBeatsProduced++;
			if (currentSampleBeatPosition == (beatsInAQuarterNote*8)+1){ // 8 is how many quarter notes are in a 2 meaures
				// If a new measure interval has been reached, start back on beat 1 
				currentSampleBeatPosition = 1;
			}
			if (DEBUG){
				if ((currentSampleBeatPosition-1)%beatsInAQuarterNote == 0){
					std::cout << "********** Quarter beat #" << totalSampleBeatsProduced/beatsInAQuarterNote << " **********\n";
				} 
			}
			nextBeatTime = beatZeroTime + totalSampleBeatsProduced * sampleLengthMicros;

			// Get all notes for the current beat
			std::vector<NoteTone> notesForBeat = tempo->getNoteTonesForBeatPosition(currentSampleBeatPosition);
			std::vector<PercussionTone> percussionsForBeat = tempo->getPercussionTonesForBeatPosition(currentSampleBeatPosition);
			for (int i = 0; i < notesForBeat.size(); i++){
				NoteTone note = notesForBeat[i];
				tk.playNoteTone(&note);
			}
			// Check main melody
			if (tempo->mainMelodyTrack.tones().count(currentSampleBeatPosition) != 0){
				NoteTone mainMelodyNote = tempo->mainMelodyTrack.tones()[currentSampleBeatPosition];
				tk.playNoteTone(&mainMelodyNote);
			}
			tempo->checkMainMelodyRepition(currentSampleBeatPosition);

			for (int i = 0; i < percussionsForBeat.size(); i++){
				PercussionTone percussion = percussionsForBeat[i];
			}
			currentSampleBeatPosition++;
		}else{
			std::this_thread::yield();
			continue;
		}
	}
	int tkResult = tk.stopStream();
	if (DEBUG){
		std::cout << "Toolkit stop result: " << tkResult;
	}
	return NULL;
}

std::vector<NoteTone> Tempo::getNoteTonesForBeatPosition(unsigned short int beatPosition){
	while(!mtx.try_lock()){
		continue;
	}
	std::vector<NoteTone> notesForBeat;
	bool decrementRepeatCounters = (beatPosition == 32) ? true : false;
	for(int i = 0; i < noteTracks.size(); i++){
		NoteTrack& track = noteTracks[i];
		if(track.tones().count(beatPosition) == 0){
			// checks if the track does not have a note for the beatPosition
			if(decrementRepeatCounters && !track.continous){
				track.repeatCount--;
				if(track.repeatCount == -1){
					noteTracks.erase(noteTracks.begin() + i);
				}
			}
			continue;
		}
		notesForBeat.push_back(track.tones()[beatPosition]);
		if(decrementRepeatCounters && !track.continous){
			track.repeatCount--;
			if(track.repeatCount == -1){
				noteTracks.erase(noteTracks.begin() + i);
			}
		}
	}
	mtx.unlock();
	return notesForBeat;
}

std::vector<PercussionTone> Tempo::getPercussionTonesForBeatPosition(unsigned short int beatPosition){
	while(!mtx.try_lock()){
		continue;
	}
	std::vector<PercussionTone> percussionsForBeat;
	bool decrementRepeatCounters = (beatPosition == 32) ? true : false;
	for(int i = 0; i < percussionTracks.size(); i++){
		PercussionTrack& track = percussionTracks[i];
		if(track.tones().count(beatPosition) == 0){
			if(decrementRepeatCounters && !track.continous){
				track.repeatCount--;
				if(track.repeatCount == -1){
					percussionTracks.erase(percussionTracks.begin() + i);
				}
			}
			continue;
		}
		percussionsForBeat.push_back(track.tones()[beatPosition]);
		if(decrementRepeatCounters && !track.continous){
			track.repeatCount--;
			if(track.repeatCount == -1){
				percussionTracks.erase(percussionTracks.begin() + i);
			}
		}
	}
	mtx.unlock();
	return percussionsForBeat;

}

void Tempo::checkMainMelodyRepition(unsigned short int beatPosition){
	// This method decrements the main melody's repeatCounter if it's on the last note of the max track length, and it's continous.
	// Usually it's two measures long, so on the 32nd beat.
	// If the main melody has no more repeats, replace it with the permanentMainMelodyTrack which is always continous.
	// Assumes we already checked that the note exists
	while(!mtx.try_lock()){
		continue;
	}
	NoteTrack& track = mainMelodyTrack;
	bool decrementRepeatCounter = (beatPosition == 32) ? true : false;
	if(!track.continous && decrementRepeatCounter){
		track.repeatCount--;
		if(track.repeatCount == -1){
			mainMelodyTrack = permanentMainMelodyTrack;
		}
	}
	mtx.unlock();
	return;
}
