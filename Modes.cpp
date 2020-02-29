#include "Modes.h"
#include <fstream>
#include <iostream>

#define WIDTH 1024
#define HEIGHT 768
#define BUFFER_SIZE 16384

#define BIG_BUFFER

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
	samples.resize(buffer_size);

	create_hamming_window();
}


void WithFFT::fft(ComplAr& data)
{
	const int n = data.size();
	if (n <= 1) return;

	ComplAr odd = data[slice(1, n / 2, 2)];
	ComplAr even = data[slice(0, n / 2, 2)];

	fft(odd);
	fft(even);

	for (int i = 0; i < n / 2; i++) {

		Complex omega = polar<double>(1.0, -2 * PI * i / n);
		//multiplication could be optimized by counting the mult only once;
		data[i] = even[i] + omega * odd[i];
		data[i+n/2] = even[i] - omega * odd[i];
	}
}

void WithFFT::create_hamming_window()
{
	for (int i = 0; i < buffer_size; i++) {
		window.push_back(0.54 - 0.46 * cos((2 * PI * i) / (float)buffer_size));
	}
}
#pragma endregion 


#pragma region Radio

Radio::Radio(const std::string& song_name): WithFFT(song_name)
{
	VA_up.setPrimitiveType(LineStrip);
	VA_down.setPrimitiveType(Lines);

	//VA_up.resize(buffer_size);
	//VA_down.resize(buffer_size);
}

void Radio::draw(sf::RenderWindow& window)
{
	window.draw(VA_up);
	window.draw(VA_down);
}

void Radio::update()
{
	int offset = song.getPlayingOffset().asSeconds() * sample_rate;
	for (int i = 0; i < buffer_size; i++)
	{
		samples[i] = Complex(buffer.getSamples()[i + offset] * window[i],0);
	}
	
	VA_up.clear();
	VA_down.clear();
	
	bin = ComplAr(samples.data(), buffer_size);
	fft(bin);
	
	const Vector2f starting_position(0.f, 384.f);
	generate_bars_lr(VA_up,starting_position,1);
	/*/

	for (float i = 3.f; i < min((float)buffer_size/2.f, 20000.f); i *= 1.01)
	{
		Vector2f position(log(i) / log(min((float)buffer_size/2.f, 20000.f)), abs(bin[(int)i]));
		VA_up.append(Vertex(starting_position + Vector2f(position.x * 800, -position.y / 100000000 * 500 ), Color::White));
		//VA_up.append(Vertex(starting_position + Vector2f(position.x * 800,0), Color::White));

		VA_down.append(Vertex(starting_position + Vector2f(position.x * 800, 0),Color(254,254,254,100)));
		VA_down.append(Vertex(starting_position + Vector2f(position.x * 800, position.y / 100000000 * 500), Color::White));
	}
	/**/
}
 void WithFFT::generate_bars_lr(sf::VertexArray& VA, const sf::Vector2f& starting_position, int size)
 {
	 int x_position = 50;
	 for (float i = 3.f; i < buffer_size/2; i *= 1.01)
	 {
		 Vector2f position(x_position, abs(bin[(int)i]));
		 VA.append(Vertex(starting_position + Vector2f(position.x , -position.y / 100000000 * 500 ), Color::White));
		// VA.append(Vertex(starting_position + Vector2f(position.x, 0), Color::White));

		 //VA.append(Vertex(starting_position + Vector2f(position.x * 800, 0), Color(254, 254, 254, 100)));
		 //VA.append(Vertex(starting_position + Vector2f(position.x * 800, position.y / 100000000 * 500), Color::White));
		 x_position++;
	 }
 }


#pragma endregion 