// Copyright (c) 2020 Jan Kleprlík <kleprlikjan@gmail.com>
// Licensed under GPL v3 or later

#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include <valarray>
#include <random>

#include "Menu.h"

const float PI = static_cast<float>(3.14159265358979323846);


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
	sf::Uint64 sample_count = 0;
	sf::Uint64 buffer_size = 0;

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
	void frequency_spectrum_round(std::vector<sf::VertexArray>& VAs, std::vector<sf::Color>& colors,const std::vector<float>& heights, const sf::Vector2f& center, const float& radius, const int& from, const int& to);

	void generate_map(sf::VertexArray& VA, const sf::Vector2f& starting_position);

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
	/**
	* Array of vertices that are drawn pointing upwards.
	*/
	sf::VertexArray VA_up;
	/**
	* Array of vertices that are drawn pointing downwards.
	*/
	sf::VertexArray VA_down;
};

class Map : public WithFFT
{
public:
	Map(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	/**
	* Array of vertices that is drawn.
	*/
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
	const float base_planet_radius = 150;

	float base_star_speed = static_cast<float>(1.01);

	float update_by_sound(const int from, const int to, const int scale_factor);
	void update_radius(const float update_value, sf::CircleShape& circle) const;

	struct Star
	{
		/**
		* x coordinate of the star
		*/
		float x = 0.f;
		/**
		* y coordinate of the star
		*/
		float y = 0.f;
		/**
		* z coordinate of the star
		*/
		float z = 0.f;
		/**
		* previous x coordinate of the star
		*/
		float old_x = 0.f;
		/**
		* previous y coordinate of the star
		*/
		float old_y = 0.f;
	};

	/**
	* Vector of stars that are used to compute positions.
	*/
	std::vector<Star> stars;
	/**
	* Array of vertices representing stars. This is drawn.
	*/
	sf::VertexArray star_vertices;

	/**
	* Vertices used to draw halo areound the planet.
	*/
	std::vector<sf::VertexArray> halo;
	/**
	* Vector of halo colors. First color is in the background. Last is at the front.
	*/
	std::vector<sf::Color> halo_colors;
	/**
	* Vector of hegihts of each halo color layer.
	*/
	std::vector<float> halo_heights;
	
	/**
	* Planet shape.
	*/
	sf::CircleShape planet;
	/**
	* Planet background shape.
	*/
	sf::CircleShape planet_background;
	/**
	* Planet texture.
	*/
	sf::Texture planet_texture;

};