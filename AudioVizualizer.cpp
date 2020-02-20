#include "Vizualizer.h"
#include <iostream>

int main ()
{
	Vizualizer vizualizer;

	vizualizer.run();

	std::cout << "vizualizer ran" << std::endl;
	
	return 0;
}
/*/
int main()
{
	VideoMode desktop = VideoMode::getDesktopMode();
	RenderWindow vizualization_window(VideoMode(1024, 768, desktop.bitsPerPixel), "Feel The Beat");
	const string song = "ass.wav";

	SoundProcessor spr(song);
	spr.initialize();


	vector<string> avDevices = SoundRecorder::getAvailableDevices();

	for (auto&& dev : avDevices)
	{
		cout << dev << endl;
	}
	Event event;
	while (vizualization_window.isOpen()) {
		while (vizualization_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				vizualization_window.close();
			}
		}
		spr.update();
		vizualization_window.clear();
		spr.draw(vizualization_window);
		vizualization_window.display();

		
	}

}*/