#include <fstream>
#include <iostream>
#include "../Headerss/Menu.h"
#include "../Headerss/Vizualizer.h"

#define BUFFER_SIZE 16384

#define BIG_BUFFER

using namespace std;
using namespace sf;

#pragma region Abstract Mode
/**
* Initializes song to be played on loop.
* All Modes must be derived from this class.
* Input: song name string
*/
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
/**
 * Initializes drawables.
 */
Amplitude::Amplitude(const std::string& song_name) :AbstractMode(song_name)
{
	VA.resize(buffer_size);
	VA.setPrimitiveType(LineStrip);
}
/**
 *  Updates heights of all vertices by current song buffer values.
 */
void Amplitude::update()
{
	const int offset = static_cast<int>(song.getPlayingOffset().asSeconds() * sample_rate);
	if (offset + buffer_size < sample_count)
	{
		const int color_jump = static_cast<int>(buffer_size / 256);
		for (int i = 0; i < buffer_size; i++)
		{
			VA[i] = Vertex(Vector2f(0.f,	//x
				static_cast<float>(HEIGHT / 2)) + Vector2f(i / static_cast<float>(buffer_size) * WIDTH, buffer.getSamples()[i + offset] * static_cast<float>(0.008)),	//y
				Color(255 - i / color_jump, 0, i / color_jump));
		}
	}

}
/**
 * Draws amplitude to the given window.
 */
void Amplitude::draw(RenderWindow& window)
{
	window.draw(VA);
}
#pragma endregion 

#pragma region WithFFT
/**
 * Initializes drawables.
 */
WithFFT::WithFFT(const std::string& song_name) :AbstractMode(song_name)
{
	samples.resize(buffer_size);

	create_hamming_window();
}
/**
 * Computes FFT on current song buffer values.
 */
void WithFFT::update()
{
	int offset = static_cast<int>(song.getPlayingOffset().asSeconds() * sample_rate);
	if(offset + buffer_size < sample_count)
	{
		for (int i = 0; i < buffer_size; i++)
		{
			samples[i] = Complex(static_cast<int>(buffer.getSamples()[i + offset] * window[i]), 0);
		}
	}

	bin = ComplAr(samples.data(), buffer_size);
	fft(bin);
}
/**
 *  Implements Fast Fourier Transform algorithm.
 */
void WithFFT::fft(ComplAr& data)
{
	const size_t n = data.size();
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
/**
 * Generates hamming window.
 */
void WithFFT::create_hamming_window()
{
	for (int i = 0; i < buffer_size; i++) {
		window.push_back(static_cast<float>(0.54) - static_cast<float>(0.46) * cos((2 * PI * i) / static_cast<float>(buffer_size)));
	}
}
/**
 * Generates LINE-like structure pointing up, with bass on the left and heights on the right side. 
 * Heights are stored into VertexArray VA.
 * Starting position of the structure is set by argument Vector2f starting_position.
 */
void WithFFT::generate_line_lr_up(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	int stop_position = static_cast<int>(log(buffer_size / (2 * 3)) / log(1.01));
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisi frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size/ 2.f; i *= static_cast<float>(1.01))
	{
		const Vector2f position(static_cast<float>(x_start), static_cast<float>(abs(bin[static_cast<int>(i)])));
		VA.append(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color::White));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}
/**
 * Generates LINE-like structure pointing down, with bass on the left and heights on the right side.
 * Heights are stored into VertexArray VA.
 * Starting position of the structure is set by argument Vector2f starting_position.
 */
void WithFFT::generate_line_lr_down(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	int stop_position = static_cast<int>(log(buffer_size / (2 * 3)) / log(1.01));
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisi frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= static_cast<float>(1.01))
	{
		const Vector2f position(static_cast<float>(x_start), static_cast<float>(abs(bin[static_cast<int>(i)])));
		VA.append(Vertex(starting_position + Vector2f(position.x, position.y / 100000000 * 500), Color::White));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}
/**
 * Generates BAR-like structure pointing down, with bass on the left and heights on the right side.
 * Heights are stored into VertexArray VA.
 * Starting position of the structure is set by argument Vector2f starting_position.
 */
