#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <functional>


class AbstractButton
{
public:
	virtual ~AbstractButton() {};
	virtual bool is_focused(sf::Vector2i vector);											//true if mouse is on the button, false otherwise
	virtual void set_text_color(sf::Color& color);						//sets the color of the button
	virtual std::string get_text();										//returns the text of the button
	virtual void set_font(sf::Font& font);								//sets the font of the text on the button
	virtual void set_text_size(int size);								//sets size of the text on the button
	virtual void set_text_string(const std::string& text_string);		//sets the string of the button

	
	virtual void set_button_color(sf::Color& color) = 0;				//sets color of the button
	virtual void draw(float x, float y, sf::RenderWindow& window) = 0;  //draws the button at centered position [x,y]
	virtual void draw(sf::RenderWindow& window) = 0;					//draws the button at internaly set position
	virtual void set_position(float x, float y) = 0;					//sets the position of a button (origin is at the center)
	//virtual void activate(Menu& menu) = 0;

protected:
	AbstractButton(float x, float y) : x_dim(x), y_dim(y), x_pos(0.f), y_pos(0.f) {};
	
	//dimensions in pixels
	float x_dim, y_dim;
	//position
	float x_pos, y_pos;
	//button text
	sf::Text text;
};

class RoundButton final : public AbstractButton 
{
public:
	/*
	 * x - widht
	 * y - height
	 */
	RoundButton(float x,float y) : AbstractButton(x, y) //x -> width, y -> height
	{
		circ_left = sf::CircleShape(y_dim / 2);
		circ_right = sf::CircleShape(y_dim / (2));
		rect_mid = sf::RectangleShape(sf::Vector2f(x_dim-y_dim, y_dim));
	};
	void set_button_color(sf::Color& color) override;
	void draw(float x, float y, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;
	void set_position(float x, float y) override;
private:
	sf::CircleShape circ_left;
	sf::CircleShape circ_right;
	sf::RectangleShape rect_mid;
};

class TriangleButton final : public AbstractButton
{
public:
	/*
	 * x - height of the triangle
	 */
	TriangleButton(float x) : AbstractButton(x, x)
	{
		triangle.setOrigin(x_dim / 2, x_dim / 2);
		triangle = sf::CircleShape(x_dim/ 2, 3);
	};
	void set_button_color(sf::Color& color) override;
	void rotate_90_deg(); //clockwise rotation
	void draw(float x, float y, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;
	void set_position(float x, float y) override;
private:
	sf::CircleShape triangle;
	int num_of_rotations = 0;
};

class PlusButton final : public AbstractButton
{
public:
	/*
	 * x - height of the plus
	 * y - thickness of the plus
	 */
	PlusButton(float x, float y) : AbstractButton(x, x)
	{
		thickness = y;
		plus = sf::RectangleShape(sf::Vector2f((float)x_dim, (float)y_dim));
		plus_cover_lt = sf::RectangleShape(sf::Vector2f(x_dim / (float)2 - y / 2, y_dim / (float)2 - y / 2));
		plus_cover_lt.setFillColor(sf::Color::Black);
		plus_cover_rt = sf::RectangleShape(sf::Vector2f(x_dim / (float)2 - y / 2, y_dim / (float)2 - y / 2));
		plus_cover_rt.setFillColor(sf::Color::Black);
		plus_cover_lb = sf::RectangleShape(sf::Vector2f(x_dim / (float)2 - y / 2, y_dim / (float)2 - y / 2));
		plus_cover_lb.setFillColor(sf::Color::Black);
		plus_cover_rb = sf::RectangleShape(sf::Vector2f(x_dim / (float)2 - y / 2, y_dim / (float)2 - y / 2));
		plus_cover_rb.setFillColor(sf::Color::Black);
		
	};
	void set_button_color(sf::Color& color) override;
	void draw(float x, float y, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;
	void set_position(float x, float y) override;
private:
	sf::RectangleShape plus;
	sf::RectangleShape plus_cover_lt;	//left top
	sf::RectangleShape plus_cover_rt;	//right top
	sf::RectangleShape plus_cover_lb;	//left bottom
	sf::RectangleShape plus_cover_rb;	//right bottom
	float thickness;
	
};
class TextButton final : public AbstractButton
{
public:
	TextButton(std::string text_, int size):AbstractButton(0, 0)
	{
		text.setString(text_);
		text.setCharacterSize(size);
		x_dim = text.getLocalBounds().width;
		y_dim = text.getLocalBounds().height;		
	}
	void set_button_color(sf::Color& color) override;
	void draw(float x, float y, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;
	void set_position(float x, float y) override;
};