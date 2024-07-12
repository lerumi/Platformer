#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
using namespace sf;
int Menu(RenderWindow& window)
{
	Image menuImage1, menuImage2, menuImage3;
	menuImage1.loadFromFile("images/text2.png");
	menuImage2.loadFromFile("images/text4.png");
	menuImage3.loadFromFile("images/text3.png");

	menuImage1.createMaskFromColor(Color(0, 0, 0));
	menuImage2.createMaskFromColor(Color(0, 0, 0));
	menuImage3.createMaskFromColor(Color(0, 0, 0));
	Texture menuTexture1, menuTexture2, menuTexture3, menuBackground;
	menuTexture1.loadFromImage(menuImage1);
	menuTexture2.loadFromImage(menuImage2);
	menuTexture3.loadFromImage(menuImage3);

	menuBackground.loadFromFile("images/menu.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(170, 150);
	menu2.setPosition(180, 250);
	menu3.setPosition(200, 380);

	menu1.setScale(0.4f, 0.4f);
	menu2.setScale(0.4f, 0.4f);
	menu3.setScale(0.4f, 0.4f);

	menuBg.setPosition(4, 0);
	menuBg.setScale(0.5f, 0.5f);

	Color color(255, 221, 46);
	Color color1(189, 136, 83);

	while (isMenu)
	{
		menu1.setColor(color1);
		menu2.setColor(color1);
		menu3.setColor(color1);
		menuNum = 0;
		window.clear();
		if (IntRect(160, 150, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(color); menuNum = 1; }
		if (IntRect(180, 250, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(color); menuNum = 2; }
		if (IntRect(200, 380, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(color); menuNum = 3; }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) return 1;
			if (menuNum == 2) { window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { return 0; }

		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
}