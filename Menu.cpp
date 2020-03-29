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
	update_buttons();
	for (auto&& button : buttons)
	{
		button->draw(window);
	}
	for (auto&& song_button : song_buttons)
	{
		song_button->draw(window);
	}
}

Menu::Menu()
{

	#pragma region loading song names

	ifstream sr("Resources/Songs/song_database.txt");
	string song_name_str;
	while (getline(sr, song_name_str))
	{
		database.load_song(song_name_str);
	}
	#pragma endregion

	//loading font
	if (!font.loadFromFile("Resources/Fonts/sansation.ttf"))
	{
		cout << "ERROR: was unable to load font \"sensation.tff\"" << endl;
		EXIT_FAILURE;
	}
	//loading music to play
	
	

	//setting sprites for drawing
	#pragma region SONGS COLLUMN
	{
		constexpr float x = 285.f;
		constexpr float y = 130.f;
		UnclickableButton songs_header(310.f, 100.f);
		songs_header.set_button_color(grey);
		songs_header.set_font(font);
		songs_header.set_text_size(60);
		songs_header.set_text_string("SONGS");
		songs_header.set_position(x, y);
		buttons.push_back(make_unique<UnclickableButton>(songs_header));

		for (int i = 0; i < 6; i++)
		{
			SongButton song_name(290.f, 50.f);
			song_name.set_button_color(light_grey);
			song_name.set_text_color(black);
			song_name.set_font(font);
			song_name.set_text_size(24);
			song_name.set_text_string(database.get_song_at(i + song_page * 6));
			song_name.set_position(x, y + 70 * (i + 1) + 25);
			song_buttons.push_back(make_unique<SongButton>(song_name));
		}
	}
	#pragma endregion

	#pragma region MODES COLLUMN
	{
		//MODES COLLUMN
		constexpr float x = 739.f;
		constexpr float y = 130.f;
		UnclickableButton modes_header(310.f, 100.f);
		modes_header.set_button_color(grey);
		modes_header.set_font(font);
		modes_header.set_text_size(60);
		modes_header.set_text_string("MODES");
		modes_header.set_position(x, y);
		buttons.push_back(make_unique<UnclickableButton>(modes_header));

		for (size_t i = 0; i < modes.size(); i++)
		{
			ModeButton mode_name(290.f, 50.f);
			mode_name.set_button_color(light_grey);
			mode_name.set_text_color(black);
			mode_name.set_font(font);
			mode_name.set_text_size(24);
			mode_name.set_text_string(modes.at(i + song_page * 6));
			mode_name.set_position(x, y + 70 * (i + 1) + 25);
			buttons.push_back(make_unique<ModeButton>(mode_name));
		}
	}
	#pragma endregion

	#pragma region CONTROLS
	{
		constexpr float x = 165.f;
		constexpr float y = 655.f;

		constexpr int size = 60;

		IncrementButton previous(size);
		previous.set_increment(-1);
		previous.set_button_color(grey);
		previous.set_position(x, y + size);
		previous.rotate_90_deg();
		previous.rotate_90_deg();
		previous.rotate_90_deg();
		previous.set_text_string("previous");
		buttons.push_back(make_unique<IncrementButton>(previous));

		IncrementButton next(size);
		next.set_increment(+1);
		next.set_button_color(grey);
		next.set_position(x + 300.f, y);
		next.rotate_90_deg();
		next.set_text_string("next");
		buttons.push_back(make_unique<IncrementButton>(next));

		AddSongButton plus(size, 10);
		plus.set_button_color(grey);
		plus.set_text_string("add song");
		plus.set_position(285.f, y);
		buttons.push_back(make_unique<AddSongButton>(plus));

		PlayButton Play("Play", 90);
		Play.set_font(font);
		Play.set_text_color(white);
		Play.set_position(780.f, y-30);
		buttons.push_back(make_unique<PlayButton>(Play));
		
	}
	#pragma endregion

	#pragma region BACK BUTTON
	back_button = std::make_unique<BackButton>(30);
	
	back_button->set_position(30.f, 30.f + 30.f);
	back_button->rotate_90_deg();
	back_button->rotate_90_deg();
	back_button->rotate_90_deg();
	back_button->set_text_string("back");
	Color grey = Color(146, 146, 146);
	back_button->set_button_color(grey);
#pragma endregion 
	
	song_buffer.loadFromFile(song_name);
	song.setBuffer(song_buffer);
	song.setLoop(true);
	song.play();
}

void Menu::add_song(const std::string& song)
{
	database.add_song(song);
}
void Menu::change_page(int i)
{
	if (song_page + i < 0 || song_page + i > (database.get_size()%6)+1)
	{
		return;
	}
	else
	{
		song_page += i;
	}
}
void Menu::set_mode(const std::string& mode)
{
	chosen_mode = mode;
}
void Menu::set_song(const std::string& song)
{
	chosen_song = song;
}

void Menu::update_buttons()
{
	int i = 0;
	for(auto&& button : song_buttons)
	{
		button->set_text_string(database.get_song_at(i + song_page * 6));
		i++;
	}
}

std::string& Menu::get_mode()
{
	return chosen_mode;
}
std::string& Menu::get_song()
{
	return chosen_song;
}

void Menu::quit()
{
	song.stop();
}

void Menu::restart()
{
	song.play();
	active = true;
}

