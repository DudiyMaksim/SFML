#include <SFML/Graphics.hpp>
#include<vector>

using namespace sf;

class Game
{
private:
	RenderWindow window;
	std::vector<Drawable*> objects;
	Event event;
public:
	Game() : window{VideoMode::getDesktopMode(), "Bomberman", Style::Default}, event{}
	{

	}

	void EventHandler() {
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.scancode == Keyboard::Scan::Escape)
				{
					window.close();
				}
			}
		}
	}

	void Update()
	{

	}
	void Run()
	{
		while (window.isOpen())
		{
			EventHandler();
			window.clear();
			Update();
			for (auto obj : objects) {
				window.draw(*obj);
			}
			window.clear(sf::Color(128, 128, 128));

			window.display();
		}
	}

	void Stop()
	{
		window.close();
	}
};