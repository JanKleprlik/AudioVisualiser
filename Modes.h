#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include <valarray>
#include <random>

#define WIDTH 1024
#define HEIGHT 768

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

class Stripes : public WithFFT
{
public:
	Stripes(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	std::vector<sf::RectangleShape> stripes;
	sf::VertexArray VA;
	
	std::vector<int> values = { 0,0,0,0,0,0,0 };
	std::vector<int> avgs = { 0,0,0,0,0,0,0 };

	
	int sub_bass = 0;			//20-60 Hz
	int bass = 0;				//60-250 Hz
	int low_midrange = 0;		//250-500 Hz
	int midrange = 0;			//500-2000 Hz
	int upper_midrange = 0;		//2000-4000 Hz
	int presence = 0;			//4000-6000 Hz
	int brillance = 0;			//6000-20000 Hz


	int old_sub_bass = 0;			//20-60 Hz
	int old_bass = 0;				//60-250 Hz
	int old_low_midrange = 0;		//250-500 Hz
	int old_midrange = 0;			//500-2000 Hz
	int old_upper_midrange = 0;		//2000-4000 Hz
	int old_presence = 0;			//4000-6000 Hz
	int old_brillance = 0;			//6000-20000 Hz


	int avg_sub_bass = 0;			//20-60 Hz
	int avg_bass = 0;				//60-250 Hz
	int avg_low_midrange = 0;		//250-500 Hz
	int avg_midrange = 0;			//500-2000 Hz
	int avg_upper_midrange = 0;		//2000-4000 Hz
	int avg_presence = 0;			//4000-6000 Hz
	int avg_brillance = 0;			//6000-20000 Hz

	
	void update_ranges();
	//void update_positions();
	void update_position(const int& speed, sf::RectangleShape& stripe);
	void create_stripes();


	//updates new_sum minus old_sum from frequecny low to high
	void update_freq_range(const int& low,const int& high,int& new_sum, int& avg);

	/**/
	// constants

	sf::RectangleShape stripe_sub_bass = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	sf::RectangleShape stripe_bass = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	sf::RectangleShape stripe_lower_midrange = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	sf::RectangleShape stripe_midrange = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	sf::RectangleShape stripe_upper_midrange = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	sf::RectangleShape stripe_presence = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	sf::RectangleShape stripe_brillance = sf::RectangleShape(sf::Vector2f(20.f, 100.f));
	/**/
};

class Space :public WithFFT
{
public:
	Space(const std::string& song_name);
	void draw(sf::RenderWindow& window) override;
	void update() override;
private:
	const int num_of_stars  = 500;
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
	};

	
	std::vector<Star> stars;
	sf::VertexArray VA;
	sf::VertexArray ampl;
	sf::CircleShape planet;
	sf::CircleShape planet_background;
	sf::Texture planet_texture;

};