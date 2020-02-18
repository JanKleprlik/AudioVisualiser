#pragma once
#include <fstream>
#include <complex>;
#include <valarray>
#include <SFML/Audio.hpp>

typedef std::valarray<std::complex<double>> ComplexArray;

class Reader
{
public:

	bool validate_metadata();
	Reader(std::ifstream& input): _input(input) {}
	void fft(ComplexArray data);
private:


	void get_buffer();
	std::ifstream& _input;
	const int bufferSize = 16384;

};

