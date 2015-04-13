#pragma once

//
// GameMessage.h
//
// Stores the event type (as string) and as many extra fields as needed 
// inside an std::map. As you may have guessed, we map <key, value> pairs
// of the form <info_type, info_body>
// i.e. <"PlayerName", "noob_destroyer_1234">
//

#include <string>
#include <map>

#define MAX_MESSAGE_SIZE 4096 // "4kb should be enough for anybody!"

class GameMessage {
	public:
		std::string eventType;
		std::map<std::string, std::string> *info;
		GameMessage(const char *buf, size_t len);
		~GameMessage();
		
	private:
		void PopulateFields(std::string);
		static const char *DELIM;
};