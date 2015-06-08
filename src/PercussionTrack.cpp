#include "PercussionTrack.h"

PercussionTrack::PercussionTrack(std::vector<PercussionTone> percussionToneList){
	empty = percussionToneList.empty() ? true : false;
	std::unordered_map<unsigned short int, PercussionTone> toneMap;
	for (int i = 0; i < percussionToneList.size(); i++){
		PercussionTone tone = percussionToneList[i];
		toneMap[tone.getStartBeatPosition()] = tone;
	}
	allTones = toneMap;
}

bool PercussionTrack::isEmpty(){
	return empty;
}

std::unordered_map<unsigned short int, PercussionTone> PercussionTrack::tones(){
	return allTones;
}