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
	isRunning = true;
	pthread_t thread;
	int threadResult = pthread_create(&thread, NULL, &Tempo::run, this);
	if (threadResult){
		std::cout << "Error: Cannot create thread" << threadResult;
		exit(-1);
	}
	//pthread_t thread1;
	//threadResult = pthread_create(&thread1, NULL, &Tempo::testStuff, this);
	//sleep(30);
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

void Tempo::addMainMelodyTrack(NoteTrack track){
	while(!mtx.try_lock()){
		continue;
	}
	mainMelodyTrack = track;
	permanentMainMelodyTrack = track;
	mtx.unlock();
}

void* Tempo::run(void*temp){
	Stk::setSampleRate((float) S_RATE);
	Stk::setRawwavePath("samples/rawwaves/");

	unsigned short int beatsInAQuarterNote = 4;
	/* 'beatsInAQuarterNote' defines how many times we fetch new tones between every quarter note.
	 * If equal to 4, a single measure will have 16 beats (4 quater notes with each having 4 16th notes)
	 * If its equal to 4, we fetch every 16th note in a measure (fetch 16 times per measure).
	 * 1 would be only once per quarter note (fetch 4 times a measure).
	 * 2 would be twice per beat which would be 8th notes (fetch 8 times per measure) */
	Tempo *tempo = (Tempo*)temp;
	double sampleLengthDouble = (tempo->bpm/60);  // beats per second 
	sampleLengthDouble = 1/sampleLengthDouble; // time length in seconds between "samples" (or fetches)
	sampleLengthDouble *= 1000000; // time length in microseconds between beats (quarter notes)
	sampleLengthDouble /= beatsInAQuarterNote; // time length in microseconds between subBeats
	int sampleLength = (int)sampleLengthDouble;

	unsigned int totalBeatsProduced = 1; // total number of subBeats produced ( Assuming beatsInAQuarterNote == 4, 32 for every 2 measures, 4 per beat).
	unsigned short int currentBeatPosition = 1; // gives the current subBeat position in the standard two measure interval (loops from 1 to 32 when beatsInAQuarterNote = 4)
	/* Similar to written music, beat 1 starts at time 0 and there is no beat 0. */

	Toolkit tk = Toolkit(sampleLength);
	tk.startStream();

	auto sampleLengthMicros = std::chrono::microseconds(sampleLength);
	auto beatZeroTime = std::chrono::high_resolution_clock::now();
	auto nextBeatTime = beatZeroTime + 1 * sampleLengthMicros;

	while(tempo->isRunning){
		if (std::chrono::high_resolution_clock::now() >= nextBeatTime){
			totalBeatsProduced++;
			if (currentBeatPosition == (beatsInAQuarterNote*8)+1){ // 8 is how many quarter notes are in a 2 meaures
				currentBeatPosition = 1;
			}
			/* On a beat (Or 16th note assuming beatsInAQuarterNote == 4) */
			if (DEBUG){
				if ((currentBeatPosition-1)%beatsInAQuarterNote == 0){
					std::cout << "********** Quarter beat #" << totalBeatsProduced/beatsInAQuarterNote << " **********\n";
				} 
			}
			nextBeatTime = beatZeroTime + totalBeatsProduced * sampleLengthMicros;

			/* Get all notes for the current beat */
			std::vector<NoteTone> notesForBeat = tempo->getNoteTonesForBeatPosition(currentBeatPosition);
			std::vector<PercussionTone> percussionsForBeat = tempo->getPercussionTonesForBeatPosition(currentBeatPosition);
			for (int i = 0; i < notesForBeat.size(); i++){
				NoteTone note = notesForBeat[i];
				/* Send to STK! */
				tk.playNoteTone(&note);
			}
			/* Check main melody */
			if (tempo->mainMelodyTrack.tones.count(currentBeatPosition) != 0){
				NoteTone mainMelodyNote = tempo->mainMelodyTrack.tones[currentBeatPosition];
				tk.playNoteTone(&mainMelodyNote);
			}
			tempo->checkMainMelodyRepition(currentBeatPosition);

			for (int i = 0; i < percussionsForBeat.size(); i++){
				PercussionTone percussion = percussionsForBeat[i];
				/* Send to STK! */
			}
			currentBeatPosition++;
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
		if(track.tones.count(beatPosition) == 0){
			/* checks if the track does not have a note for the beatPosition */
			if(decrementRepeatCounters && !track.continous){
				track.repeatCount--;
				if(track.repeatCount == -1){
					noteTracks.erase(noteTracks.begin() + i);
				}
			}
			continue;
		}
		notesForBeat.push_back(track.tones[beatPosition]);
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
		if(track.tones.count(beatPosition) == 0){
			if(decrementRepeatCounters && !track.continous){
				track.repeatCount--;
				if(track.repeatCount == -1){
					percussionTracks.erase(percussionTracks.begin() + i);
				}
			}
			continue;
		}
		percussionsForBeat.push_back(track.tones[beatPosition]);
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
	/* Assumes we already checked that the note exists */
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

void Tempo::replaceMainMelody(NoteTrack newMelody){
	while(!mtx.try_lock()){
		continue;
	}
	mainMelodyTrack = newMelody;
	if(newMelody.continous){
		permanentMainMelodyTrack = newMelody;
	}
	mtx.unlock();
}
