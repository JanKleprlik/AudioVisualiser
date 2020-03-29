#include "Modes.h"
#include "Vizualizer.h"
#include <fstream>
#include <iostream>

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
	for (float i = 3.f; i < buffer_size/ 2.f; i *= 1.01)
	{
		const Vector2f position(x_start, abs(bin[(int)i]));
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

void WithFFT::frequency_spectrum_lr(sf::VertexArray& VA, const sf::Vector2f& starting_position, const int& length, const int& height)
{
	for (int i = 0; i < length; i++)
	{
		VA.append(Vertex(starting_position + Vector2f(i, -abs(bin[(int)i]) / 100000000 * height), Color::White));
	}
}

void WithFFT::generate_map(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	const auto move_vec = Vector2f(-0.9, 1.1);
	const int stop_position = log(buffer_size / (2 * 3)) / log(1.01);
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
	
	const Vector2f starting_position(0.f, HEIGHT/2.f);
	generate_bars_lr_up(VA_up,starting_position);
	generate_bars_lr_down(VA_down, starting_position);


	//generate_line_lr_up(VA_up, starting_position);
	//generate_line_lr_down(VA_down, starting_position);
	//frequency_spectrum_lr(VA_up, starting_position,WIDTH,HEIGHT/2);
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

#pragma region Space
Space::Space(const std::string& song_name) : WithFFT(song_name)
{
	stars.resize(num_of_stars);
	for(int i = 0; i < num_of_stars; i++)
	{
		stars[i] = (Star {1.f * (rand() % WIDTH),1.f* (rand() % HEIGHT), 0 });
	}
	
	planet_texture.loadFromFile("Resources/mars.png");
	planet_texture.setSmooth(true);




	planet.setRadius(circle_radius);
	planet.setPointCount(40);
	planet.setFillColor(Color(255, 255, 255, 128));
	planet.setTexture(&planet_texture);
	planet.setOrigin(circle_radius, circle_radius);
	planet.setPosition(Vector2f((WIDTH / 2.f), (HEIGHT / 2.f)));

	planet_background.setRadius(circle_radius);
	planet_background.setPointCount(40);
	planet_background.setFillColor(Color::Black);
	planet_background.setOrigin(circle_radius, circle_radius);
	planet_background.setPosition(Vector2f((WIDTH / 2.f), (HEIGHT / 2.f)));



	VA.setPrimitiveType(Lines);
	VA.resize(2*num_of_stars);
}

void Space::draw(sf::RenderWindow& window)
{
	window.draw(VA);
	window.draw(planet_background);
	window.draw(planet);
	//window.draw(ampl);
}
float Space::update_by_sound(const int& from, const int& to, const int& scale_factor)
{
	float sum = 0;
	for (int i = from; i < to; ++i)
	{
		sum += abs(bin[i]);
	}
	return (sum / (to - from)) / scale_factor;
}

void Space::update_circle(const float& update_value, sf::CircleShape& circle)
{
	circle.setRadius(circle_radius * update_value);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(Vector2f((WIDTH / 2.f), (HEIGHT / 2.f)));
}


void Space::update()
{
	WithFFT::update();

	//update speed accoring to bass level
	speed = 1.01 + update_by_sound(0, 60,100000000);
	
	//float circ_update = 1.f + update_by_sound(60, 250, 10000000);
	float circ_update = 1.f + update_by_sound(90, 280, 10000000);

	update_circle(circ_update, planet_background);
	update_circle(circ_update, planet);

	//rotation is based on human speech frequencies
	planet.rotate(0.5 + update_by_sound(500, 3000,1000000 ));
	
	VA.clear();
	ampl.clear();
	
	for(auto& star: stars)
	{
		star.z += 1;


		star.old_x = star.x;
		star.old_y = star.y;
		star.x = (star.x - WIDTH / 2) * speed + WIDTH / 2;		
		star.y = (star.y - HEIGHT / 2) * speed + HEIGHT / 2;		

		//reset star - its out of borders
		if (star.x < 0 || star.x > WIDTH || star.y < 0 || star.y>HEIGHT)
		{
			star.x = 1.f * (rand() % WIDTH);
			star.y = 1.f * (rand() % HEIGHT);
			star.z = 100.f;

			star.old_x = star.x;
			star.old_y = star.y;
		}

		//adding head of the star
		VA.append(Vertex(Vector2f(star.x, star.y),Color(star.z,star.z,star.z,255)));
		//adding tail of the star
		VA.append(Vertex(Vector2f(star.old_x, star.old_y), Color(star.z, star.z, star.z, 255)));
	}

	//+1 is for drawing optimalization
	const auto position = Vector2f(WIDTH/2.f - planet.getRadius()+1, HEIGHT/2.f);
	
	frequency_spectrum_lr(ampl,position ,int(2.f * planet.getRadius()) , 100);
	
}




#pragma endregion 