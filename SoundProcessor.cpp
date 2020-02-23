#include "SoundProcessor.h"
#include <iostream>
using namespace std;
using namespace sf;
/*
Initializes data needed for vizualizin given song
*/
void SoundProcessor::initialize(const std::string song_name) {
	songName = song_name;
	//check for existence of a song
	ifstream f(songPath + songName);
	if (!f.good()) {
		//Wrong song handler
		cout << "Song \"" << songName << "\" is not in the database." << endl;
		return;
	}


	soundBuffer.loadFromFile(songPath + songName);
	song.setBuffer(soundBuffer);


	sampleCount = soundBuffer.getSampleCount();
	sampleRate = soundBuffer.getSampleRate() * soundBuffer.getChannelCount();

	//bufferSize = sampleCount;
	generate_hamming_window();

	//samples.reserve(bufferSize);
	samples.resize(bufferSize);
	samples_hamming.resize(bufferSize);

	//do this somere else, to all chosen vizualization effects
	//VA1.resize(bufferSize);
	VA1.resize(800);
	VA1.setPrimitiveType(LineStrip);
	//VA2.resize(bufferSize);
	VA2.resize(800);
	VA2.setPrimitiveType(LineStrip);
	song.setLoop(true);
	song.play();
}

void SoundProcessor::generate_hamming_window() {
	for (int i = 0; i < 800; i++) {
		window_hamming.push_back(0.54 - 0.46 * cos((2 * PI * i) / (float)800));   //800 change for bufferSize
	}
}

void SoundProcessor::set_samples() {
	int offset = song.getPlayingOffset().asSeconds() * sampleRate;
	for (int i = 0; i < bufferSize; i++) {
		samples[i] = complex<double>(soundBuffer.getSamples()[i + offset]);
		//  /(float)bufferSize * 900
		//delete this later - only testing purposes
		VA1[i] = Vertex(Vector2f(0, 550) + Vector2f(i / (float)bufferSize * 700, samples[i].real() * 0.005), Color::Color(57, 255, 20));
	}
}
void SoundProcessor::set_samples_hamming() {
	int offset = song.getPlayingOffset().asSeconds() * sampleRate;
	for (int i = 0; i < bufferSize; i++) {
		samples_hamming[i] = complex<double>(soundBuffer.getSamples()[i + offset] * window_hamming[i], 0);
		VA2[i] = Vertex(Vector2f(0, 250) + Vector2f(i / (float)bufferSize * 700, samples_hamming[i].real() * 0.005), Color::Color(255, 0, 0));
	}
}
//------------------------TRIMMED------------------------
void SoundProcessor::set_samples_trimmed() {
	int offset = song.getPlayingOffset().asSeconds() * sampleRate;
	for (int i = 0; i < 800; i++) {
		samples[i] = complex<double>(soundBuffer.getSamples()[i + offset]);
		//  /(float)bufferSize * 900
		//delete this later - only testing purposes
		VA1[i] = Vertex(Vector2f(0, 550) + Vector2f(i, samples[i].real() * 0.005), Color::Color(255 - i , 0, i ));
	}
}
void SoundProcessor::set_samples_hamming_trimmed() {
	int offset = song.getPlayingOffset().asSeconds() * sampleRate;
	for (int i = 0; i < 800; i++) {
		samples_hamming[i] = complex<double>(soundBuffer.getSamples()[i + offset] * window_hamming[i], 0);
		VA2[i] = Vertex(Vector2f(0, 250) + Vector2f(i, samples_hamming[i].real() * 0.005), Color::Color(0, 0, 255));
	}
}


void SoundProcessor::fft(valarray<complex<double>>& data) {
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
void SoundProcessor::draw(RenderWindow& window) {

	window.draw(VA1);
	window.draw(VA2);
}
void SoundProcessor::update() {
	//set_samples();
	//set_samples_hamming();
	set_samples_trimmed();
	set_samples_hamming_trimmed();
}