void WithFFT::generate_bars_lr_down(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	int stop_position = static_cast<int>(log(buffer_size / (2 * 3)) / log(1.01));
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisi frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= static_cast<float>(1.01))
	{
		const Vector2f position(static_cast<float>(x_start), static_cast<float>(abs(bin[static_cast<int>(i)])));
		VA.append(Vertex(starting_position + Vector2f(position.x, position.y / 100000000 * 500), Color(209, 209, 209)));
		VA.append(Vertex(starting_position + Vector2f(position.x, 0), Color(254, 254, 254, 60)));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}
/**
 * Generates BAR-like structure pointing up, with bass on the left and heights on the right side.
 * Heights are stored into VertexArray VA.
 * Starting position of the structure is set by argument Vector2f starting_position.
 */
void WithFFT::generate_bars_lr_up(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	int stop_position = static_cast<int>(log(buffer_size / (2 * 3)) / log(1.01));
	//buffer_size is divided by 2 for working only on first half of spectrum (second half is the same)
	//, and by three to avoid first few noisy frequencies
	int x_start = (WIDTH - stop_position) / 2;
	for (float i = 3.f; i < buffer_size / 2; i *= static_cast<float>(1.01))
	{
		const Vector2f position(static_cast<float>(x_start), static_cast<float>(abs(bin[static_cast<int>(i)])));
		VA.append(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color::White));
		VA.append(Vertex(starting_position + Vector2f(position.x, 0)));
		//position.y is divided by such a large number to scale it into reasonable interval
		//multiplying sets the maximum height in pixels
		x_start++;
	}
}
/**
 * Generates raw frequency spectrum with Bass on the left, heights on the right.
 * Heights are stored into VertexArray VA.
 * Starting position of the structure is set by argument Vector2f starting_position.
 */
void WithFFT::frequency_spectrum_lr(sf::VertexArray& VA, const sf::Vector2f& starting_position, const int& length, const int& height)
{
	for (int i = 0; i < length; i++)
	{
		VA.append(Vertex(starting_position +	//base
			Vector2f(static_cast<float>(i), //x
			static_cast<float>(-abs(bin[static_cast<int>(i)]) / 100000000 * height)), //y
			Color(217, 98, 80,128)));
	}
}

/**
 * Generates logarithmically scaled frequency halo forming a circle. 
 * Each layer is stored at one VertexArray in the vector VAs.
 * Parameters:
 * Color of each layer is in vector colors.
 * Height of each layer is in vector heights.
 * Center is the center of the halo.
 * Radius is the base radius the halo will reach when no music is playing.
 * From is a frequency from which the halo should start generating.
 * To is a frequency to which the halo should be generated.
 */
void WithFFT::frequency_spectrum_round(std::vector<sf::VertexArray>& VAs,std::vector<sf::Color>& colors,const std::vector<float>& heights, const sf::Vector2f& center, const float& radius, const int& from, const int& to)
{
	if (VAs.size() != colors.size() || VAs.size() != heights.size())
	{
		throw new invalid_argument("number of VertexArrays is not the same as number of colors");
	}
	if(from == 0)
	{
		throw new invalid_argument("spectrum cant start at 0, set different 'from' value");
	}
	
	const float scale = float(to-from) / 180.f;		//divided by angle (in degrees) that the scale should cover (360 - circle; 180 - half a circle)
	
	for (int i = 0; i < VAs.size(); i++)
	{
		VAs[i].append(Vertex(center, colors[i]));
	}


	/**/
	for(int j = static_cast<int>(VAs.size()-1); j >= 0; j--)
	{
		//generating left side of the halo

		float new_radius;
		float angle;
		Vector2f cartez;
		Vector2f base_position;
		for (float i = static_cast<float>(from); i< static_cast<float>(to); i *= static_cast<float>(1.01))
		{
			//generating left-side
			new_radius = radius * static_cast<float>(1 + (abs(bin[static_cast<int>(i)]) / 30000000000 * heights[j]));
			angle= ((i-from)/ scale + 90 ) * PI / 180;
			
			cartez = Vector2f(new_radius * cos(angle), new_radius * sin(angle)); //polar coordinates to cartez
			base_position = center + cartez;
			VAs[j].append(Vertex(base_position, colors[j]));
		}		
		//last vertex for completing full semicircle
		angle = (270 * PI / 180);
		cartez = Vector2f(new_radius * cos(angle), new_radius * sin(angle));
		VAs[j].append(Vertex(center + cartez));
		
		//generating right side of the halo
		for (float i = static_cast<float>(from); i< static_cast<float>(to); i *= static_cast<float>(1.01))
		{
			new_radius = radius * static_cast<float>(1 + (abs(bin[static_cast<int>(i)]) / 30000000000 * heights[j]));
			angle = (-(i - from) / scale + 90) * PI / 180;
			cartez = Vector2f(new_radius * cos(angle), new_radius * sin(angle));
			base_position = center + cartez;
			VAs[j].append(Vertex(base_position, colors[j]));
		}
		//last vertex for completing full semicircle
		angle = -(90 * PI / 180);
		cartez = Vector2f(new_radius * cos(angle), new_radius * sin(angle));
		VAs[j].append(Vertex(center + cartez));
	}
}
/**
 * Generates map-like structure based on current song buffer values.
 * Map structure is stored at VertexArray VA.
 * Starting position is at bottom left corner of the map.
 */
