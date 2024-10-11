#include <SFML/Graphics.hpp>

using namespace sf;

class Window
{
private:
	RenderWindow* window;
public:
	Window()
	{
		window = new RenderWindow(VideoMode::getDesktopMode(), "Bomberman");
	}


	void Show() {
		window->display();
	}
};