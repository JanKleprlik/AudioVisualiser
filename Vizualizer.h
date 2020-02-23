#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Menu.h"
class Vizualizer
{
public:
	Vizualizer();
	void run();
private:
	sf::RenderWindow window;
	std::string active_song;
	std::string active_mode; //perhaps int
	Menu menu;
	bool is_menu_active = true;
	const int FPS = 15;
};

