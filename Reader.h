#pragma once
#include <fstream>
#include <SFML/Audio.hpp>

class Reader
{
public:
private:
	void read_metadata();

	std::ifstream input;
	const int bufferSize = 16384;
};

