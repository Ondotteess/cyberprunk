#include <SFML/Graphics.hpp>
#include <iostream>
#include <assert.h>


using namespace sf;
using namespace std;

const float speed_x = 0.15;
const float speed_y = -0.4;

const int timeset = 400;

const int H = 16;				 //		Настройки игрового процесса
const int W = 80;

const int BD_HEIGHT = 150;

float n_buildings_speed = 0.1f;
float m_buildings_speed = 0.05f;
float f_buildings_speed = 0.01f;


float offsetX, offsetY;

String TileMap[H] = {
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",
	"W                      BWBW                                                    W",
	"W                                     BW                                       W",		  //		Расположение плиток
	"W                                                                              W",
	"W                                                   BWBWB                      W",
	"W                                                                              W",
	"W                                       BWBWBWBWBW                             W",
	"W                                   BW                                         W",
	"W                      BW                                                      W",
	"W                                                                              W",
	"W                                       BW                                     W",
	"BWBWBWHWBWBWBWBWBWBWBWBWBWBWHWHWHWBWBWBWBWBWHWBWBWBWBWBWBWBWBWBWBWBWBWBWBWBWBWBW",
};


class PLAYER {
public:
	char last_dir;
	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;

	PLAYER(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(50, 250, 40, 45);
		dx = dy = 0;
		currentFrame = 0;
	}

	void update(float time) {
		rect.left += dx * time;
		Collision(0);
		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;																				
		Collision(1);																					
																										
		currentFrame += 0.005 * time;																	
		if (currentFrame > 10) currentFrame -= 8;

		if (dx > 0) { sprite.setTextureRect(IntRect(45 * int(currentFrame), 295, 40, 40)); last_dir = 'r'; };
		if (dx < 0) { sprite.setTextureRect(IntRect(45 * int(currentFrame) + 40, 295, -40, 40)); last_dir = 'l'; };
		if (!dx && last_dir == 'r') sprite.setTextureRect(IntRect(45, 25, 30, 40));
		if (!dx && last_dir == 'l' ) sprite.setTextureRect(IntRect(45+30, 25, -30, 40));
		if (dy != 0 && last_dir == 'r') sprite.setTextureRect(IntRect(40, 650, 30, 40));
		if (dy != 0 && last_dir == 'l') sprite.setTextureRect(IntRect(40+30, 650, -30, 40));

		sprite.setPosition(rect.left - offsetX, rect.top);

		dx = 0;
	}

	void Collision(int dir) {
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++) {
				if (TileMap[i][j] == 'B' || TileMap[i][j] == 'H' || TileMap[i][j] == 'W') {																				//   Коллизию делает
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height; dy = 0; onGround = true; };
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32; dy = 0; }

					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;

					assert(i < H);
					assert(j < W);
				}
			}
	}
	void set(Texture& image, int x, int y) {
		sprite.setTexture(image);
		rect = FloatRect(x, y, 32, 32);

		dx = 0.1;
		currentFrame = 0;
	}
};

class ENEMY {
public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool life;

	void set(Texture& image, int x, int y) {
		sprite.setTexture(image);
		rect = FloatRect(x, y, 32, 32);

		dx = 0.05;
		currentFrame = 0;
		life = true;
	}

	void update(float time, PLAYER p) {
		rect.left += dx * time;

		Collision();
		if (abs(rect.top - p.rect.top) < 64 and abs(rect.left - p.rect.left) < 32 * 6 and abs(rect.left - p.rect.left) > 0) {
			if (rect.left > p.rect.left) {
				dx = -0.05;
			} else {
				dx = +0.05;
			}
		} else {
			dx = 0;
		}

		currentFrame += time * 0.005;
		if (currentFrame > 8) {
			currentFrame -= 7;
		}

		if (dx > 0) sprite.setTextureRect(IntRect(10 + 55 * int(currentFrame), 155, 55, 50));
		if (dx < 0) sprite.setTextureRect(IntRect(10 + 55 * int(currentFrame) + 55, 155, -55, 50));

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}
	void Collision() {
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
				if ((TileMap[i][j] == 'W')) {
					if (dx > 0) {
						rect.left = j * 32 - rect.width; dx *= -1;
					}
					else if (dx < 0) {
						rect.left = j * 32 + 32;  dx *= -1;
					}

				}
	}
};