void WithFFT::generate_map(sf::VertexArray& VA, const sf::Vector2f& starting_position)
{
	const auto move_vec = Vector2f(static_cast<float>(-0.9), static_cast<float>(1.1));
	const int stop_position = static_cast<int>(log(buffer_size / (2 * 3)) / log(1.01));
	//buffer_size is divided by 2 for working only on first half of spectrum (second is the same)
	//, and by three to avoid first few noisy frequencies
	int x_start = (WIDTH - stop_position) / 2;
	Vector2f position(static_cast<float>(x_start),0.f);

	
	//Move previously generated points
	for (int i = 0; i < map.size(); i++)
	{
		map[i].position -= move_vec;
	}

	//Generate new map
	map.push_back(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color(254, 254, 254, 20))); //helps with balancing height
	for (float i = 3.f; i < (buffer_size/2)-80; i*= static_cast<float>(1.01))
	{
		position = Vector2f(static_cast<float>(x_start), static_cast<float>(abs(bin[static_cast<int>(i)])));
		map.push_back(Vertex(starting_position + Vector2f(position.x, -position.y / 100000000 * 500), Color(254, 254, 254, 20)));
		x_start++;
	}
	map.push_back(Vertex(starting_position + Vector2f(position.x, -0 / 100000000 * 500), Color(254, 254, 254, 20))); //helps with balancing height
	
	VA.clear();
	for (int i = static_cast<int>(max((double)0,map.size() - 3e5)); i < map.size(); i++)
	{
		VA.append(map[i]);	}
	
}


#pragma endregion 

#pragma region Radio
/**
 *  Initializes drawables.
 */
Radio::Radio(const std::string& song_name): WithFFT(song_name)
{
	VA_up.setPrimitiveType(LineStrip);
	VA_down.setPrimitiveType(LinesStrip);
}
/**
 *  Draws Radio to given window.
 */
void Radio::draw(sf::RenderWindow& window)
{
	window.draw(VA_up);
	window.draw(VA_down);
}
/**
 *  Updates bar heights.
 */
void Radio::update()
{
	WithFFT::update();
	VA_up.clear();
	VA_down.clear();
	
	const Vector2f starting_position(0.f, HEIGHT/2.f);
	generate_bars_lr_up(VA_up,starting_position);
	generate_bars_lr_down(VA_down, starting_position);
}


#pragma endregion 

#pragma region Map
/**
 *  Initializes drawables.
 */
Map::Map(const std::string& song_name) :WithFFT(song_name)
{
	VA.setPrimitiveType(LineStrip);
}
/**
 *  Draws Map to the given window.
 */
void Map::draw(sf::RenderWindow& window)
{
	window.draw(VA);
}
/**
 *  Updates map position.
 */
void Map::update()
{
	WithFFT::update();
	VA.clear();

	const Vector2f starting_position(0.f, 700.f);

	generate_map(VA, starting_position);
}



#pragma endregion 

#pragma region Space
/**
 *  Initializes drawables.
 */
