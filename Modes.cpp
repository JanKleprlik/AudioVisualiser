#include "Modes.h"
#include <fstream>
#include <iostream>

#define WIDTH 1024
#define HEIGHT 768
#define BUFFER_SIZE 16384

//#define BIG_BUFFER

using namespace std;
using namespace sf;

#pragma region Abstract Mode

AbstractMode::AbstractMode(const std::string& song_name)
{
	ifstream ifs(path + song_name);
	if (!ifs.good())
	{
		//existence of song_name is checked at menu also
		cout << "Song \"" << song_name << "\" is not in the database." << endl;
		return;
	}

	buffer.loadFromFile(path + song_name);
	song.setBuffer(buffer);

	sample_count = buffer.getSampleCount();
	sample_rate = buffer.getSampleRate() * buffer.getChannelCount();
#ifdef BIG_BUFFER
	if (BUFFER_SIZE < sample_count)
	{
		buffer_size = BUFFER_SIZE;
	}
	else
	{
		buffer_size = sample_count;
	}
#endif

#ifndef BIG_BUFFER
	buffer_size = WIDTH;
#endif
	
	song.setLoop(true);
	song.play();
}


#pragma endregion 


#pragma region Amplitude
Amplitude::Amplitude(const std::string& song_name) :AbstractMode(song_name)
{
	VA.resize(buffer_size);
	VA.setPrimitiveType(LineStrip);
}

void Amplitude::update()
{
	int offset = song.getPlayingOffset().asSeconds() * sample_rate;
	int color_jump = buffer_size / 256;
	for (int i = 0; i < buffer_size; i++)
	{
		VA[i] = Vertex(Vector2f(0.f, (float)(HEIGHT/2)) + Vector2f(i/(float)buffer_size*WIDTH, buffer.getSamples()[i + offset] * 0.008), Color(255 - i/color_jump, 0, i/color_jump));
	}
}

void Amplitude::draw(RenderWindow& window)
{
	window.draw(VA);
}
#pragma endregion 


#pragma region WithFFT
WithFFT::WithFFT(const std::string& song_name) :AbstractMode(song_name)
{
	window.resize(buffer_size);
	samples.resize(buffer_size);
}


void WithFFT::fft(ComplAr& data)
{
	const int n = data.size();
	if (n <= 1) return;

	valarray<complex<double>> odd = data[slice(0, n / 2, 2)];
	valarray<complex<double>> even = data[slice(0, n / 2, 2)];

	fft(odd);
	fft(even);

	for (int i = 0; i < n / 2; i++) {

		complex<double> omega = polar<double>(1, 2 * PI * i / n);
		//multiplication could be optimized by counting the mult only once;
		data[i] = even[i] + omega * odd[i];
		data[i] = even[i] - omega * odd[i];
	}
}

void WithFFT::create_hamming_window()
{
	for (int i = 0; i < buffer_size; i++) {
		window[i] = (0.54 - 0.46 * cos((2 * PI * i) / (float)buffer_size));
	}
}
#pragma endregion 


#pragma region Radio

Radio::Radio(const std::string& song_name): WithFFT(song_name)
{
	VA_up.setPrimitiveType(Lines);
	VA_down.setPrimitiveType(Lines);

	VA_up.resize(buffer_size);
	VA_down.resize(buffer_size);
}

void Radio::draw(sf::RenderWindow& window)
{
	
}

void Radio::update()
{
	
}


#pragma endregion 