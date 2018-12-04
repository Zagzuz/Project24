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
};

inline skill_slot::skill_slot(skill* s)
{
	s_.reset(s);
	if (!texture_.loadFromFile("Images/skill_slot_cell.png"))
		std::cerr << "Error slot texture load on path";
}

inline void skill_slot::draw_me(sf::RenderWindow& w, const sf::Vector2f& pos, const sf::Vector2f& size) const
{
	sf::Sprite sprite;
	sprite.setTexture(texture_);
	sprite.setScale
	(
		size.x / texture_.getSize().x,
		size.y / texture_.getSize().y
	);
	sprite.setPosition(pos);
	w.draw(sprite);

	sf::Sprite skill_sprite;
	skill_sprite.setTexture(s_->get_texture());
	skill_sprite.setScale
	(
		size.x / s_->get_texture().getSize().x / 1.5,
		size.y / s_->get_texture().getSize().y / 1.5
	);
	skill_sprite.setPosition
	(
		pos.x + size.x / 2 - size.x / 3,
		pos.y + size.y / 2 - size.y / 3
	);
	w.draw(skill_sprite);
}

inline void skill_slot::highlight_me(sf::RenderWindow& w, const sf::Vector2f& pos, const sf::Vector2f& size) const
{
	sf::Sprite sprite;
	sprite.setTexture(texture_);
	sprite.setScale
	(
		size.x / texture_.getSize().x,
		size.y / texture_.getSize().y
	);
	sprite.setPosition(pos);
	sprite.setColor(sf::Color::Yellow);
	w.draw(sprite);

	sf::Sprite skill_sprite;
	skill_sprite.setTexture(s_->get_texture());
	skill_sprite.setScale
	(
		size.x / s_->get_texture().getSize().x / 1.5,
		size.y / s_->get_texture().getSize().y / 1.5
	);
	skill_sprite.setPosition
	(
		pos.x + size.x / 2 - size.x / 3,
		pos.y + size.y / 2 - size.y / 3
	);
	w.draw(skill_sprite);
}
