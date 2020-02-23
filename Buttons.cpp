#include "Buttons.h"
#pragma region Abstract button
/*
 * Returns true if mouse is on the button. False otherwise.
 */
bool AbstractButton::is_focused(sf::Vector2i vector)
{
	return (x_pos <= vector.x && vector.x <= x_pos + x_dim &&
			y_pos <= vector.y && vector.y <= y_pos + y_dim);
}
void AbstractButton::set_font(sf::Font& font)
{
	text.setFont(font);
}
std::string AbstractButton::get_text()
{
	return text.getString();
}
void AbstractButton::set_text_color(sf::Color& color)
{
	text.setFillColor(color);
}
void AbstractButton::set_text_size(int size)
{
	text.setCharacterSize(size);
}
void AbstractButton::set_text_string(const std::string& text_string)
{
	text.setString(text_string);
	text.setString(text_string);
}

#pragma endregion 

#pragma region Round Button
void RoundButton::set_button_color(sf::Color& color)
{
	circ_left.setFillColor(color);
	circ_right.setFillColor(color);
	rect_mid.setFillColor(color);
}

void RoundButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);

	//drawing button
	draw(window);
}
void RoundButton::draw(sf::RenderWindow& window)
{
	//drawing button
	window.draw(circ_right);
	window.draw(rect_mid);
	window.draw(circ_left);
	window.draw(text);
}
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
void TriangleButton::set_button_color(sf::Color& color)
{
	triangle.setFillColor(color);
}

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

void TriangleButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);
	
	//drawing button
	draw(window);
}
void TriangleButton::draw(sf::RenderWindow& window)
{
	//drawing button
	window.draw(triangle);
	window.draw(text);
}
void TriangleButton::set_position(float x, float y)
{
	//setting positions relative to center of the button
	x_pos = x - (x_dim / 2);
	y_pos = y - (y_dim / 2);

	triangle.setPosition(x_pos, y_pos);
}

#pragma endregion 

#pragma region Plus Button
void PlusButton::set_button_color(sf::Color& color)
{
	plus.setFillColor(color);
}

void PlusButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);
	
	//drawing button
	draw(window);
	
}

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
void TextButton::set_button_color(sf::Color& color)
{
	//nothing happens
}

/*
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

void TextButton::draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextButton::draw(float x, float y, sf::RenderWindow& window)
{
	//setting positions relative to center of the button
	set_position(x, y);

	//drawing button
	draw(window);
}
#pragma endregion 