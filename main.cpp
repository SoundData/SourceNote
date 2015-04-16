// SourceNote
#include "Toolkit.h"
#include "Tempo.h"
#include "NoteTone.h"
#include "ToneCreator.h"
#include "NoteTrack.h"
#include "SNCore.h"

// STK
#include "Instrmnt.h"
#include "JCRev.h"
#include "Sitar.h"
#include "Messager.h"
#include "RtAudio.h"

// std lib
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <mutex>


// This is a main file for the algorithm team's tests.
// It should not be implemented in the final release.
int main(void) {

	/*** GameMessage testing ***/
	/*
	std::mutex mutex;
	SNCore core = SNCore(mutex);

	GameMessage *gm = new GameMessage();
	gm->eventType = std::string("PLAYER_CHANGECLASS");
	gm->info = new std::map<std::string, std::string>();
	gm->info->insert( std::pair<std::string, std::string>("PlayerName", "JoeMaag"));
	gm->info->insert( std::pair<std::string, std::string>("PlayerClass", "Scout"));

	core.start(gm);
	sleep(6);

	// GameMessage *gm2 = new GameMessage();
	// gm2->eventType = std::string("PLAYER_DEATH");
	// gm2->info = new std::map<std::string, std::string>();
	// std::cout << "death\n";
	// core.decodeMessage(gm2);
	// sleep(20);

	// std::cout << "death\n";
	// core.decodeMessage(gm2);
	// sleep(15);

	// 	GameMessage *gm5 = new GameMessage();
	// gm5->eventType = std::string("PLAYER_SPAWN");
	// gm5->info = new std::map<std::string, std::string>();
	// gm5->info->insert( std::pair<std::string, std::string>("PlayerClass", "Scout"));
	// std::cout << "spawn\n";
	// core.decodeMessage(gm5);
	// sleep(20);

	GameMessage *gm3 = new GameMessage();
	gm3->eventType = std::string("PLAYER_CHANGECLASS");
	gm3->info = new std::map<std::string, std::string>();
	gm3->info->insert( std::pair<std::string, std::string>("PlayerName", "JoeMaag"));
	gm3->info->insert( std::pair<std::string, std::string>("PlayerClass", "Heavy"));
	core.decodeMessage(gm3);
	sleep(6);
*/
	

	
	std::mutex mutex;
	Tempo t = Tempo(120, mutex);
	ToneCreator tc = ToneCreator();
	NoteTrack nt = tc.makeRandomMelodyNotesInRandomKeyWithOctave(1,true); //octave 1, main melody
	nt.continous = false;
	nt.repeatCount = 1;

	//  Uncomment for percussion testing 
	// PercussionTrack pt = tc.makeRandomBeatWithPercussionType(kSnare);
	// pt.addPercussionTrack(pt, false); 
	
	
	t.addNoteTrack(nt);
	t.start();
	sleep(5);

	NoteTrack nt2 = tc.makeRandomMelodyNotesInRandomKeyWithOctave(1,false);
	nt2.continous = true;

	t.addNoteTrack(nt2);
	sleep(5);


	NoteTrack nt3 = tc.makeRandomMelodyNotesInRandomKeyWithOctave(2,false);
	nt3.continous = true;

	t.addNoteTrack(nt3);
	sleep(20);
	
	
	// tk.playTone(0, 2, "sine", 440.0);
	// tk.playNoteTone(a);
	// tk.playPercussionTone(b);
	
	return 0;
}