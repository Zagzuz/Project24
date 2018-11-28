#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "typed_skill.hpp"
#include <list>
#include <SFML/Graphics/RectangleShape.hpp>
#include "fText.hpp"

class book
{
	sf::Texture texture_, table_;
	std::list<skill*> skills_;
public:
	book();
	book(std::initializer_list<skill*> skills);
	void add_skill(skill* s) { skills_.push_back(s); }
	void draw_me(sf::RenderWindow& w) const;
};

inline book::book()
{
	if (!texture_.loadFromFile("Images/book.png"))
		std::cerr << "Error book texture load";
	if (!table_.loadFromFile("Images/wizards_table.png"))
		std::cerr << "Error wizard's table texture load";
}

inline book::book(std::initializer_list<skill*> skills)
{
	for (auto& s : skills)
		skills_.push_back(s);
	if (!texture_.loadFromFile("Images/book.png"))
		std::cerr << "Error book texture load";
	if (!table_.loadFromFile("Images/wizards_table.png"))
		std::cerr << "Error wizard's table texture load";
}

inline void book::draw_me(sf::RenderWindow& w) const
{
	sf::Sprite table_sprite;
	table_sprite.setTexture(table_);

	table_sprite.setScale
	(
		float(w.getSize().x) / table_.getSize().x,
		float(w.getSize().y) / table_.getSize().y
	);

	w.draw(table_sprite);

	sf::Sprite sprite;
	sprite.setTexture(texture_);

	sprite.setScale
	(
		float(w.getSize().x) / texture_.getSize().x * 0.8f, 
		float(w.getSize().y) / texture_.getSize().y * 0.8f
	);

	const auto sprite_x_size = texture_.getSize().x * sprite.getScale().x;
	const auto sprite_y_size = texture_.getSize().y * sprite.getScale().y;

	sprite.setPosition
	(
		float(w.getSize().x) / 2 - sprite_x_size / 2,
		float(w.getSize().y) / 2 - sprite_y_size / 2
	);

	w.draw(sprite);

	fText ft("Escape to close book", "Fonts/04b30.ttf", 20, sf::Color::Transparent, sf::Color::White, 0.5);
	ft.setPosition(float(w.getSize().x) / 50, float(w.getSize().y) / 50);
	w.draw(ft);

	/*fText next("-> to open next page", "Fonts/04b30.ttf", 20, sf::Color::Transparent, sf::Color::White, 0.5);
	next.setPosition
	(
		w.getSize().x - next.findCharacterPos(20).x - float(w.getSize().x) / 50,
		w.getSize().y - next.getCharacterSize() - float(w.getSize().y) / 50
	);
	w.draw(next);*/

	float start_x = 260;
	const float start_y = 130;
	const float	x_coeff = 150, y_coeff = 100;

	uint8_t i = 0, j = 0;
	for (auto* s : skills_)
	{
		s->draw_me(w, { start_x + i * x_coeff, start_y + j * y_coeff });
		if (i++ == 5)
		{
			if (j++ == 4) break;
			i = 0;
			start_x -= 30;
		}
		else if (i == 3) start_x += 30;
	}
}