Space::Space(const std::string& song_name) : WithFFT(song_name)
{
	stars.resize(num_of_stars);
	for(int i = 0; i < num_of_stars; i++)
	{
		stars[i] = (Star {1.f * (rand() % WIDTH),1.f* (rand() % HEIGHT), 0 });
	}
	
	//initialization of planet
	planet_texture.loadFromFile("Resources/mars.png");
	planet_texture.setSmooth(true);
	planet.setRadius(base_planet_radius);
	planet.setPointCount(40);
	planet.setFillColor(Color(255, 255, 255, 128));
	planet.setTexture(&planet_texture);
	planet.setOrigin(base_planet_radius, base_planet_radius);
	planet.setPosition(Vector2f((WIDTH / 2.f), (HEIGHT / 2.f)));

	planet_background.setRadius(base_planet_radius);
	planet_background.setPointCount(40);
	planet_background.setFillColor(Color::Black);
	planet_background.setOrigin(base_planet_radius, base_planet_radius);
	planet_background.setPosition(Vector2f((WIDTH / 2.f), (HEIGHT / 2.f)));


	//initialization of stars
	star_vertices.setPrimitiveType(Lines);
	star_vertices.resize(2*num_of_stars);

	//inicialization of halo effect
	for (int i = 0; i <4; i++)
	{
		halo.push_back(VertexArray(TriangleFan));

	}
	const float base_height = 300;
	halo_heights.push_back(base_height * 3);
	halo_heights.push_back(base_height * 2.6);
	halo_heights.push_back(base_height * 2);
	halo_heights.push_back(base_height);

	halo_colors.push_back(Color::Blue);		
	halo_colors.push_back(Color::Red);
	halo_colors.push_back(Color::Yellow);
	halo_colors.push_back(Color::White);
}
/**
 *  Draws Space to the given window.
 */
void Space::draw(sf::RenderWindow& window)
{
	window.draw(star_vertices);

	for (auto&& layer :halo)
	{
		window.draw(layer);
	}
	
	window.draw(planet_background);
	window.draw(planet);

}
/**
 *  Helper funciton. Gets scaled approximate of given range of frequencies.
 */
float Space::update_by_sound(const int from, const int to, const int scale_factor)
{
	float sum = 0;
	for (int i = from; i < to; ++i)
	{
		sum += static_cast<float>(abs(bin[i]));
	}
	return (sum / (to - from)) / scale_factor;
}
/**
 * Helper function. Updates radius of given circle shape.
 */
void Space::update_radius(const float update_value,sf::CircleShape& circle) const
{
	circle.setRadius(base_planet_radius * update_value);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(Vector2f((WIDTH / 2.f), (HEIGHT / 2.f)));
}

/**
 *  Updates Space state by current song buffer values.
 */
void Space::update()
{
	WithFFT::update();

	//update base_star_speed accoring to sub_bass frequencies
	base_star_speed = static_cast<float>(1.01) + update_by_sound(0, 60,100000000);
	
	//planet radius is mapped to human voice frequencies
	float radius_update = 1.f + update_by_sound(90, 280, 5000000);

	update_radius(radius_update, planet_background);
	update_radius(radius_update, planet);

	//rotation is based on higher pitch frequencies - the flow of the sceen feels more natural imo
	planet.rotate(static_cast<float>(0.5) + update_by_sound(500, 3000,1000000 ));

	//stars generation
	star_vertices.clear();
	for(auto& star: stars)
	{
		star.z += 1;


		star.old_x = star.x;
		star.old_y = star.y;
		star.x = (star.x - WIDTH / 2) * base_star_speed + WIDTH / 2;		
		star.y = (star.y - HEIGHT / 2) * base_star_speed + HEIGHT / 2;		

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
		star_vertices.append(Vertex(Vector2f(star.x, star.y),Color(static_cast<sf::Uint8>(star.z), static_cast<sf::Uint8>(star.z), static_cast<sf::Uint8>(star.z),255)));
		//adding tail of the star
		star_vertices.append(Vertex(Vector2f(star.old_x, star.old_y), Color(static_cast<sf::Uint8>(star.z), static_cast<sf::Uint8>(star.z), static_cast<sf::Uint8>(star.z), 255)));;
	}
	//generating halo around planet
	for (size_t i = 0; i < halo.size(); i++)
	{
		halo[i].clear();
	}
	
	frequency_spectrum_round(halo, halo_colors, halo_heights, Vector2f(WIDTH / 2.f, HEIGHT / 2.f), planet.getRadius() - 2, 20, 80);
}
#pragma endregion 