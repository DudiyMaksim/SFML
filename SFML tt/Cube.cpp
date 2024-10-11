#include"Figure.hpp"

class Cube : public Figure
{
private:
public:
	Cube(String path, int count) : Figure(path)
	{

	}
	
	void init()
	{
		mainSprite = new Sprite();
		mainSprite->setTexture(*texture);
		sprites.push_back(mainSprite);
	}

	void draw(RenderTarget& target, RenderStates states)
	{

	}
};