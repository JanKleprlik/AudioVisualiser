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

void WithFFT::update()
{
	int offset = song.getPlayingOffset().asSeconds() * sample_rate;
	for (int i = 0; i < buffer_size; i++)
	{
		samples[i] = Complex(buffer.getSamples()[i + offset] * window[i], 0);
	}



	bin = ComplAr(samples.data(), buffer_size);
	fft(bin);
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

void WithFFT::generate_line_lr_up(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	int stop_position = log(buffer_size / (2 * 3)) / log(1.01);
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisi frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= 1.01)
	{
		Vector2f position(x_start, abs(bin[(int)i]));
		VA.append(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color::White));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}

void WithFFT::generate_line_lr_down(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	int stop_position = log(buffer_size / (2 * 3)) / log(1.01);
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisi frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= 1.01)
	{
		const Vector2f position(x_start, abs(bin[(int)i]));
		VA.append(Vertex(starting_position + Vector2f(position.x, position.y / 100000000 * 500), Color::White));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}

void WithFFT::generate_bars_lr_down(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	const int stop_position = log(buffer_size / (2 * 3)) / log(1.01);
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisi frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= 1.01)
	{
		const Vector2f position(x_start, abs(bin[(int)i]));
		VA.append(Vertex(starting_position + Vector2f(position.x, position.y / 100000000 * 500), Color(209, 209, 209)));
		VA.append(Vertex(starting_position + Vector2f(position.x, 0), Color(254, 254, 254, 60)));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}
void WithFFT::generate_bars_lr_up(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	const int stop_position = log(buffer_size / (2 * 3)) / log(1.01);
	//buffer_size is divided by 2 for working only on first half of spectrum (second half is the same)
	//, and by three to avoid first few noisy frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= 1.01)
	{
		const Vector2f position(x_start, abs(bin[(int)i]));
		VA.append(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color::White));
		VA.append(Vertex(starting_position + Vector2f(position.x, 0)));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}

void WithFFT::frequency_spectrum_lr(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	for (int i = 3; i < buffer_size / 2; i++)
	{
		VA.append(Vertex(starting_position + Vector2f(i / 8, -abs(bin[(int)i]) / 100000000 * 500), Color::White));
	}
}

void WithFFT::generate_map(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	auto move_vec = Vector2f(-0.7, 1.3);
	int stop_position = log(buffer_size / (2 * 3)) / log(1.01);
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisy frequencies
	int x_start = (WIDTH - stop_position) / 2;
	Vector2f position(x_start,0);

	
	//Move previously generated points
	for (int i = 0; i < map.size(); i++)
	{
		map[i].position -= move_vec;
	}

	//Generate new map
	map.push_back(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color(254, 254, 254, 20))); //helps with balancing height
	for (float i = 3.f; i < (buffer_size/2)-80; i*=1.01)
	{
		position = Vector2f(x_start, abs(bin[(int)i]));
		map.push_back(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color(254, 254, 254, 20)));
		x_start++;
	}
	map.push_back(Vertex(starting_position + Vector2f(position.x, -0 / 100000000 * 500), Color(254, 254, 254, 20))); //helps with balancing height
	
	VA.clear();
	for (int i = max((double)0,map.size() - 3e5); i < map.size(); i++)
	{
		VA.append(map[i]);	}
	
}


#pragma endregion 


#pragma region Radio

Radio::Radio(const std::string& song_name): WithFFT(song_name)
{
	VA_up.setPrimitiveType(LineStrip);
	VA_down.setPrimitiveType(LinesStrip);
}

void Radio::draw(sf::RenderWindow& window)
{
	window.draw(VA_up);
	window.draw(VA_down);
}

void Radio::update()
{
	WithFFT::update();
	VA_up.clear();
	VA_down.clear();
	
	const Vector2f starting_position(0.f, 384.f);
	//generate_bars_lr_up(VA_up,starting_position);
	//generate_bars_lr_down(VA_down, starting_position);


	generate_line_lr_up(VA_up, starting_position);
	generate_line_lr_down(VA_down, starting_position);
	//frequency_spectrum_lr(VA_up, starting_position);
}


#pragma endregion 

