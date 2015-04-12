// SourceNote
#include "Toolkit.h"
#include "Tempo.h"
#include "NoteTone.h"
#include "ToneCreator.h"
#include "NoteTrack.h"

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

#define S_RATE 44100

using std::min;
using namespace stk;

// This is a main file for the algorithm team's tests.
// It should not be implemented in the final release.
int main(void) {
	
	Stk::setSampleRate( (float) S_RATE );
	Stk::setRawwavePath( "samples/rawwaves/" );
	
	std::mutex mutex;
	Tempo t = Tempo(120, mutex);
	ToneCreator tc = ToneCreator();
	NoteTrack nt = tc.makeRandomMelodyNotesInRandomKeyAndRandomOctave(true);
	nt.continous = true;

	t.addNoteTrack(nt);
	t.start();
	sleep(10);

	NoteTrack nt2 = tc.makeRandomMelodyNotesInRandomKeyAndRandomOctave(false);
	nt2.continous = true;

	t.addNoteTrack(nt2);
	sleep(10);

	//PercussionTone *b = new PercussionTone(1, "kick.wav");
	
		
	
	
	// tk.playTone(0, 2, "sine", 440.0);
	//tk.playNoteTone(a);
	//tk.playPercussionTone(b);
	
	return 0;
}