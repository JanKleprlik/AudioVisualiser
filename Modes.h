#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include <valarray>

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
	
	int sample_rate = 0;
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
	
private:
	std::vector<float> window;
	ComplAr samples;
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