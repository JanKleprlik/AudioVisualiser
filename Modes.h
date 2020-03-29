#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include <valarray>
#include <random>

#include "Menu.h"

const float PI = 3.14159265358979323846;


using Complex = std::complex<double>;
using ComplAr = std::valarray<Complex>;

class AbstractMode
{
public:
	virtual ~AbstractMode() {};
	AbstractMode(const std::string& song_name);
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void update() = 0;

	

protected:
	sf::SoundBuffer buffer;
	sf::Sound song;
	
	unsigned int sample_rate = 0;
	int sample_count = 0;
	int buffer_size = 0;

	//helper constants
	const std::string path = "Resources/Songs/";

private:
	static AbstractMode* instance;
};

class Amplitude : public AbstractMode
{
public:
	Amplitude(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	sf::VertexArray VA;
};

class WithFFT : public AbstractMode
{
public:
	WithFFT(const std::string& song_name);
	void fft(ComplAr& data);
	void create_hamming_window();
	
protected:
	void update() override;
	
	void generate_bars_lr_up(sf::VertexArray& VA,const sf::Vector2f& starting_position);
	void generate_bars_lr_down(sf::VertexArray& VA, const sf::Vector2f& starting_position);

	void generate_line_lr_up(sf::VertexArray& VA, const sf::Vector2f& starting_position);
	void generate_line_lr_down(sf::VertexArray& VA, const sf::Vector2f& starting_position);

	void frequency_spectrum_lr(sf::VertexArray& VA, const sf::Vector2f& starting_position, const int& length, const int& height);

	void generate_map(sf::VertexArray& VA, const sf::Vector2f& starting_position);

	//void generate_stripe(const int& velocity, const sf::Vector2f& position, const sf::RectangleShape& stripe);
	//void generate_stripes(const int& sub_bass,const int& bass, const int& low_midrange, const int&  midrange, const int& upper_midrange, const int& presence, const int& brillance);
	std::vector<sf::Vertex> map;
	std::vector<Complex> samples;
	ComplAr bin;
	std::vector<float> window;

};

class Radio : public WithFFT
{
public:
	Radio(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	sf::VertexArray VA_up;
	sf::VertexArray VA_down;
};

class Map : public WithFFT
{
public:
	Map(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	sf::VertexArray VA;
};

class Space :public WithFFT
{
public:
	Space(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	const int num_of_stars  = 300;
	const int circle_radius = 100;

	float speed = 1.01;
	float circ_transform = 1.f;

	float update_by_sound(const int& from, const int& to, const int& scale_factor);
	void update_circle(const float& update_value, sf::CircleShape& circle);

	
	struct Star
	{
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		float old_x = 0.f;
		float old_y = 0.f;
	};

	
	std::vector<Star> stars;
	sf::VertexArray VA;
	sf::VertexArray ampl;
	sf::CircleShape planet;
	sf::CircleShape planet_background;
	sf::Texture planet_texture;

};