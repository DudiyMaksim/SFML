#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>

using namespace sf;

const int WIDTH = 20;
const int HEIGHT = 20;
const int TILE_SIZE = 25;

enum Direction { UP, DOWN, LEFT, RIGHT };

class Food {
private:
    Texture texture;
    Sprite sprite;
public:
    Food() {
        if (!texture.loadFromFile("Sprites/apple.png")) {
            std::cerr << "Error loading food texture" << std::endl;
            exit(-1);
        }
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        spawn();
    }

    void spawn() {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        sprite.setPosition(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    Vector2f getPosition() {
        return sprite.getPosition();
    }
};

struct SnakeSegment {
    Vector2i position;
    Direction direction;
};

class Snake {
public:
    Snake(int startX, int startY) : direction(RIGHT) {
        cornerTextures = {
            Texture(), Texture(), Texture(), Texture()
        };
        std::array<std::string, 4> cornerTextureFiles = {
            "Sprites/body_corner_up_right.png",   
            "Sprites/body_corner_up_left.png",
            "Sprites/body_corner_down_right.png", 
            "Sprites/body_corner_down_left.png"  
        };

        for (int i = 0; i < cornerTextures.size(); ++i) {
            if (!cornerTextures[i].loadFromFile(cornerTextureFiles[i])) {
                std::cerr << "Error loading texture: " << cornerTextureFiles[i] << std::endl;
                exit(-1);
            }
        }
        headTextures = {
            Texture(), Texture(), Texture(), Texture()
        };
        std::array<std::string, 4> headTextureFiles = {
            "Sprites/head_up.png",
            "Sprites/head_down.png",
            "Sprites/head_left.png",
            "Sprites/head_right.png"
        };

        for (int i = 0; i < headTextures.size(); ++i) {
            if (!headTextures[i].loadFromFile(headTextureFiles[i])) {
                std::cerr << "Error loading texture: " << headTextureFiles[i] << std::endl;
                exit(-1);
            }
        }

        if (!bodyHorizontalTexture.loadFromFile("Sprites/body_horizontal.png") ||
            !bodyVerticalTexture.loadFromFile("Sprites/body_vertical.png")) {
            std::cerr << "Error loading body textures" << std::endl;
            exit(-1);
        }

        headSprite.setTexture(headTextures[RIGHT]);
        headSprite.setPosition(startX * TILE_SIZE, startY * TILE_SIZE);

        snakeBody.push_back({ Vector2i(startX, startY), RIGHT }); 
        snakeBody.push_back({ Vector2i(startX - 1, startY), RIGHT }); 
        snakeBody.push_back({ Vector2i(startX - 2, startY), RIGHT }); 
    }

    void update() {
        Vector2i newPos = snakeBody.front().position;

        if (direction == UP) newPos.y -= 1;
        if (direction == DOWN) newPos.y += 1;
        if (direction == LEFT) newPos.x -= 1;
        if (direction == RIGHT) newPos.x += 1;

        snakeBody.insert(snakeBody.begin(), { newPos, direction });

        if (!grow) {
            snakeBody.pop_back();
        }
        else {
            grow = false;
        }
        float scaleFactor = 0.8f;
        headSprite.setPosition(newPos.x * TILE_SIZE - 6, newPos.y * TILE_SIZE - 6);

        switch (direction) {
        case UP:
            headSprite.setTexture(headTextures[0]);
            break;
        case DOWN:
            headSprite.setTexture(headTextures[1]);
            break;
        case LEFT:
            headSprite.setTexture(headTextures[2]);
            break;
        case RIGHT:
            headSprite.setTexture(headTextures[3]);
            break;
        }

        float offset = TILE_SIZE / 2;
        if (direction == UP) {
            headSprite.move(0, -offset);
        }
        else if (direction == DOWN) {
            headSprite.move(0, offset);
        }
        else if (direction == LEFT) {
            headSprite.move(-offset, 0);
        }
        else if (direction == RIGHT) {
            headSprite.move(offset, 0);
        }
    }

    void draw(RenderWindow& window) {
        window.draw(headSprite);

        Sprite bodySprite;
        float scaleFactor = 0.8f;

        for (size_t i = 1; i < snakeBody.size(); ++i) {
            const SnakeSegment& currentSegment = snakeBody[i];
            const SnakeSegment& prevSegment = snakeBody[i - 1];

            if (currentSegment.direction != prevSegment.direction) {
                if (prevSegment.direction == UP && currentSegment.direction == RIGHT) {
                    bodySprite.setTexture(cornerTextures[1]);
                }
                else if (prevSegment.direction == UP && currentSegment.direction == LEFT) {
                    bodySprite.setTexture(cornerTextures[0]);
                }
                else if (prevSegment.direction == DOWN && currentSegment.direction == RIGHT) {
                    bodySprite.setTexture(cornerTextures[3]);
                }
                else if (prevSegment.direction == DOWN && currentSegment.direction == LEFT) {
                    bodySprite.setTexture(cornerTextures[2]);
                }
                else if (prevSegment.direction == RIGHT && currentSegment.direction == DOWN) {
                    bodySprite.setTexture(cornerTextures[0]);
                }
                else if (prevSegment.direction == LEFT && currentSegment.direction == DOWN) {
                    bodySprite.setTexture(cornerTextures[1]);
                }
                else if (prevSegment.direction == RIGHT && currentSegment.direction == UP) {
                    bodySprite.setTexture(cornerTextures[2]);
                }
                else if (prevSegment.direction == LEFT && currentSegment.direction == UP) {
                    bodySprite.setTexture(cornerTextures[3]);
                }
            }
            else {
                if (currentSegment.direction == UP || currentSegment.direction == DOWN) {
                    bodySprite.setTexture(bodyVerticalTexture);
                }
                else {
                    bodySprite.setTexture(bodyHorizontalTexture);
                }
            }
            bodySprite.setPosition(currentSegment.position.x * TILE_SIZE - 5, currentSegment.position.y * TILE_SIZE - 5);
            bodySprite.setScale(scaleFactor, scaleFactor);
            window.draw(bodySprite);
        }
    }

    void drawGrid(RenderWindow& window) {
        for (int i = 0; i <= WIDTH; ++i) {
            Vertex line[] =
            {
                Vertex(Vector2f(i * TILE_SIZE, 0), Color(200, 200, 200)),
                Vertex(Vector2f(i * TILE_SIZE, HEIGHT * TILE_SIZE), Color(200, 200, 200))
            };
            window.draw(line, 2, Lines);
        }

        for (int i = 0; i <= HEIGHT; ++i) {
            Vertex line[] =
            {
                Vertex(Vector2f(0, i * TILE_SIZE), Color(200, 200, 200)),
                Vertex(Vector2f(WIDTH * TILE_SIZE, i * TILE_SIZE), Color(200, 200, 200))
            };
            window.draw(line, 2, Lines);
        }
    }void growSnake() {
        grow = true;
    }

    bool checkFoodCollision(Food& food) {
        Vector2f foodPosition = food.getPosition();
        return (snakeBody.front().position.x * TILE_SIZE + TILE_SIZE / 2 == foodPosition.x &&
            snakeBody.front().position.y * TILE_SIZE + TILE_SIZE / 2 == foodPosition.y);
    }

    void setDirection(Direction dir) {
        if ((direction == UP && dir != DOWN) ||
            (direction == DOWN && dir != UP) ||
            (direction == LEFT && dir != RIGHT) ||
            (direction == RIGHT && dir != LEFT)) {
            direction = dir;
        }
    }

    const std::vector<SnakeSegment>& getBody() const {
        return snakeBody;
    }

    int getScore() const {
        return score;
    }

    void increaseScore() {
        score++; 
    }

    bool checkSelfCollision() {
        for (size_t i = 1; i < snakeBody.size(); ++i) {
            if (snakeBody.front().position == snakeBody[i].position) {
                return true;
            }
        }
        return false;
    }

    bool hasWon() const {
        return score >= 10;
    }

private:
    std::vector<SnakeSegment> snakeBody;
    Sprite headSprite;
    std::vector<Texture> cornerTextures;
    std::vector<Texture> headTextures;
    Texture bodyHorizontalTexture;
    Texture bodyVerticalTexture;
    Direction direction;
    bool grow = false;
    int score = 0;
};

int main() {
    srand((time(0)));
    RenderWindow window(VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "Snake Game");
    window.setFramerateLimit(10);
    Snake snake(5, 5);
    Food food;

    Font font;
    Text scoreText;
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Sprites/background.png")) {
        std::cerr << "Error loading background texture" << std::endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(WIDTH * TILE_SIZE) / backgroundTexture.getSize().x,
        static_cast<float>(HEIGHT * TILE_SIZE) / backgroundTexture.getSize().y);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    snake.setDirection(UP);
                if (event.key.code == Keyboard::Down)
                    snake.setDirection(DOWN);
                if (event.key.code == Keyboard::Left)
                    snake.setDirection(LEFT);
                if (event.key.code == Keyboard::Right)
                    snake.setDirection(RIGHT);
            }
        }

        snake.update();

        if (snake.checkFoodCollision(food)) {
            snake.growSnake();
            snake.increaseScore();
            food.spawn();
        }

        const auto& body = snake.getBody();
        if (body.front().position.x < 0 || body.front().position.x >= WIDTH ||
            body.front().position.y < 0 || body.front().position.y >= HEIGHT) {
            std::cerr << "Game Over: Out of bounds!" << std::endl;
            break;
        }

        if (snake.checkSelfCollision()) {
            std::cerr << "Game Over: Collision with self!" << std::endl;
            break;
        }

        if (snake.hasWon()) {
            std::cout << "You win!" << std::endl;
            break;
        }

        window.clear();
        window.draw(backgroundSprite);
        snake.drawGrid(window);
        food.draw(window);
        snake.draw(window);

        scoreText.setString("Score: " + std::to_string(snake.getScore()));
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
