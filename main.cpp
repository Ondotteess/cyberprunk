#include <SFML/Graphics.hpp>
#include <iostream>
#include <assert.h>

float fall = 0;

bool alive = true;

using namespace sf;
using namespace std;

float speed_x = 0.15;
float speed_y = -0.4;

int timeset = 400;

const int H = 16;				 //		Настройки игрового процесса
const int W = 80;

const int BD_HEIGHT = 150;

float n_buildings_speed = 0.1f;
float m_buildings_speed = 0.05f;
float f_buildings_speed = 0.01f;


float offsetX, offsetY;

String TileMap[H] = {
	"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",
	"W                                                                              W",		  //		Расположение плиток
	"W            10   2222222222                                                   W",
	"W            15        1   R                                                   W",
	"W            1         1   R                                                   W",
	"W            1 4       1   R      R222222222                                   W",
	"W            12222222221   R      R5       1                                   W",
	"W            1       3 1   R      R        1                                   W",
	"W            1         1   R      R        1                                   W",
	"W            1         1   R      R        1                                   W",
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
		fall = 0;
	}

	void update(float time) {
		rect.left += dx * time;
		Collision(0);
		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);
		if (alive) {
			currentFrame += 0.005 * time;
			if (currentFrame > 10) currentFrame -= 8;

			if (dx > 0) { sprite.setTextureRect(IntRect(45 * int(currentFrame), 295, 40, 40)); last_dir = 'r'; };
			if (dx < 0) { sprite.setTextureRect(IntRect(45 * int(currentFrame) + 40, 295, -40, 40)); last_dir = 'l'; };
			if (!dx && last_dir == 'r') sprite.setTextureRect(IntRect(45, 25, 30, 40));
			if (!dx && last_dir == 'l') sprite.setTextureRect(IntRect(45 + 30, 25, -30, 40));
			if (dy != 0 && last_dir == 'r') sprite.setTextureRect(IntRect(40, 650, 30, 40));
			if (dy != 0 && last_dir == 'l') sprite.setTextureRect(IntRect(40 + 30, 650, -30, 40));
		}
		if (!alive) {
			fall += 0.004 * time;

			if (int(fall) == 0) { sprite.setTextureRect(IntRect(40, 870, 35, 40)); rect.left -= speed_x * time; rect.top += (speed_y * time) / 2; }
			if (int(fall) == 1) { sprite.setTextureRect(IntRect(75, 865, 35, 40)); rect.left -= speed_x * time; rect.top += (speed_y * time) / 2; }
			if (int(fall) == 2) { sprite.setTextureRect(IntRect(112, 865, 45, 35)); rect.left -= speed_x * time; rect.top += (speed_y * time) / 2; }
			if (int(fall) == 3)  sprite.setTextureRect(IntRect(158, 870, 40, 35));
			if (int(fall) == 4) sprite.setTextureRect(IntRect(202, 870, 40, 40));
			if (int(fall) == 5) sprite.setTextureRect(IntRect(245, 870, 45, 40));
			if (int(fall) == 6) sprite.setTextureRect(IntRect(295, 870, 45, 40));
			if (int(fall) == 7) sprite.setTextureRect(IntRect(350, 870, 45, 40));
			if (int(fall) == 8) sprite.setTextureRect(IntRect(420, 870, 45, 40));
			if (int(fall) == 9) sprite.setTextureRect(IntRect(485, 870, 45, 40));
			if (int(fall) == 10) sprite.setTextureRect(IntRect(550, 870, 45, 40));
			if (int(fall) == 11) sprite.setTextureRect(IntRect(615, 870, 45, 40));
			if (fall > 11) { alive = true; fall = 0; }
		}
		sprite.setPosition(rect.left - offsetX, rect.top);

		dx = 0;
	}

	void Collision(int dir) {
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++) {
				if (TileMap[i][j] == 'B' || TileMap[i][j] == 'H' || TileMap[i][j] == 'W' || \
					TileMap[i][j] == '2' || TileMap[i][j] == 'R') {																				//   Коллизию делает
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height; dy = 0; onGround = true; };
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32; dy = 0; }

					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;

					assert(i < H);
					assert(j < W);
				}

				if (TileMap[i][j] == '4') TileMap[i][j] = ' ';
			}
	}
	void set(Texture& image, int x, int y) {
		sprite.setTexture(image);
		rect = FloatRect(x, y, 32, 32);

		dx = 0.1;
		currentFrame = 0;
		alive = true;
	}


};

class ENEMY {
public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool life, isBeating;

	void set(Texture& image, int x, int y) {
		sprite.setTexture(image);
		rect = FloatRect(x, y, 32, 32);

		dx = 0.05;
		currentFrame = 0;
		life = true;
	}

