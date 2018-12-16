#pragma once

#include "skill.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class skill_slot
{
	std::unique_ptr<skill> s_;
	sf::Texture texture_;
public:
	explicit skill_slot(skill* s);
	void draw_me(sf::RenderWindow& w, const sf::Vector2f& pos, const sf::Vector2f& size) const;
	void highlight_me(sf::RenderWindow& w, const sf::Vector2f& pos, const sf::Vector2f& size) const;
	skill* get_skill() const { return s_.get(); }
	std::string save_info() const { return s_->save_info(); }
	void load_info(std::ifstream& file_stream) { s_->load_info(file_stream); }
};

inline skill_slot::skill_slot(skill* s)
{
	s_.reset(s);
	if (!texture_.loadFromFile("Images/circle.png"))
		std::cerr << "Error slot texture load on path";
}

inline void skill_slot::draw_me(sf::RenderWindow& w, const sf::Vector2f& pos, const sf::Vector2f& size) const
{
	sf::Sprite sprite;
	sprite.setTexture(texture_);
	sprite.setScale
	(
		size.x / texture_.getSize().x * 8 / 9,
		size.y / texture_.getSize().y * 8 / 9
	);
	sprite.setPosition(pos);
	w.draw(sprite);

	sf::Sprite skill_sprite;
	skill_sprite.setTexture(s_->get_texture());
	skill_sprite.setScale
	(
		size.x / s_->get_texture().getSize().x / 1.8,
		size.y / s_->get_texture().getSize().y / 1.8
	);
	skill_sprite.setPosition
	(
		pos.x + size.x / 2 - size.x / 3,
		pos.y + size.y / 2 - size.y / 3
	);
	w.draw(skill_sprite);

	fText mana_cost(std::to_string(int(s_->get_mana_cost())), 
		"Fonts/04b30.ttf", 15, sf::Color::Blue, sf::Color::White, 1.5f);
	mana_cost.setPosition
	(
		pos.x + size.x / 2 - size.x / 3,
		pos.y + size.y / 2 - size.y / 3
	);
	w.draw(mana_cost);
}

inline void skill_slot::highlight_me(sf::RenderWindow& w, const sf::Vector2f& pos, const sf::Vector2f& size) const
{
	sf::Sprite sprite;
	sprite.setTexture(texture_);
	sprite.setScale
	(
		size.x / texture_.getSize().x * 8 / 9,
		size.y / texture_.getSize().y * 8 / 9
	);
	sprite.setPosition(pos);
	sprite.setColor(sf::Color::Yellow);
	w.draw(sprite);

	sf::Sprite skill_sprite;
	skill_sprite.setTexture(s_->get_texture());
	skill_sprite.setScale
	(
		size.x / s_->get_texture().getSize().x / 1.8,
		size.y / s_->get_texture().getSize().y / 1.8
	);
	skill_sprite.setPosition
	(
		pos.x + size.x / 2 - size.x / 3,
		pos.y + size.y / 2 - size.y / 3
	);
	w.draw(skill_sprite);


	fText mana_cost(std::to_string(int(s_->get_mana_cost())),
		"Fonts/04b30.ttf", 15, sf::Color::Blue, sf::Color::White, 1.5f);
	mana_cost.setPosition
	(
		pos.x + size.x / 2 - size.x / 3,
		pos.y + size.y / 2 - size.y / 3
	);
	w.draw(mana_cost);
}
