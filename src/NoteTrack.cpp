#include "NoteTrack.h"

NoteTrack::NoteTrack(std::vector<NoteTone> noteToneList){
	empty = noteToneList.empty() ? true : false;
	std::unordered_map<unsigned short int, NoteTone> toneMap;
	for (int i = 0; i < noteToneList.size(); i++){
		NoteTone tone = noteToneList[i];
		toneMap[tone.getStartBeatPosition()] = tone;
	}
	allTones = toneMap;
}

bool NoteTrack::isEmpty(){
	return empty;
}

std::unordered_map<unsigned short int, NoteTone> NoteTrack::tones(){
	return allTones;
}