	void update(float time, PLAYER p) {
		char last_dir = 'r';
		sprite.setScale(1, 1);
		rect.left += dx * time;

		Collision();

		if ((abs(rect.top - p.rect.top) < 20) && (abs(rect.left - p.rect.left) < 32 * 10)) {
			if (rect.left > p.rect.left) dx = -0.05;
			else dx = 0.05;
		} 

		currentFrame += time * 0.008;
		if (currentFrame > 8) currentFrame -= 7;

		if (dx > 0) { sprite.setTextureRect(IntRect(10 + 55 * int(currentFrame), 155, 55, 50)); last_dir = 'r'; }
		if (dx < 0) { sprite.setTextureRect(IntRect(10 + 55 * int(currentFrame) + 55, 155, -55, 50)); last_dir = 'l'; }


		if (abs(rect.left - p.rect.left) <= 70 && abs(rect.top - p.rect.top) < 20) {
			isBeating = true;
			if (dx > 0) { sprite.setScale(1, 1); }
			else if (dx < 0 && isBeating) { sprite.setScale(-1, 1); };
			if (int(currentFrame) == 0) sprite.setTextureRect(IntRect(53, 210, -55, 50));
			if (int(currentFrame) == 1) sprite.setTextureRect(IntRect(105, 210, -50, 50));
			if (int(currentFrame) == 2) sprite.setTextureRect(IntRect(170, 210, -50, 50));
			if (int(currentFrame) == 3) sprite.setTextureRect(IntRect(242, 210, -60, 50));
			if (int(currentFrame) == 4) sprite.setTextureRect(IntRect(318, 210, -75, 50));
			if (int(currentFrame) == 5) sprite.setTextureRect(IntRect(441, 210, -80, 50));
			if (int(currentFrame) == 6) sprite.setTextureRect(IntRect(526, 210, -50, 50));
			if (int(currentFrame) == 7) {
				sprite.setTextureRect(IntRect(579, 210, -50, 50));
				if (abs(rect.top - p.rect.top) <= 20) { alive = false; p.update(time); };

			}
			dx = 0;
		}
		else isBeating = false;

		if (isBeating && last_dir == 'l') sprite.setPosition(rect.left - offsetX + 35, rect.top - offsetY);
		else sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

	}
	void Collision() {
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++) {
				if ((TileMap[i][j] == 'W' || TileMap[i][j] == 'R' || TileMap[i][j] == '1')) {
					if (dx > 0) { rect.left = j * 32 - rect.width; dx *= -1; }
					else if (dx < 0) { rect.left = j * 32 + 32;  dx *= -1; }
				}

			}
	}
};


int main() {
	RenderWindow window(VideoMode(1080, 720), "Test");

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

	Texture tileset1;
	if (!tileset1.loadFromFile("./image/tileset1.png")) {
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
	Sprite tile1(tileset1);

	float currentFrame = 0;

	PLAYER p(t);
	ENEMY enemy_1;
	ENEMY enemy_2;
	enemy_1.set(enemy1, 500, 300);
	enemy_2.set(enemy1, 500, 440);
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

		enemy_1.update(time, p);
		enemy_2.update(time, p);

		if (p.rect.left > 800 / 2) offsetX = p.rect.left - 800 / 2;


		window.clear(Color::White);
		window.draw(backgroundSprite);
		window.draw(far_buildings);					//    Отрисовка спрайтов
		window.draw(buildings);
		window.draw(near_buildings);

		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == '0') { tile1.setTextureRect(IntRect(65, 65, 10, 10)); tile1.setScale(45, 45); tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile1); }
				if (TileMap[i][j] == '1') { tile1.setTextureRect(IntRect(144, 21, 32, 68)); tile1.setScale(1, 1); tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile1); }
				if (TileMap[i][j] == '2') { tile1.setTextureRect(IntRect(335, 15, 32, 32)); tile1.setScale(2, 2); tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile1); }
				if (TileMap[i][j] == '3') { tile1.setTextureRect(IntRect(224, 176, 47, 117)); tile1.setScale(1.4, 1.5); tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile1); }
				if (TileMap[i][j] == '4') {
					tile1.setTextureRect(IntRect(105, 175, 45, 15));
					tile1.setScale(1, 1);
					tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY + 20);
					window.draw(tile1);

					tile1.setTextureRect(IntRect(323, 96, 22, 88));
					tile1.setScale(1, 1);
					tile1.setPosition(j * 32 - offsetX + 17, i * 32 - offsetY - 15);
					window.draw(tile1);
				}
				if (TileMap[i][j] == '5') { tile1.setTextureRect(IntRect(160, 96, 46, 30)); tile1.setScale(3, 3.3); tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile1); }


				if (TileMap[i][j] == 'R') { tile1.setTextureRect(IntRect(15, 15, 15, 50)); tile1.setScale(2, 1); tile1.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile1); }

				if (TileMap[i][j] == 'B') { tile.setTextureRect(IntRect(176, 16, 64, 40)); tile.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile); }

				if (TileMap[i][j] == 'H') { tile.setTextureRect(IntRect(272, 16, 64, 40)); tile.setPosition(j * 32 - offsetX, i * 32 - offsetY); window.draw(tile); }

				if (TileMap[i][j] == ' ' || TileMap[i][j] == 'W') continue;

			}

		window.draw(p.sprite);
		window.draw(enemy_1.sprite);
		window.draw(enemy_2.sprite);
		window.display();

	}
	return 0;
}
