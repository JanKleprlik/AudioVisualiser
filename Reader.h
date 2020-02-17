#pragma once
#include <fstream>
#include <SFML/Audio.hpp>

class Reader
{
public:
	Reader() {}
private:
	//void read_metadata();  //Wont need

	//std::ifstream input;		//Wont need
	const int bufferSize = 16384;
	
	
	sf::SoundBuffer soundBuffer_;
	
};

