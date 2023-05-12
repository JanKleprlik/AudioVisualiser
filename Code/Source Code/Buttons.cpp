// Copyright (c) 2020 Jan Kleprlík <kleprlikjan@gmail.com>
// Licensed under GPL v3 or later

#include <iostream>
#include "../Headerss/Menu.h"

#pragma region Abstract button
/**
 * Returns true if mouse is on the button. False otherwise.
 */
bool AbstractButton::is_focused(sf::Vector2i vector)
{
	return (x_pos <= vector.x && vector.x <= x_pos + x_dim &&
			y_pos <= vector.y && vector.y <= y_pos + y_dim);
}
/**
 * Sets the text font.
 */
void AbstractButton::set_font(sf::Font& font)
{
	
	text.setFont(font);
}
/**
 * Returns text of the button.
 */
std::string AbstractButton::get_text()
{
	return text.getString();
}
/**
 * Sets text color.
 */
void AbstractButton::set_text_color(sf::Color& color)
{
	text.setFillColor(color);
}
/**
 * Sets text size.
 */
void AbstractButton::set_text_size(int size)
{
	text.setCharacterSize(size);
}
/**
 * Sets text.
 */
void AbstractButton::set_text_string(const std::string& text_string)
{
	text.setString(text_string);
	text.setString(text_string);
}

#pragma endregion 

#pragma region Round Button
/**
 * Sets button color.
 */
void RoundButton::set_button_color(sf::Color& color)
{
	circ_left.setFillColor(color);
	circ_right.setFillColor(color);
	rect_mid.setFillColor(color);
}
/**
 * Draws button with round edges to centered by [x,y].
 */
void RoundButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);

	//drawing button
	draw(window);
}
/**
 * Helper function. Draws button drawables.
 */
void RoundButton::draw(sf::RenderWindow& window)
{
	//drawing button
	window.draw(circ_right);
	window.draw(rect_mid);
	window.draw(circ_left);
	window.draw(text);
}
/**
 * Sets button position centered by [x,y].
 */
void RoundButton::set_position(float x, float y)
{
	//setting positions relative to center of the button
	x_pos = x - x_dim / 2;
	y_pos = y - y_dim / 2;
	
	circ_left.setPosition(x_pos, y_pos);
	rect_mid.setPosition(x_pos + y_dim / 2, y_pos);
	circ_right.setPosition(x + x_dim / 2 - y_dim, y_pos);

	const sf::Rect<float> size = rect_mid.getGlobalBounds();
	text.setPosition(size.left, size.top + (size.height - text.getGlobalBounds().height) / 4);	
}

#pragma endregion

#pragma region Triangle Button
/**
 * Sets button color.
 */
void TriangleButton::set_button_color(sf::Color& color)
{
	triangle.setFillColor(color);
}
/**
 * Rotates button by 90 degrees to the right.
 */
void TriangleButton::rotate_90_deg()
{
	num_of_rotations = (num_of_rotations + 1) % 4;
	//moving original positions regarding to rotation
	switch (num_of_rotations)
	{
	case 0: y_pos += x_dim; break;
	case 1: x_pos -= x_dim; break;
	case 2: y_pos -= x_dim; break;
	case 3: x_pos += x_dim; break;
	}

	triangle.rotate(90.f);
}
/**
 * Draws triangle button centered by [x,y].
 */
void TriangleButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);
	
	//drawing button
	draw(window);
}
/**
 * Helper function. Draws drawables.
 */
void TriangleButton::draw(sf::RenderWindow& window)
{
	//drawing button
	window.draw(triangle);
	window.draw(text);
}
/**
 * Sets the position of triangle button centered by [x,y].
 */
void TriangleButton::set_position(float x, float y)
{
	//setting positions relative to center of the button
	x_pos = x - (x_dim / 2);
	y_pos = y - (y_dim / 2);

	triangle.setPosition(x_pos, y_pos);
}

#pragma endregion 

