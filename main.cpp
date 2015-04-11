// SourceNote
#include "Toolkit.h"
#include "Tempo.h"
#include "NoteTone.h"
#include "NoteTone.h"

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

#define S_RATE 44100

using std::min;
using namespace stk;

// This is a main file for the algorithm team's tests.
// It should not be implemented in the final release.
int main(void) {
	
	Stk::setSampleRate( (float) S_RATE );
	Stk::setRawwavePath( "samples/rawwaves/" );
	
	NoteTone *a = new NoteTone(0, 32, kSine, 220.00, "A3");
	NoteTone *c = new NoteTone(0, 4, kSine, 329.63, "E4");
	
	//PercussionTone *b = new PercussionTone(1, "kick.wav");
	
	Toolkit tk = Toolkit(125000);
	
	tk.startStream();
	
	tk.playNoteTone(a);
	
	Stk::sleep(1000);
	tk.playNoteTone(c);
	Stk::sleep(1000);
	tk.playNoteTone(c);
	Stk::sleep(1000);
	tk.playNoteTone(c);
	Stk::sleep(2000);
	
	tk.stopStream();
	
	
	
	// tk.playTone(0, 2, "sine", 440.0);
	//tk.playNoteTone(a);
	//tk.playPercussionTone(b);
	
	return 0;
}