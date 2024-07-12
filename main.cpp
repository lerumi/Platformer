#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "level.h"
#include "menu.h"
#include "Smert.h"
#include <vector>
#include <list>


using namespace sf;
float CurrentFrame = 0;
int playerScore, Star_score;
class Game {
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health, zone=0;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Game(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 3; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	virtual void update(float time, int schetchik) = 0;
	virtual void stolknovenie() = 0;

	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
};
class Player :public Game {
private:
	bool OnLadder;
	enum { left, right, jump, stay, climb } state;
public:
	
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Game(image, Name, X, Y, W, H) {
		playerScore = Star_score = 0; state = stay; obj = lev.GetAllObjects();
		if (name == Name) {
			sprite.setTextureRect(IntRect(4, 19, w, h));
		}
		OnLadder = 0;
	}
	void stolknovenie() {};
	void control(float time) {
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				state = right; speed = 0.1; 
			}
			if ((Keyboard::isKeyPressed(Keyboard::W)) && (onGround)) {
				state = jump; dy = -0.6; onGround = false;
			}
			else if ((Keyboard::isKeyPressed(Keyboard::W)) && (OnLadder)) {
				state = climb;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "win")
				{
					health = -1;
					break;
				}
				if (obj[i].name == "gr")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
				if (obj[i].name == "stair")
				{
					OnLadder = 1;
				}
				if (obj[i].name == "lovu")
				{
					life = false;
					//std::cout<<"0";
				}
				if (obj[i].name == "zone_stolknovenii")
				{
					zone = 1;
				}
				else if (obj[i].name != "zone_stolknovenii")
				{
					zone = 0;
					//std::cout << 1;
				}
				
				
			}
	}
	void animation(float time)
	{
		if ((state == stay || state == jump) && OnLadder == 0)
		{
			sprite.setTextureRect(IntRect(10, 0, 60, 80));
		}
		else if (state == right)
		{
			CurrentFrame += 0.006 * time;
			if (CurrentFrame > 5) CurrentFrame -= 5;
			sprite.setTextureRect(IntRect(50 * int(CurrentFrame) + 70, 0, 49, 79));
			sprite.setScale(dx/abs(speed), 1);
		}
		else if (state == left)
		{
			CurrentFrame += 0.004 * time;
			if (CurrentFrame > 5) CurrentFrame -= 5;
			sprite.setTextureRect(IntRect(50 * int(CurrentFrame) + 70, 0, 49, 80));
			sprite.setScale(dx / abs(speed), 1);

		}
		
		if (state == climb && OnLadder == 1)
		{
			sprite.setTextureRect(IntRect(10, 280, 60, 80));
		}
		
	}
	void update(float time, int schetchik)
	{
		OnLadder = 0;
		state = stay;
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		control(time);
		
		switch (state)
		{
		case right:dx = speed; break;
		case left:dx = -speed; break;
		case stay: dx = 0; break;
		case climb: dy = -0.2; break;
		}
		animation(time);
		
		sprite.setPosition(x + w / 2 + 15, y + h / 2-20);
		if (health == 0) { life = false; }
		if (!isMove) { speed = 0; }
		dy = dy + 0.0015 * time;
		
		
	}
};
class Enemy :public Game {
private:
	std::vector<Object> obj_2;
public:
	float CurrentFrame_enemy = 0;
	enum { left, right, atack } state;
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int for_rect_x, int for_rect_y, String obj_name) :Game(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects(obj_name);
		obj_2 = lvl.GetObjects("zone_stolknovenii");
		if (name == Name) {
			sprite.setTextureRect(IntRect(for_rect_x, for_rect_y, w, h));
			sprite.setScale(4.0f, 4.0f);
			dx = 0.04;
		}
	}
	void stolknovenie() {};
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.06; sprite.scale(-1, 1); state = right; }
				if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.06; sprite.scale(-1, 1); state = left;}
			}

	}
	void animation(float time, int schetchik)
	{
		if (state == right || state==left)
		{
			CurrentFrame_enemy += 0.002 * time * schetchik;
			if (CurrentFrame_enemy > 5) CurrentFrame_enemy -= 5;
			sprite.setTextureRect(IntRect(18 * int(CurrentFrame_enemy)+27, 17, 18, 17));
			//sprite.setScale(dx / abs(speed), 1);

		}

		if (state == atack)
		{
			sprite.setTextureRect(IntRect(10, 280, 60, 80));
		}

	}
	void update(float time, int schetchik)
	{
		checkCollisionWithMap(dx, 0);
		animation(time, schetchik);
		x += dx * time * schetchik;
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
	}
}; 