#pragma region Map
Map::Map(const std::string& song_name) :WithFFT(song_name)
{
	VA.setPrimitiveType(LineStrip);
}
void Map::draw(sf::RenderWindow& window)
{
	window.draw(VA);
}

void Map::update()
{
	WithFFT::update();
	VA.clear();

	const Vector2f starting_position(0.f, 700.f);

	generate_map(VA, starting_position);
}



#pragma endregion 

#pragma region Stripes
Stripes::Stripes(const std::string& song_name) : WithFFT(song_name)
{
	//initialize rectangles perhaps
}

void Stripes::draw(sf::RenderWindow& window)
{
	/**/
	for (auto&& stripe : stripes)
	{
		auto a = stripe.getPosition();
		window.draw(stripe);
	}
	/**/
	window.draw(VA);
}

void Stripes::update()
{
	WithFFT::update();

	update_ranges();

	create_stripes();
	
	update_positions();



	VA.clear();

	const Vector2f starting_position(0.f, 384.f);
	frequency_spectrum_lr(VA, starting_position);
}




void Stripes::create_stripes()
{

	Vertex a = Vertex(Vector2f(20.f, 20.f));
	a.position += Vector2f(0.f, 5.f);

	const int lim = 100000;
	const int hold = 15;
	if (avg_sub_bass/100000 > 15)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Red);
		stripe.setPosition(100.f, 668.f);
		stripes.emplace_back(stripe);


		Vertex left = Vertex(Vector2f(100.f, 668.f), Color::Red);
		Vertex right = Vertex(Vector2f(120.f, 668.f), Color::Red);

		VA_bass.append(left);
		VA_bass.append(right);		
	}
	
	/*/
	if (avg_bass/lim > hold)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Green);
		stripe.setPosition(200.f, 668.f);
		stripes.emplace_back(stripe);
	}

	if (avg_low_midrange/lim > hold)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Blue);
		stripe.setPosition(300.f, 668.f);
		stripes.emplace_back(stripe);
	}

	if (avg_midrange > lim)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Red);
		stripe.setPosition(400.f, 668.f);
		stripes.emplace_back(stripe);
	}

	if (avg_upper_midrange > lim)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Green);
		stripe.setPosition(500.f, 668.f);
		stripes.emplace_back(stripe);
	}

	if (avg_presence > lim)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Blue);
		stripe.setPosition(600.f, 668.f);
		stripes.emplace_back(stripe);
	}

	if (avg_brillance > lim)
	{
		auto stripe = RectangleShape(sf::Vector2f(20.f, 5.f));
		stripe.setFillColor(Color::Red);
		stripe.setPosition(700.f, 668.f);
		stripes.emplace_back(stripe);
	}
	/**/
}


void Stripes::update_positions()
{
	for (auto&& it = stripes.begin();it < stripes.end(); ++it )
	{
		if (it->getPosition().y < 0)
		{
			stripes.erase(it);
		}
		else
		{
			it->move(0.f, -5.f);
		}
	}	
}


void Stripes::update_ranges()
{
	//get sub_bass 
	update_freq_range(20, 60, sub_bass, old_sub_bass,avg_sub_bass);
	//get bass
	update_freq_range(61, 250, bass, old_bass,avg_bass);
	//get low_midrange
	update_freq_range(251, 500, low_midrange, old_low_midrange,avg_low_midrange);
	//get midrange
	update_freq_range(501, 2000, midrange, old_midrange,avg_midrange);
	//get upper_midrange
	update_freq_range(2001, 4000, upper_midrange, old_upper_midrange,avg_upper_midrange);
	//get presence
	update_freq_range(4001, 6000, presence, old_presence,avg_presence);
	//get brilliance
	update_freq_range(6001, bin.size(), brillance, old_brillance,avg_brillance);
	
}

void Stripes::update_freq_range(const int& low, const int& high, int& new_sum, int& old_sum, int&avg)
{
	new_sum = 0;
	for(int i = low; i<=high; i++)
	{
		new_sum += abs(bin[i]);
	}
	//new_sum -= old_sum;
	//old_sum = new_sum;
	avg = (new_sum)/ (high - low);
}


#pragma endregion