int main() {
	RenderWindow window(VideoMode(1080, 720), "Test", Style::Fullscreen);

	Texture t;
	t.loadFromFile("./image/zero.png");

	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("./image/background.png")) {
		return EXIT_FAILURE;
	}

	Texture near_buildings_texture;
	if (!near_buildings_texture.loadFromFile("./image/near_buildings.png")) {
		return EXIT_FAILURE;											  //	Загрузка текстур
	}

	Texture buildings_texture;
	if (!buildings_texture.loadFromFile("./image/buildings.png")) {
		return EXIT_FAILURE;
	}

	Texture far_buildings_texture;
	if (!far_buildings_texture.loadFromFile("./image/far_buildings.png")) {
		return EXIT_FAILURE;
	}

	Texture tileset;
	if (!tileset.loadFromFile("./image/tileset.png")) {
		return EXIT_FAILURE;
	}

	Texture enemy1;
	if (!enemy1.loadFromFile("./image/enemy1.png")) {
		return EXIT_FAILURE;
	}

	Sprite near_buildings(near_buildings_texture);
	near_buildings.setScale(4.0f, 4.0f);
	near_buildings.setPosition(0, BD_HEIGHT);

	Sprite buildings(buildings_texture);
	buildings.setScale(4.0f, 4.0f);
	buildings.setPosition(0, BD_HEIGHT);

	Sprite far_buildings(far_buildings_texture);
	far_buildings.setScale(4.0f, 4.0f);
	far_buildings.setPosition(0, BD_HEIGHT);

	Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(window.getSize().x / backgroundSprite.getLocalBounds().width,
		window.getSize().y / backgroundSprite.getLocalBounds().height);

	Sprite tile(tileset);

	float currentFrame = 0;

	PLAYER p(t);
	ENEMY enemy;
	enemy.set(enemy1, 200, 400);
	p.set(t, 100, 400);

	Clock clock;

	RectangleShape rectangle(Vector2f(32, 32));

	View view(FloatRect(0, 0, 800, 600));
	window.setView(view);

	view.setSize(Vector2f(600, 440));
	window.setView(view);

	while (window.isOpen()) {

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / timeset;

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Left) or Keyboard::isKeyPressed(Keyboard::A)) {
			p.dx = -1 * speed_x;
			if (p.rect.left > 800 / 2) {
				near_buildings.move(n_buildings_speed, 0);
				buildings.move(m_buildings_speed, 0);							//   Сдвиг спрайтов
				far_buildings.move(f_buildings_speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) or Keyboard::isKeyPressed(Keyboard::D)) {
			p.dx = speed_x;
			if (p.rect.left > 800 / 2) {
				near_buildings.move(-n_buildings_speed, 0);
				buildings.move(-m_buildings_speed, 0);							//   Сдвиг спрайтов
				far_buildings.move(-f_buildings_speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) or Keyboard::isKeyPressed(Keyboard::W)) {
			if (p.onGround) { p.dy = speed_y; p.onGround = false; }
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		p.update(time);
		enemy.update(time, p);

		if (p.rect.left > 800 / 3) offsetX = p.rect.left - 800 / 3;


		window.clear(Color::White);
		window.draw(backgroundSprite);
		window.draw(far_buildings);					//    Отрисовка спрайтов
		window.draw(buildings);
		window.draw(near_buildings);

		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 'B') tile.setTextureRect(IntRect(176, 16, 64, 40));

				if (TileMap[i][j] == 'H') tile.setTextureRect(IntRect(272, 16, 64, 40));
				//		Отрисовка текстур
				if (TileMap[i][j] == ' ' || TileMap[i][j] == 'W') continue;

				tile.setPosition(j * 32 - offsetX, i * 32 - offsetY);
				rectangle.setPosition(j * 32 - offsetX, i * 32 - offsetY);
				window.draw(rectangle);
				window.draw(tile);
			}

		window.draw(p.sprite);
		window.draw(enemy.sprite);
		window.display();

	}
	return 0;
}
