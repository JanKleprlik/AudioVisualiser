#include "Menu.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;
/*
 * Renders menu sprites
 */
void Menu::draw(RenderWindow& window)
{
	for (auto&& button : buttons)
	{
		button->draw(window);
	}
	/*/
	//HEADERS
	text.setCharacterSize(60);
	draw_big_button(130.f, 80.f, window, "Songs");
	draw_big_button(130.f + table_move.x, 80.f, window, "Modes");

	//"Play"
	text.setString("Play");
	text.setPosition(780.f, 653.f);
	text.setFillColor(Color::White);
	window.draw(text);

	//"song names"
	text.setCharacterSize(24);
	text.setFillColor(Color::Black);
	//draw_song_table(130.f, 200.f, window);

	//"mode names"
	//draw_mode_table(130.f + table_move.x, 200.f, window);

	//"song picker controller"
	draw_song_controller(130.f, 670.f, window);
	/**/


}

void Menu::draw_big_button(float x, float y, sf::RenderWindow& window, const std::string& text_string)
{
	big_circ_left.setPosition(x, y);
	big_circ_right.setPosition(x + 210, y);
	big_rect.setPosition(x + 50, y);
	window.draw(big_circ_right);
	window.draw(big_circ_left);
	window.draw(big_rect);

	text.setPosition(x + 60, y + 8);
	text.setString(text_string);
	window.draw(text);
}

/*/
void Menu::draw_headers(float x, float y, sf::RenderWindow& window)
{
	//"Songs"
	big_circ_left.setPosition(x, y);
	big_circ_right.setPosition(x+210, y);
	big_rect.setPosition(x+50, y);
	window.draw(big_circ_right);
	window.draw(big_circ_left);
	window.draw(big_rect);

	text.setPosition(x+60, y+8);
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
}
/**/

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

void Menu::draw_song_controller(float x, float y, sf::RenderWindow& window)
{
	triangle.setRotation(270.f);
	triangle.setPosition(x, y);
	window.draw(triangle);
	triangle.setRotation(90.f);
	triangle.setPosition(x + 310, y - 50);
	window.draw(triangle);

	//"plus"
	plus.setPosition(x + 130, y - 50);
	window.draw(plus);
	plus_cover.setPosition(x + 130, y - 50);
	window.draw(plus_cover);
	plus_cover.setPosition(x + 160, y - 50);
	window.draw(plus_cover);
	plus_cover.setPosition(x + 130, y - 20);
	window.draw(plus_cover);
	plus_cover.setPosition(x + 160, y - 20);
	window.draw(plus_cover);
}

/*/
void Menu::draw_vizualize_button(float x, float y, sf::RenderWindow& window, const string& text_string)
{
	big_circ_left.setPosition(x, y);
	big_circ_right.setPosition(x + 210, y);
	big_rect.setPosition(x + 50, y);
	window.draw(big_circ_right);
	window.draw(big_circ_left);
	window.draw(big_rect);

	text.setPosition(x + 60, y + 8);
	text.setString(text_string);
	window.draw(text);
}
/**/

/*
 * Menu Constructor
 * Initializes color for sprites, loads song database.
 */
Menu::Menu()
{

	#pragma region loading song names

	ifstream sr("Resources/Songs/song_database.txt");
	string song_name_str;
	while (getline(sr, song_name_str))
	{
		database.add_song(song_name_str);
	}
	#pragma endregion

	//loading font
	if (!font.loadFromFile("Resources/Fonts/sansation.ttf"))
	{
		cout << "ERROR: was unable to load font \"sensation.tff\"" << endl;
		EXIT_FAILURE;
	}

	#pragma region SONGS COLLUMN
	{
		constexpr float x = 285.f;
		constexpr float y = 130.f;
		RoundButton songs_header(310.f, 100.f);
		songs_header.set_button_color(grey);
		songs_header.set_font(font);
		songs_header.set_text_size(60);
		songs_header.set_text_string("SONGS");
		songs_header.set_position(x, y);
		buttons.push_back(make_unique<RoundButton>(songs_header));

		for (int i = 0; i < 6; i++)
		{
			RoundButton song_name(290.f, 50.f);
			song_name.set_button_color(light_grey);
			song_name.set_text_color(black);
			song_name.set_font(font);
			song_name.set_text_size(24);
			song_name.set_text_string(database.get_song_at(i + song_page * 6));
			song_name.set_position(x, y + 70 * (i + 1) + 25);
			buttons.push_back(make_unique<RoundButton>(song_name));
		}
	}
	#pragma endregion

	#pragma region MODES COLLUMN
	{
		//MODES COLLUMN
		constexpr float x = 739.f;
		constexpr float y = 130.f;
		RoundButton modes_header(310.f, 100.f);
		modes_header.set_button_color(grey);
		modes_header.set_font(font);
		modes_header.set_text_size(60);
		modes_header.set_text_string("MODES");
		modes_header.set_position(x, y);
		buttons.push_back(make_unique<RoundButton>(modes_header));

		for (int i = 0; i < 6; i++)
		{
			RoundButton mode_name(290.f, 50.f);
			mode_name.set_button_color(light_grey);
			mode_name.set_text_color(black);
			mode_name.set_font(font);
			mode_name.set_text_size(24);
			mode_name.set_text_string(modes.at(i + song_page * 6));
			mode_name.set_position(x, y + 70 * (i + 1) + 25);
			buttons.push_back(make_unique<RoundButton>(mode_name));
		}
	}
	#pragma endregion

	#pragma region CONTROLS
	{
		constexpr float x = 165.f;
		constexpr float y = 655.f;

		constexpr int size = 60;

		TriangleButton previous(size);
		previous.set_button_color(grey);
		previous.set_position(x, y + size);
		previous.rotate_90_deg();
		previous.rotate_90_deg();
		previous.rotate_90_deg();
		previous.set_text_string("previous");
		buttons.push_back(make_unique<TriangleButton>(previous));

		TriangleButton next(size);
		next.set_button_color(grey);
		next.set_position(x + 300.f, y);
		next.rotate_90_deg();
		next.set_text_string("next");
		buttons.push_back(make_unique<TriangleButton>(next));

		PlusButton plus(size, 10);
		plus.set_button_color(grey);
		plus.set_text_string("add song");
		plus.set_position(285.f, y);
		buttons.push_back(make_unique<PlusButton>(plus));

		TextButton Play("Play", 90);
		Play.set_font(font);
		Play.set_text_color(white);
		Play.set_position(780.f, y-30);
		buttons.push_back(make_unique<TextButton>(Play));
		
	}
	#pragma endregion
	
	big_circ_right.setFillColor(grey);
	big_circ_left.setFillColor(grey);
	small_circ_left.setFillColor(light_grey);
	small_circ_right.setFillColor(light_grey);
	big_rect.setFillColor(grey);
	small_rect.setFillColor(light_grey);
	triangle.setFillColor(grey);
	plus.setFillColor(grey);
	plus_cover.setFillColor(Color::Black);
}
