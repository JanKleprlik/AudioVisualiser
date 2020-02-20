#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "SongDatabase.h"

class Menu
{
public:
	Menu();
	void draw(sf::RenderWindow& window);

private:

	
	SongDatabase database;
	int song_page = 0;

	// Helper funcitons
	void draw_small_table(float x, float y, sf::RenderWindow& window);
	
	// Helper variables & constants
	const sf::Color grey = sf::Color(146, 146, 146);
	const sf::Color light_grey = sf::Color(209, 209, 209);
	sf::Font sansation;
	sf::Vector2f table_move = sf::Vector2f(454.f, 0.f);

	//sprites
	sf::CircleShape big_circ_left = sf::CircleShape(50.f, 30);
	sf::CircleShape big_circ_right = sf::CircleShape(50.f, 30);
	sf::CircleShape small_circ_left = sf::CircleShape(25.f, 30);
	sf::CircleShape small_circ_right = sf::CircleShape(25.f, 30);

	sf::RectangleShape big_rect = sf:: RectangleShape(sf::Vector2f(210.f, 100.f));
	sf::RectangleShape small_rect = sf::RectangleShape(sf::Vector2f(260.f, 50.f));

	sf::Text text = sf::Text("", sansation, 24);
};



class Vizualizer
{
public:
	Vizualizer();
	void run();
private:
	sf::RenderWindow window;
	std::string active_song;
	Menu menu;
	bool is_menu_active = true;

};

