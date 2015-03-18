#ifndef __TEMPO_H_INCLUDED__
#define __TEMPO_H_INCLUDED__

#include <thread>

class Tempo{
private:
	unsigned int bpm;
	bool isRunning;
	int run();

public:
	Tempo(int beatsPerMinute);
	void start();
	void stop();
	std::thread *runThread;
};

#endif 