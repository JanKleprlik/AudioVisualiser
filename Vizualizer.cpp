#include "Vizualizer.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

#define WIDTH 1024
#define HEIGHT 768

void Vizualizer::run()
{
	Event event;
	const Clock clock;
	Time time;
	int i = 0;
	while(window.isOpen())
	{
		while(window.pollEvent(event))
		{

			#pragma region Event handling
			if (event.type == Event::Closed)
			{
				window.close();
				break;
			}
			if (is_menu_active)
			{
				if (event.type == Event::MouseButtonReleased)
				{
					auto mouse_vec = Mouse::getPosition(window);
					for(auto&& button : menu.buttons)
					{
						if (button->is_focused(mouse_vec))
						{
							cout << button->get_text() << endl;
							button->activate(menu);
						}
					}
					for(auto&& button : menu.song_buttons)
					{
						if (button->is_focused(mouse_vec))
						{
							cout << button->get_text() << endl;
							button->activate(menu);
						}
					}					
					//cout << "Button Released At " << Mouse::getPosition(window).x << " : " << Mouse::getPosition(window).y << endl;
				}
			}
			#pragma endregion 
		}

		

		window.clear();
		if (is_menu_active)
		{
			//draw menu
			menu.draw(window);
			is_menu_active = menu.active;
			if (!is_menu_active)
			{
				processor.initialize(menu.get_song());
			}
		}
		else
		{
			//cout << menu.active << " : " << menu.get_mode() << " : " << menu.get_song() << endl;
			//draw vizualization
			processor.update();
			window.clear();
			processor.draw(window);
			
		}



		window.display();

		//FPS
		/*/
		time = clock.getElapsedTime();
		int fps = i / time.asSeconds();
		cout << fps << endl;
		i++;
		/**/
	}
}

/*
 * Vizualizer constructor
 * Sets parameters for window.
 */
Vizualizer::Vizualizer()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	const VideoMode desktop = VideoMode::getDesktopMode();
	window.create(VideoMode(WIDTH, HEIGHT, desktop.bitsPerPixel), "Feel The Beat",Style::Default, settings);
	window.setFramerateLimit(FPS);
}


