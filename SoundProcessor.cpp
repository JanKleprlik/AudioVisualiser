#include "SoundProcessor.h"
#include <iostream>
using namespace std;
using namespace sf;
/*
Initializes data needed for vizualizin given song
*/
void SoundProcessor::Initialize() {
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

	//do this somere else, to all chosen vizualization effects
	VA1.resize(900);
	VA1.setPrimitiveType(LineStrip);

	song.play();
}

void SoundProcessor::generate_hamming_window() {
	for (int i = 0; i < bufferSize; i++) {
		window_hamming.push_back(0.54 - 0.46 * cos((2 * PI * i) / (float)bufferSize - 1));
	}
}

void SoundProcessor::set_samples_hamming() {
	int offset = song.getPlayingOffset().asSeconds() * sampleRate;
	auto* buffer = soundBuffer.getSamples();
	for (int i = 0; i < bufferSize; i++) {
		samples_hamming[i] = complex<double>(buffer[i+offset] * window_hamming[i]);
	}
}

void SoundProcessor::set_samples() {
	int offset = song.getPlayingOffset().asSeconds() * sampleRate;
	//auto* buffer = soundBuffer.getSamples();
	for (int i = 0; i < 900; i++) {
		samples[i] = complex<double>(soundBuffer.getSamples()[i + offset]);
		//(float)bufferSize * 900
		//delete this later - only testing purposes
		VA1[i] = Vertex(Vector2f(0, 250) + Vector2f(i , samples[i].real() * 0.005), Color::Color(255, 0, 0));
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
}
void SoundProcessor::update() {
	set_samples();
}