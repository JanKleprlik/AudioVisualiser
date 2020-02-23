#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "SoundProcessor.h"
class Vizualizer
{
public:
	Vizualizer();
	void run();
private:
	sf::RenderWindow window;
	std::string active_song;
	std::string active_mode;
	
	Menu menu;
	SoundProcessor processor;
	
	bool is_menu_active = true;
	const int FPS = 40;
};

