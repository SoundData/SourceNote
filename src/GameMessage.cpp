#include "GameMessage.h"

#include <vector>
#include <sstream>
#include <iostream>

const char* GameMessage::DELIM = "##";

std::vector<std::string> split(const std::string&, const std::string&);

GameMessage::GameMessage(const char *buf, size_t msgLen) : info(NULL){
	std::string temp(buf, msgLen);
	PopulateFields(temp);
}

GameMessage::~GameMessage(){
	if (info != NULL)
	{
		delete info;
	}
}

void GameMessage::PopulateFields(std::string temp){
	auto toks = split(temp, DELIM);
	auto toksSz = toks.size();

	if (toksSz == 0){
		std::cout << "ERROR: No tokens parsed from [" << temp <<  "]" << std::endl;
	}
	
	if (!(toks.size() % 2 == 1)){
		std::cout << "ERROR: even number of tokens from [" << temp << "]" << std::endl;
		return;
	}

	eventType = toks[0];
	if (toks.size() >= 2)
		info = new std::map<std::string, std::string>();

	for (std::string::size_type i = 1; i < toks.size(); i += 2){
		info->insert(std::pair<std::string, std::string>(toks[i], toks[i+1]));
	}
}

std::vector<std::string> split(const std::string& str, const std::string& delim){
	std::vector<std::string> tokens;
	auto delimSz = delim.size();
	// loop invariant:
	// pos2 holds the end of the previous token, pos1 holds the end of the current token
	int pos1 = 0, pos2 = 0;
	while ((pos1 = str.find(delim, pos2)) != std::string::npos){
		// Copy pos1-pos2 bytes, starting at pos2
		std::string substr = str.substr(pos2, pos1 - pos2);
		// std::cout << "Pushing back \"" << substr << "\" to tokens" << std::endl;
		tokens.push_back(substr);
		pos2 = pos1 + delimSz;
	}

	std::string substr = str.substr(pos2, str.length());
	// std::cout << "Pushing back \"" << substr << "\" to tokens" << std::endl;
	tokens.push_back(substr);

	return tokens;
}