#pragma region Plus Button
/**
 * Sets button color.
 */
void PlusButton::set_button_color(sf::Color& color)
{
	plus.setFillColor(color);
}
/**
 * Draws plus button centered by [x,y].
 */
void PlusButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);
	
	//drawing button
	draw(window);
	
}
/**
 * Helper function. Draws drawables.
 */
void PlusButton::draw(sf::RenderWindow& window)
{
	//drawing button
	window.draw(plus);
	window.draw(text);
	//drawing black corners so '+' sign appears
	window.draw(plus_cover_lt);
	window.draw(plus_cover_rt);
	window.draw(plus_cover_lb);
	window.draw(plus_cover_rb);
}
/**
 * Sets the position centered by [x,y].
 */
void PlusButton::set_position(float x, float y)
{
	x_pos = x - (x_dim / 2);	//note: is square
	y_pos = y - (x_dim / 2);
	plus.setPosition(x_pos,y_pos);

	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(x, y);

	plus_cover_lt.setPosition(x_pos, y_pos);
	plus_cover_rt.setPosition(x_pos + (x_dim / 2) + thickness/2, y_pos);
	plus_cover_lb.setPosition(x_pos, y_pos + (x_dim / 2) + thickness/2);
	plus_cover_rb.setPosition(x_pos + (x_dim / 2) + thickness/2, y_pos + (x_dim / 2) + thickness/2);
}


#pragma endregion 

#pragma region  Text Button
/**
 * Does nothing. Text Button color is always black.
 */
void TextButton::set_button_color(sf::Color& color)
{
	//nothing happens
}

/**
 * Sets position relative to top left corner.
 */
void TextButton::set_position(float x, float y)
{
	x_pos = x;
	y_pos = y;
	x_dim = text.getLocalBounds().width;
	y_dim = text.getLocalBounds().height;
	text.setPosition(x_pos, y_pos);
}
/**
 * Helper function. Draws drawables.
 */
void TextButton::draw(sf::RenderWindow& window)
{
	window.draw(text);
}
/**
 * Draws text button with top left corner at [x,y].
 */
void TextButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);

	//drawing button
	draw(window);
}
#pragma endregion 

#pragma region Play Button
/**
 * Activates play mode. Disables the menu.
 */
void PlayButton::activate(Menu& menu)
{
	menu.is_active = false;
}

#pragma endregion 

#pragma region Increment Button
/**
 * Increments current menu page.
 */
void IncrementButton::activate(Menu& menu)
{
	menu.change_page(inc);
}

#pragma endregion 

#pragma region Mode Button
/**
 * Sets mode button as active. 
 */
void ModeButton::activate(Menu& menu)
{
	if (menu.active_mode_button != nullptr)
	{
		//deactivating old button
		menu.active_mode_button->set_button_color(this->light_grey);
		menu.active_mode_button->set_text_color(black);
	}
	
	menu.active_mode_button = this;
	this->set_button_color(this->active_grey);
	this->set_text_color(white);

	menu.set_mode();
}

#pragma endregion

#pragma region Song Button
/**
 * Sets song button as active.
 */
void SongButton::activate(Menu& menu)
{
	if (menu.active_song_button != nullptr)
	{
		//deactivating old button
		menu.active_song_button->set_button_color(this->light_grey);
		menu.active_song_button->set_text_color(black);
	}
	
	menu.active_song_button = this;
	this->set_button_color(this->active_grey);
	this->set_text_color(white);

	menu.set_song();
}

#pragma endregion 

#pragma region AddSong Button
/**
 * Reads input from console and adds the song to database. If given name is valid.
 */
void AddSongButton::activate(Menu& menu)
{
	std::string song_name;

	std::cin >> song_name;
	
	menu.add_song(song_name);
}

#pragma endregion

#pragma region Unclickable Button
/**
 * Mockup button. Does nothing.
 */
void UnclickableButton::activate(Menu& menu)
{
	//Nothing to do here
}

#pragma endregion

