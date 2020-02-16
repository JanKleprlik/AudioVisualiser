// AudioVizualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include <iostream>
/*/
int main()
{
    std::cout << "Hello World!\n";
}
*/
int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	float i = 0;



	// get the clipboard content as a string
	sf::String string = sf::Clipboard::getString();
	// or use it in the event loop
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::KeyPressed)
		{
			// Using Ctrl + V to paste a string into SFML
			if (event.key.control && event.key.code == sf::Keyboard::V)
				string = sf::Clipboard::getString();
			// Using Ctrl + C to copy a string out of SFML
			if (event.key.control && event.key.code == sf::Keyboard::C)
				sf::Clipboard::setString("Hello World!");
		}
	}


	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		i++;
		if (i > 100) i = 0;
		shape.setRadius(i);
		//sf::CircleShape shape(i);
		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}