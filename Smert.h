#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
using namespace sf;
class for_text_sprite
{
	Image Image1;
	Texture Texture1;
	
public:
	Sprite sprite1;
	for_text_sprite(Image& I, int x, int y)
	{
		Image1 = I;
		Image1.createMaskFromColor(Color(0, 0, 0));
		Texture1.loadFromImage(Image1);
		sprite1.setTexture(Texture1);
		sprite1.setPosition(x, y);
	}
	friend int smert(RenderWindow& window, int k);
	void changeColor(int r, int g, int b)
	{
		Color color(r, g, b);
		sprite1.setColor(color);
	}
};
int smert(RenderWindow& window, int k, int playerScore, int Star_score)
{
	Image Image1, Image2, Image3, Image4;
	Texture menuBackground;
	Font font;//רנטפע 
	font.loadFromFile("images/Samson.ttf");
	Text text("", font, 48);
	std::ostringstream playerScoreString;
	playerScoreString << playerScore;
	text.setString(playerScoreString.str());
	//text.setColor(Color::Red);
	//text.setStyle(Text::Bold);
	
	text.setPosition(700, 570);
	int x, y;
	if (k == -2)
	{
		menuBackground.loadFromFile("images/loser.png");
		Image1.loadFromFile("images/lose.png");
		Image2.loadFromFile("images/score.png");
		Image3.loadFromFile("images/text_menu.png");
		Image4.loadFromFile("images/restart.png");
		x = 520; y = 300;
	}
	else
	{
		menuBackground.loadFromFile("images/win.png");
		Image1.loadFromFile("images/win_text.png");
		Image2.loadFromFile("images/score.png");
		Image3.loadFromFile("images/text_menu.png");
		Image4.loadFromFile("images/next.png");
		x = 580; y = 280;
	}

	Sprite menuBg(menuBackground);
	for_text_sprite sostoianie(Image1, x, y), score(Image2, 490, 580), text_menu(Image3, 200, 700), next(Image4, 930, 700);
	bool isMenu = 1;
	int menuNum = 0;

	sostoianie.sprite1.setScale(0.4f, 0.4f);
	score.sprite1.setScale(0.4f, 0.4f);
	text_menu.sprite1.setScale(0.4f, 0.4f);
	next.sprite1.setScale(0.4f, 0.4f);

	menuBg.setPosition(4, 0);

	Color color(255, 221, 46);
	Color color1(189, 136, 83);
	score.sprite1.setColor(color);
	text.setFillColor(color);
	sostoianie.sprite1.setColor(color);

	Image Image_star;
	Image_star.loadFromFile("images/Star_bolsha.png");
	for_text_sprite Stars(Image_star, 500, 300);
	


	while (isMenu)
	{
		text_menu.sprite1.setColor(color1);
		next.sprite1.setColor(color1);
		menuNum = 0;
		window.clear();
		if (IntRect(200, 700, 300, 50).contains(Mouse::getPosition(window))) { text_menu.sprite1.setColor(color); menuNum = 1; }
		if (IntRect(930, 700, 300, 50).contains(Mouse::getPosition(window))) { next.sprite1.setColor(color); menuNum = 2; }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) return 1;
			if (menuNum == 2 && k == -2) { return -1; }
			//else { return 0; }
		}

		window.draw(menuBg);




		if (Star_score >= 1)
		{
			Stars.sprite1.setPosition(600, 400);
			window.draw(Stars.sprite1);
		}
		if (Star_score >= 2)
		{
			Stars.sprite1.setPosition(700, 400);
			window.draw(Stars.sprite1);
		}
		if (Star_score == 3)
		{
			Stars.sprite1.setPosition(800, 400);
			window.draw(Stars.sprite1);
		}

		
		window.draw(text);

		window.draw(sostoianie.sprite1);
		window.draw(score.sprite1);
		window.draw(text_menu.sprite1);
		window.draw(next.sprite1);
		window.display();
	}
}