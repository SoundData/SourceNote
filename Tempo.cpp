#include "Tempo.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

Tempo::Tempo(int beatsPerMinute) : bpm(beatsPerMinute) {};

using namespace std;

void Tempo::start(){
	isRunning = true;

	runThread = new thread(&Tempo::run, this);

}

void Tempo::stop(){
	isRunning = false;
}


// Removed the (void *) parameter - when we start this thread, the standard lib takes care of setting up the context
// when we make the call in start()
int Tempo::run(){

	

	int beatSubInterval = 4;
	/* 'beatSubInterval' defines how many times we fetch new tones between every quarter beat.
	* If its equal to 4, we fetch every 16th note in a measure. 1 would be only once per beat. 2 would be twice per beat
	* which would be 8th notes */

	/* Translate BPM to microseconds between subBeats */
	double sampleInterval = bpm / 60;
	/* sampleInterval = beats per second */
	sampleInterval = 1 / sampleInterval;
	/* sampleInterval = time length in seconds between "samples" (or fetches) */
	sampleInterval *= 1000000;
	/* sampleInterval = time length in microseconds between beats (quarter notes) */
	sampleInterval /= beatSubInterval;
	/* sampleInterval = time length in microseconds between subBeats */
	int sampleLength = (int)sampleInterval;

	int beatCount = 1;
	/* Similar to written music, beat 1 starts at time 0 and there is no beat 0. */
	int beatSubIntervalCount = 4;

	/******** Testing variables ********/

	//double excess = 0;
	///* 'excess' accumulates the extra time used by 'sleep_for' that exceeds the 'sampleLength' time */
	//int sleepCount = 0;
	///* 'sleepCount' counts all sleeps to find the average sleep time variation */
	//double sleepTimeAccumulator = 0;
	///* sum of all sleep times */
	//std::cout << "\n\n\n******  Time is measured in milliseconds  ****** \n";
	//std::cout << "\n******  Expected sleep length: " << sampleLength / 1000 << "  ******\n\n\n";

	/************************************/

	auto sampleLengthMicros = chrono::microseconds(sampleLength);

	auto beatZero = chrono::high_resolution_clock::now();
	auto nextBeat = beatZero + 1 * sampleLengthMicros;

	auto start = chrono::high_resolution_clock::now();
	auto end = start; 

	int count = 1;

	double ACCEPTABLE_BADNESS = 0.5; // ms

	while (isRunning){
		/* fetch tones */
		

		if (abs(chrono::duration<double, milli>(chrono::high_resolution_clock::now() - nextBeat).count()) < ACCEPTABLE_BADNESS
			|| chrono::high_resolution_clock::now() > nextBeat)
		{
			cout << "Beat " << count++ <<  endl;
			if (count == 10)
				start = chrono::high_resolution_clock::now();

			else if (count == 50)
			{
				end = chrono::high_resolution_clock::now();
				break;
			}

			nextBeat = beatZero + count * sampleLengthMicros;
		}

		else
		{
			this_thread::yield();
			continue;
		}
		

	}

	cout << chrono::duration<double, micro>(end - start).count() << " us between beats 10 and 50 (expected 5.00000e+006)" << endl;


	int x;
	cin >> x;

	return 0;
}


int main(){
	Tempo tempo = Tempo(120);
	tempo.start();

	tempo.runThread->join();

	return 0;
}

