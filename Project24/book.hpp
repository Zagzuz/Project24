#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include "fText.hpp"
#include "skill_slot.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

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
	skill* draw_me(sf::RenderWindow& w, sf::Event& e, const ChType mana, ChType& current_mana) const;
	//void left_click(sf::RenderWindow& w, const sf::Vector2f& mouse_pos) const;
	//skill* get_skill(const sf::Vector2f& mouse_pos);
	std::string save_info() const;
	void load_info(std::ifstream& file_stream);
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

inline skill* book::draw_me(sf::RenderWindow& w, sf::Event& e, const ChType mana, ChType& current_mana) const
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

	fText mana_text(std::to_string(int(current_mana)) + "/" + std::to_string(int(mana)), 
		"Fonts/04b30.ttf", 15, sf::Color::Blue, sf::Color::White, 0.7f);
	mana_text.setPosition
	(
		float(w.getSize().x) * 0.8f - mana_text.findCharacterPos(mana_text.getString().getSize() - 1).x + 25,
		float(w.getSize().y) * 0.8f + mana_text.getCharacterSize()
	);

	w.draw(mana_text);

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
		w.pollEvent(e);
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			return nullptr;

		const sf::Vector2f slot_pos{ start_x + i * interval_x, start_y + j * interval_y};
		const auto mouse_x = sf::Mouse::getPosition(w).x;
		const auto mouse_y = sf::Mouse::getPosition(w).y;

		if (mouse_x >= slot_pos.x &&
			mouse_x <= slot_pos.x + slot_size.x &&
			mouse_y >= slot_pos.y &&
			mouse_y <= slot_pos.y + slot_size.x)
		{
			it->highlight_me(w, slot_pos, slot_size);
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
				if (it->get_skill()->get_mana_cost() <= current_mana)
				{
					current_mana -= it->get_skill()->get_mana_cost();
					e = {};
					return it->get_skill();
				}
		}
		else it->draw_me(w, slot_pos, slot_size);

		++it;

		if (i == 5)
		{
			i = 0;
			++j;
		}
		else ++i;
	}

	return nullptr;
}

inline std::string book::save_info() const
{
	std::ostringstream oss;
	oss << skill_slots_.size() << ' ';
	for (auto& slot : skill_slots_)
		oss << slot.save_info();
	return oss.str();
}

inline void book::load_info(std::ifstream& file_stream)
{
	size_t size;
	file_stream >> size;
	while (size--)
	{
		skill_slots_.emplace_back(skill_slot(new skill()));
		skill_slots_.back().load_info(file_stream);
	}
}
