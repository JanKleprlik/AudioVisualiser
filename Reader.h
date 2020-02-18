#pragma once
#include <fstream>
#include <complex>;
#include <valarray>
#include <SFML/Audio.hpp>

typedef std::valarray<std::complex<double>> ComplexArray;

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