class Enemy_bow : public Enemy
{
public:
	Enemy_bow(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int for_rect_x, int for_rect_y, String obj_name) :Enemy(image, Name, lvl, X, Y, W, H, for_rect_x, for_rect_y, obj_name){ sprite.scale(-1, 1); }

	void stolknovenie() {};
	void update(float time, int schetchik)
	{
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		CurrentFrame_enemy += 0.002 * time * schetchik;
		if (CurrentFrame_enemy > 4) CurrentFrame_enemy -= 4;
		sprite.setTextureRect(IntRect(18 * int(CurrentFrame_enemy)+28, 17*7+10, 17, 17));
		
	}
};

class Arrow :public Game {
public:
	int direction;
	int x_ish, y_ish;
	Arrow(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Game(image, Name, X-50, Y, W, H) {
		obj = lvl.GetObjects("solid");
		x_ish = X-50;
		y_ish= Y;
		direction = dir;
		speed = 0.25;
		w = h = 1;
		life = true;
		sprite.setScale(3.0f, 3.0f);
	}

	void stolknovenie()
	{
		x = x_ish;
		y = y_ish;
		sprite.setPosition(x + w / 2, y + h / 2);
	}
	void update(float time, int schetchik)
	{
		int k = 0;
		switch (direction)
		{
		case 0: dx = -speed; dy = 0; break;
		case 1: dx = speed; dy = 0;   break;
		}

		x += dx * time;
		y += dy * time;

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;
		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(obj[i].rect))
			{
				stolknovenie();
				k++;
			}
		}
		if (k == 0)
		{
			sprite.setPosition(x + w / 2, y + h / 2);
		}
		
	}
};
class Coins : public Game
{
public:
	Coins(Image& image, String Name, Level& lev, float X, float Y, int W, int H, int for_rect_x, int for_rect_y) :Game(image, Name, X, Y, W, H) {
		if (name == Name) {
			sprite.setTextureRect(IntRect(for_rect_x, for_rect_y, w, h));
			sprite.setScale(0.6f, 0.6f);
		}
	}
	void stolknovenie() {};
	void update(float time, int schetchik)
	{
		//y -= 00.2 * time;
		sprite.setPosition(x, y);
	}
};
int Start(RenderWindow& window)
{
	Level lvl;
	lvl.LoadFromFile("lvl23.tmx");

	Image heroImage;
	heroImage.loadFromFile("images/player.png");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemies.png");

	Image ArrowImage;
	ArrowImage.loadFromFile("images/Arrows_2.png");

	Image Coin;
	Coin.loadFromFile("images/money.png");

	Image Star;
	Star.loadFromFile("images/Star.png");
	
	Object player = lvl.GetObject("Pl");
	Object bow_1 = lvl.GetObject("enemy_bow");

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 60, 60);

	std::list<Game*>  entities;
	std::list<Game*>::iterator it;

	Image Image_ser;
	Image_ser.loadFromFile("images/serdeshko.png");
	Image_ser.createMaskFromColor(Color(0, 0, 0));
	Texture Texture1;
	Texture1.loadFromImage(Image_ser);
	Sprite sprite1;
	sprite1.setTexture(Texture1);

	Image Image_win;
	Image_win.loadFromFile("images/door.png");
	//Image_win.createMaskFromColor(Color(0, 0, 0));
	Texture Texture2;
	Texture2.loadFromImage(Image_win);
	Sprite sprite2;
	sprite2.setTexture(Texture2);
	

	std::vector<Object> e = lvl.GetObjects("enemy_sword");
	std::vector<Object> e1 = lvl.GetObjects("coin");
	std::vector<Object> e2 = lvl.GetObjects("Star");

	Enemy_bow bow_2(easyEnemyImage, "enemy_bow", lvl, bow_1.rect.left, bow_1.rect.top, 20, 17, 0, 109, "Pl");
	for (int i = 0; i < e.size(); i++)
	{
		entities.push_back(new Enemy(easyEnemyImage, "enemy_sword", lvl, e[i].rect.left, e[i].rect.top, 20, 17, 0, 18, "solid"));
		
	}
	for (int i = 0; i < e1.size()+e2.size(); i++)
	{
		if(i>=e1.size()) entities.push_back(new Coins(Star, "stars", lvl, e2[i-e1.size()].rect.left, e2[i-e1.size()].rect.top, 30, 35, 0, 0));
		else entities.push_back(new Coins(Coin, "coins", lvl, e1[i].rect.left, e1[i].rect.top, 30, 20, 0, 0));
	}

	entities.push_back(new Arrow(ArrowImage, "arrow", lvl, bow_1.rect.left, bow_1.rect.top, 0, 0, 0));

	Clock clock;
	

	playerScore = Star_score = 0;
	while (window.isOpen())
	{
		if (p.health == 0 || p.life == false) { sleep(seconds(0.1)); return -2; }
		if (p.health == -1) { sleep(seconds(0.1)); return -1; }
		int schetchik = 1;
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		p.update(time, 1);
		bow_2.update(time, 1);
		window.clear();
		lvl.Draw(window);
		window.draw(bow_2.sprite);
		sprite2.setPosition(1000, 350);
		sprite2.setScale(0.4f, 0.4f);
		window.draw(sprite2);
		window.draw(p.sprite);
		if (p.health >= 1)
		{
			sprite1.setPosition(100, 10);
			window.draw(sprite1);
		}
		if (p.health >= 2)
		{
			sprite1.setPosition(150, 10);
			window.draw(sprite1);
		}
		if (p.health == 3)
		{
			sprite1.setPosition(200, 10);
			window.draw(sprite1);
		}

		if (p.zone == 1) schetchik = 2;
		else schetchik = 1;

		

		for (it = entities.begin(); it != entities.end();)
		{
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "coins") { playerScore += 1;  it = entities.erase(it); }
				else if ((*it)->name == "stars") { Star_score += 1;  it = entities.erase(it); }
			}
			(*it)->update(time, schetchik);
			if ((*it)->life == false) { it = entities.erase(it); }
			else it++;
		}

		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "enemy_sword") {

					if ((p.dy > 0) && (p.onGround == false)) { (*it)->dx = 0; p.dy = -0.2; (*it)->health = 0; }
					else {
						p.health -= 1;
					}
					if ((*it)->dx > 0)
					{
						(*it)->dx *= -1;
						(*it)->sprite.scale(-1, 1);
					}
					else if ((*it)->dx < 0)
					{
						(*it)->dx *= -1;
						(*it)->sprite.scale(-1, 1);
					}

				}
				if ((*it)->name == "arrow") {
					(*it)->stolknovenie();
					p.health -= 1;
				}

			}
			window.draw((*it)->sprite);
		}

		window.display();
	}
	
}

int main()
{
	int k=1;
	RenderWindow window(sf::VideoMode(40*32, 25*32), "platformer");
	
	while (k != 0)
	{	
		if (k == 1)
		{
			k = Menu(window);
			if (k != 0)
			{
				k = Start(window);
			}
		}
		if (k <= -1)
		{
			k = smert(window, k, playerScore, Star_score);
			if (k == -1)
			{
				k = Start(window);
			}
		}
	}
	window.close();
}