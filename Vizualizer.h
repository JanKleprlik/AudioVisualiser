#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "SoundProcessor.h"
#include "Modes.h"
class Vizualizer
{
public:
	Vizualizer();
	void run();
private:
	sf::RenderWindow window;	//maybe add some settings for the window - just like FPS
	std::string active_song;
	std::string active_mode;
	void initialize_mode(const std::string& mode_name, const std::string& song_name);
	
	Menu menu;
	std::unique_ptr<AbstractMode> mode;
	SoundProcessor processor;
	
	
	bool is_menu_active = true;
	const int FPS = 40;
};

