#pragma once
#include <complex>;
#include <valarray>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


class SoundProcessor
{
public:
	//Constructor
	SoundProcessor(): sampleCount(0), sampleRate(0) {}
	void initialize(const std::string song_name);
	void draw(sf::RenderWindow& window);
	void update();
	
private:
	//drawables
	sf::VertexArray VA1;
	sf::VertexArray VA2;


	//functions
	void fft(std::valarray<std::complex<double>>& data);
	void generate_hamming_window();
	void set_samples_hamming();
	void set_samples();
	void set_samples_hamming_trimmed();
	void set_samples_trimmed();

	//Colors
	sf::Color blue = sf::Color(0, 0, 255); 


	
	//FFT information
	std::vector<float> window_hamming; //hamming window for clearing input data
	std::vector<std::complex<double>> samples_hamming;	//input data multiplied by hamming window
	std::vector<std::complex<double>> samples;	//raw amplitude input data
	std::valarray<std::complex<double>> data; //input data after fft


	//Song information
	std::string songName = "";
	sf::SoundBuffer soundBuffer;
	sf::Sound song;
	int sampleRate;
	int sampleCount;
	const int bufferSize = 16384;//  implicit value is not needed //wont play songs that have less than 16384 samples

	//Helper constants
	const std::string songPath = "Resources/Songs/";
	const float PI = 3.14159265358979323846;
};

//this is going to be abstract class and inside of SoundProcessor
class Graphics {
public:
	void draw(sf::RenderWindow& window);
private:
	std::vector<sf::VertexArray> PrimitivesArrays;
};
