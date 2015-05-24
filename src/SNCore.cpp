#include "SNCore.h"

bool healthIsLow;
bool allowRespawnEvent;

SNCore::SNCore(std::mutex &mtx) : tempo(Tempo(120, mtx)) {};

void SNCore::start(const GameMessage *classMessage){
	setInitialMusicForClass(classMessage);
	tempo.start();
}

void SNCore::decodeMessage(const GameMessage* message){
	if (message->eventType == "PLAYER_DEATH"){
		allowRespawnEvent = true;
		healthIsLow = false;
		NoteTrack minorMainMelody = toneCreator.makeMainMelodyScaleInNewScale(kMinor, kMajor);
		minorMainMelody.continous = false;
		minorMainMelody.repeatCount = 3;
		tempo.setMainMelodyTrack(minorMainMelody);

		NoteTrack lowOctaveTrack = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, false);
		lowOctaveTrack.continous = false;
		lowOctaveTrack.repeatCount = 3;
		tempo.addNoteTrack(lowOctaveTrack);

		PercussionTrack randomPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kLtom);
		randomPercussionTrack.continous = false;
		randomPercussionTrack.repeatCount = 3;
		tempo.addPercussionTrack(randomPercussionTrack, false);

	}else if(message->eventType == "PLAYER_CHANGECLASS"){
		// stop Tempo to change the BPM
		healthIsLow = false;
		tempo.stop();
		if(message->info->at("PlayerClass") == "Scout"){
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(2, true);
			newMainMelody.continous = true;
			tempo.setMainMelodyTrack(newMainMelody);

			PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kSnare);
			//temp.addPercussionTrack(newPercussionTrack, true);

		}else if(message->info->at("PlayerClass") == "Heavy"){
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, true);
			newMainMelody.continous = true;
			tempo.setMainMelodyTrack(newMainMelody);

			PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
			//temp.addPercussionTrack(newPercussionTrack, true);

		}else{
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, true);
			newMainMelody.continous = true;
			tempo.setMainMelodyTrack(newMainMelody);

			PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
			//temp.addPercussionTrack(newPercussionTrack, true);
		}
		tempo.setNewBPM(getBPMForClass(message));
		sleep(1); //why does this make it work??
		tempo.start();

	}else if(message->eventType == "PLAYER_HURT"){
		 std::string damage = message->info->at("VictimHealth");
		 std::string::size_type sz;
		 short int damageValue = std::stoi(damage, &sz);
		 std::cout << "Damage " << damageValue << "\n";
		if(damageValue < 50 && !healthIsLow){
			healthIsLow = true;
			NoteTrack newTrack = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(2, false);
			newTrack.continous = false;
			newTrack.repeatCount = 1;
			tempo.addNoteTrack(newTrack);
		}else{
			healthIsLow = false;
		}
	}else if(message->eventType == "PLAYER_SPAWN"){
		if (allowRespawnEvent){
			allowRespawnEvent = false;
			if(message->info->at("PlayerClass") == "Scout"){
				NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(2, true);
				newMainMelody.continous = true;
				tempo.setMainMelodyTrack(newMainMelody);

				PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kSnare);
				//temp.addPercussionTrack(newPercussionTrack, true);

			}else if(message->info->at("PlayerClass") == "Heavy"){
				NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, true);
				newMainMelody.continous = true;
				tempo.setMainMelodyTrack(newMainMelody);

				PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
				//temp.addPercussionTrack(newPercussionTrack, true);

			}else{
				NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, true);
				newMainMelody.continous = true;
				tempo.setMainMelodyTrack(newMainMelody);

				PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
				//temp.addPercussionTrack(newPercussionTrack, true);
			}
		}
	}
}

void SNCore::setInitialMusicForClass(const GameMessage *classMessage){
	tempo.setNewBPM(getBPMForClass(classMessage));
	if(classMessage->info->at("PlayerClass") == "Scout"){
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(2, true);
			newMainMelody.continous = true;
			tempo.setMainMelodyTrack(newMainMelody);

			// PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kSnare);
			// temp.addPercussionTrack(newPercussionTrack, true);

		}else if(classMessage->info->at("PlayerClass") == "Heavy"){
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, true);
			newMainMelody.continous = true;
			tempo.setMainMelodyTrack(newMainMelody);

			// PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
			// temp.addPercussionTrack(newPercussionTrack, true);

		}else{
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(1, true);
			newMainMelody.continous = true;
			tempo.setMainMelodyTrack(newMainMelody);

			// PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
			// temp.addPercussionTrack(newPercussionTrack, true);
		}
}

int SNCore::getBPMForClass(const GameMessage *classMessage){
	if(classMessage->eventType != "PLAYER_CHANGECLASS"){
		return -1;
	}
	if(classMessage->info->at("PlayerClass") == "Scout"){
		return 160;
	}else if(classMessage->info->at("PlayerClass") == "Heavy"){
		return 100;
	}else
		return 120;
}