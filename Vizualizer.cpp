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
	while(window.isOpen())
	{
		while(window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			}
			
		}

		window.clear();
		if (is_menu_active)
		{
			//draw menu
			menu.draw(window);
		}
		else
		{
			//draw vizualization
		}
		window.display();
	}
}


/*
 * Renders menu sprites
 */
void Menu::draw(RenderWindow& window)
{
	text.setCharacterSize(60);

	//HEADERS
	//"Songs"
	big_circ_left.setPosition(130, 80);
	big_circ_right.setPosition(340, 80);
	big_rect.setPosition(180, 80);
	window.draw(big_circ_right);
	window.draw(big_circ_left);
	window.draw(big_rect);

	text.setPosition(190, 88);
	text.setString("Songs");
	window.draw(text);
	//"Modes"
	big_circ_left.move(table_move);
	big_circ_right.move(table_move);
	big_rect.move(table_move);
	window.draw(big_circ_right);
	window.draw(big_circ_left);
	window.draw(big_rect);

	text.move(table_move);
	text.setString("Modes");
	window.draw(text);
	


	//SONGS
	text.setCharacterSize(24);
	text.setFillColor(Color::Black);
	//"song names"
	draw_song_table(130.f, 200.f, window);

	//"mode names"
	draw_mode_table(130.f + table_move.x, 200.f, window);

	//song picker

	TriangleFan triangle

	
	//VIZUALIZE
	
}
void Menu::draw_song_table(float x, float y, RenderWindow& window)
{
	for (int i = 0; i < 6; i++)
	{
		text.setString(database.get_song_at(i + song_page * 6));
		text.setPosition(x + 26, y + 10 + i * 70);
		small_circ_left.setPosition(x, y + i * 70);
		small_circ_right.setPosition(x + 260, y + i * 70);
		small_rect.setPosition(x + 27, y + i * 70);

		window.draw(small_circ_right);
		window.draw(small_circ_left);
		window.draw(small_rect);
		window.draw(text);
	}
}

void Menu::draw_mode_table(float x, float y, sf::RenderWindow& window)
{
	for (int i = 0; i < 6; i++)
	{
		text.setString(modes[i]);
		text.setPosition(x + 26, y + 10 + i * 70);
		small_circ_left.setPosition(x, y + i * 70);
		small_circ_right.setPosition(x + 260, y + i * 70);
		small_rect.setPosition(x + 27, y + i * 70);

		window.draw(small_circ_right);
		window.draw(small_circ_left);
		window.draw(small_rect);
		window.draw(text);
	}
}



Menu::Menu()
{

	#pragma region loading song names

	ifstream sr("Resources/Songs/song_database.txt");
	string song_name;
	while(getline(sr,song_name))
	{
		database.add_song(song_name);
	}
	/*
	database.add_song("animals.wav");
	database.add_song("ass.wav");
	database.add_song("canary.wav");
	database.add_song("crazy.wav");
	database.add_song("edge.wav");
	database.add_song("fashiondrunk.wav");
	database.add_song("frequency.wav");
	database.add_song("gymno.wav");
	database.add_song("orchestral.ogg");
	database.add_song("ppl.wav");
	database.add_song("sifflet.wav");
	database.add_song("sintra.wav");
	*/
	#pragma endregion

	//loading modes
	#pragma region loading modes
	modes.push_back("Amplitude");
	modes.push_back("Radio");
	modes.push_back("Map");
	modes.push_back("Stripes");
	modes.push_back("Chaos");
	modes.push_back("Space");
	#pragma endregion 

	
	//initializing helper variables
	if (!sansation.loadFromFile("Resources/Fonts/sansation.ttf"))
	{
		cout << "ERROR: was unable to load font \"sensation.tff\"" << endl;
		EXIT_FAILURE;
	}


	big_circ_right.setFillColor(grey);
	big_circ_left.setFillColor(grey);
	small_circ_left.setFillColor(light_grey);
	small_circ_right.setFillColor(light_grey);
	big_rect.setFillColor(grey);
	small_rect.setFillColor(light_grey);
}


/*
 * Vizualizer constructor
 */
Vizualizer::Vizualizer()
{
	const VideoMode desktop = VideoMode::getDesktopMode();
	window.create(VideoMode(WIDTH, HEIGHT, desktop.bitsPerPixel), "Feel The Beat");
	window.setFramerateLimit(60);
}


