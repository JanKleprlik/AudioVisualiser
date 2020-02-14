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