#include "Reader.h"

//Checks if given file is .wav format.
//Gets info from metadata
bool Reader::validate_metadata() {
	//unsigned char c;
	unsigned int RIFFint;		//'RIFF'
	unsigned int file_size;		//<data size>
	unsigned int WAVEint;		//'WAVE'
	_input >> RIFFint;
	_input >> file_size;
	_input >> WAVEint;
	if (RIFFint != 0x52494646 ||WAVEint != 0x57415645) return false;
	return true;	

}