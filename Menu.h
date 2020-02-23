#pragma once
#include <SFML/Graphics.hpp>
#include "SongDatabase.h"
#include "Buttons.h"

class Menu
{
public:
	Menu();
	void draw(sf::RenderWindow& window);


	std::string chosen_song;
	std::string chosen_mode;
	bool active = true;

	//Buttons
	std::vector<std::unique_ptr<AbstractButton>> buttons;
	
	//button functions
	void start_playing();
	void set_chosen_song(std::string song);
	void set_chosen_mode(std::string mode);
	void change_page(int i);
	void add_song(std::string mode);
private:


	SongDatabase database;
	std::vector<std::string> modes = std::vector<std::string>{ "Amplitude","Radio","Map","Stripes","Chaos","Space" };
	int song_page = 0;



	// Helper variables & constants
	sf::Font font;

	sf::Color grey = sf::Color(146,146,146);
	sf::Color light_grey = sf::Color(209, 209, 209);
	sf::Color active_green = sf::Color(90, 204, 69);
	sf::Color active_grey = sf::Color(209, 209, 209, 80);
	sf::Color black = sf::Color(0, 0, 0);
	sf::Color white = sf::Color(255, 255, 255);



	

	//TODO: DELETE THIS LATER AND EVERYTHING CORRESPONIGN IN .cpp
	
	//sprites
	sf::Vector2f table_move = sf::Vector2f(454.f, 0.f);
	sf::CircleShape big_circ_left = sf::CircleShape(50.f, 30);
	sf::CircleShape big_circ_right = sf::CircleShape(50.f, 30);
	sf::CircleShape small_circ_left = sf::CircleShape(25.f, 30);
	sf::CircleShape small_circ_right = sf::CircleShape(25.f, 30);
	sf::CircleShape triangle = sf::CircleShape(25.f, 3.f);
	sf::RectangleShape big_rect = sf::RectangleShape(sf::Vector2f(210.f, 100.f));
	sf::RectangleShape small_rect = sf::RectangleShape(sf::Vector2f(260.f, 50.f));

	sf::RectangleShape plus = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
	sf::RectangleShape plus_cover = sf::RectangleShape(sf::Vector2f(20.f, 20.f));

	sf::Text text = sf::Text("", font, 24);
	
	// Helper funcitons
//void draw_headers(float x, float y, sf::RenderWindow& window);
	void draw_big_button(float x, float y, sf::RenderWindow& window, const std::string& text);
	void draw_song_table(float x, float y, sf::RenderWindow& window);
	void draw_mode_table(float x, float y, sf::RenderWindow& window);
	void draw_song_controller(float x, float y, sf::RenderWindow& window);
	//void draw_vizualize_button(float x, float y, sf::RenderWindow& window, const std::string& text);
};
