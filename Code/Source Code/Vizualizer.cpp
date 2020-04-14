#include <iostream>
#include "../Headerss/Vizualizer.h"

using namespace std;
using namespace sf;

/**
 * Main function. Handles all user input.
 */
void Vizualizer::run()
{
	Event event;
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
					
					for(auto&& button : menu.mode_buttons)
					{
						if (button->is_focused(mouse_vec))
						{
							cout << button->get_text() << endl;
							button->activate(menu);
						}
					}

					for (int i = 0; i < min(6, int(menu.song_buttons.size()) - menu.song_page * 6); i++)
					{
						if (menu.song_buttons[i+menu.song_page * 6]->is_focused(mouse_vec))
						{
							cout << menu.song_buttons[i + menu.song_page * 6]->get_text() << endl;
							menu.song_buttons[i + menu.song_page * 6]->activate(menu);
						}
					}
				}
			}
			else
			{
				if (event.type == Event::MouseButtonReleased && menu.back_button->is_focused(Mouse::getPosition(window)))
				{
					cout << "back" << endl;
					is_menu_active = true;
					menu.restart();
					mode = nullptr;


				}
			}
			#pragma endregion 
		}
		
		window.clear();
		if (is_menu_active)
		{
			menu.draw(window);
			is_menu_active = menu.is_active;
			if (!is_menu_active)
			{

				initialize_mode(menu.get_mode(), menu.get_song());
				menu.quit();
			}
		}
		else
		{	
			mode->update();
			window.clear();
			mode->draw(window);
			
			//draw back button
			menu.back_button->draw(window);			
		}
		window.display();
	}
}

/**
 * Vizualizer constructor
 * Sets parameters for window.
 */
Vizualizer::Vizualizer()
{
	ContextSettings settings;
	settings.antialiasingLevel = 4;
	
	const VideoMode desktop = VideoMode::getDesktopMode();
	window.create(VideoMode(WIDTH, HEIGHT, desktop.bitsPerPixel), "Feel The Beat",Style::Default, settings);
	window.setFramerateLimit(FPS);
}

/**
 * Starts up current mode.
 */
void Vizualizer::initialize_mode(const std::string& mode_name, const std::string& song_name)
{

	if (mode_name == "Amplitude")
	{
		mode = std::make_unique<Amplitude>(song_name);
	}
	else if (mode_name == "Radio")
	{
		mode = std::make_unique<Radio>(song_name);
	}
	else if (mode_name == "Map")
	{
		mode = std::make_unique<Map>(song_name);

	}
	else if (mode_name == "Space")
	{
		mode = std::make_unique<Space>(song_name);
	}
	else
	{
		throw std::invalid_argument("Received name of mode that has no initialization implemented.");
	}
}