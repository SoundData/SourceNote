#include "SNCore.h"

void SNCore::start(GameMessage *classMessage){
	Tempo temp = Tempo(getBPMForClass(classMessage),mutex);
	/* Why can't I just make the object pointer in one line?! */
	tempo = &temp;
	decodeMessage(classMessage);
}

void SNCore::decodeMessage(GameMessage* message){
	if (message->eventType == "player_death"){
		NoteTrack minorMainMelody = toneCreator.makeMainMelodyScaleInNewScale(kMajor, kMinor);
		minorMainMelody.continous = false;
		minorMainMelody.repeatCount = 4;
		tempo->addMainMelodyTrack(minorMainMelody);

		NoteTrack lowOctaveTrack = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(false, 1);
		lowOctaveTrack.continous = false;
		lowOctaveTrack.repeatCount = 6;
		tempo->addNoteTrack(lowOctaveTrack);

		PercussionTrack randomPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kLtom);
		randomPercussionTrack.continous = false;
		randomPercussionTrack.repeatCount = 3;
		tempo->addPercussionTrack(randomPercussionTrack, false);

	}else if(message->eventType == "player_changeclass"){
		/* stop Tempo to change the BPM */
		tempo->stop();
		if(message->info->at("class") == "TFClass_Scout"){
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(true, 2);
			newMainMelody.continous = true;
			tempo->addMainMelodyTrack(newMainMelody);

			PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kSnare);
			tempo->addPercussionTrack(newPercussionTrack, true);

		}else if(message->info->at("class") == "TFClass_Heavy"){
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(true, 1);
			newMainMelody.continous = true;
			tempo->addMainMelodyTrack(newMainMelody);

			PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
			tempo->addPercussionTrack(newPercussionTrack, true);

		}else{
			NoteTrack newMainMelody = toneCreator.makeRandomMelodyNotesInRandomKeyWithOctave(true, 1);
			newMainMelody.continous = true;
			tempo->addMainMelodyTrack(newMainMelody);

			PercussionTrack newPercussionTrack = toneCreator.makeRandomBeatWithPercussionType(kKick);
			tempo->addPercussionTrack(newPercussionTrack, true);
		}
		tempo->setNewBPM(getBPMForClass(message));
		tempo->start();
	}
}

int SNCore::getBPMForClass(GameMessage *classMessage){
	if(classMessage->eventType != "player_changeclass"){
		return -1;
	}
	if(classMessage->info->at("class") == "TFClass_Scout"){
		return 160;
	}else if(classMessage->info->at("class") == "TFClass_Heavy"){
		return 100;
	}else
		return 120;
}