#include<SFML/Graphics.hpp>

using namespace sf;

class Figure : public Drawable
{
protected:
	Texture* texture;
	std::vector<Sprite*> sprites;
	Sprite* mainSprite;
	Vector2u size;
public:
	Figure(String path)
	{
		texture = new Texture();
		if (!texture->loadFromFile(path))
		{
			throw std::exception("failed load texture");
		}

		size = texture->getSize();
	}

	virtual void init() = 0;

	void draw(RenderTarget& target, RenderStates states) const = 0;

	void setPosition(Vector2f pos)
	{
		mainSprite->setPosition(pos);
	}
};