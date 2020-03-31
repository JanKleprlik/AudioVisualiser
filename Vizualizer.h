#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Modes.h"


#define WIDTH 1920
#define HEIGHT 1080
#define FPS 55


class Vizualizer
{
public:
	Vizualizer();
	void run();
private:
	sf::RenderWindow window;
	std::string active_song;
	std::string active_mode;
	void initialize_mode(const std::string& mode_name, const std::string& song_name);
	
	Menu menu;
	std::unique_ptr<AbstractMode> mode;
	
	
	bool is_menu_active = true;
};

