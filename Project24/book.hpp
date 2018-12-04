#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include "fText.hpp"
#include "skill_slot.hpp"
#include <SFML/Window/Mouse.hpp>

//_____  _____
//|_|_|  |_|_|
//|_|_|  |_|_|
//|_|_|  |_|_|
//|_|_|  |_|_|


class book
{
	sf::Texture texture_, table_;
	std::list<skill_slot> skill_slots_;
public:
	book();
	book(std::initializer_list<skill*> skills);
	void add_skill(skill* s) { skill_slots_.emplace_back(skill_slot(s)); }
	void draw_me(sf::RenderWindow& w) const;
	void left_click(sf::RenderWindow& w, const sf::Vector2f& mouse_pos) const;
	skill* get_skill(const sf::Vector2f& mouse_pos);
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
	for (auto s : skills)
		add_skill(s);
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

	const float x_coeff = float(w.getSize().x) / w.getSize().y;
	const float y_coeff = float(w.getSize().y) / w.getSize().x;

	const float start_x = float(w.getSize().x) / 10 * x_coeff, 
 		interval_x = float(w.getSize().x) / 15.5f * x_coeff;
	const float start_y = float(w.getSize().y) / 4 * y_coeff,
		interval_y = float(w.getSize().y) / 4 * y_coeff;

	const sf::Vector2f slot_size
	{
		float(w.getSize().x) / 15, 
		float(w.getSize().x) / 15
	};
	
	auto it = skill_slots_.begin();
	uint8_t i = 0, j = 0;
	while (i <= 6 && j <= 4 && it != skill_slots_.end())
	{
		const sf::Vector2f slot_pos{ start_x + i * interval_x, start_y + j * interval_y};

		const auto mouse_x = sf::Mouse::getPosition(w).x;
		const auto mouse_y = sf::Mouse::getPosition(w).y;

		if (mouse_x >= slot_pos.x &&
			mouse_x <= slot_pos.x + slot_size.x &&
			mouse_y >= slot_pos.y &&
			mouse_y <= slot_pos.y + slot_size.x)
		{
			it->highlight_me(w, slot_pos, slot_size);
		}
		else
		{
			it->draw_me(w, slot_pos, slot_size);
		}

		++it;

		if (i == 5)
		{
			i = 0;
			++j;
		}
		else ++i;
	}
}

inline void book::left_click(sf::RenderWindow& w, const sf::Vector2f& mouse_pos) const
{
	
}

inline skill* book::get_skill(const sf::Vector2f& mouse_pos)
{

}
