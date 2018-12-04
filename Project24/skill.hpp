#pragma once

#include <string>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>

#include "unit.hpp"

class skill
{
protected:
	std::string name_;
	ChType required_level_;
	ChType mana_cost_;
	ChType number_coefficient_;
	sf::Texture texture_;
	std::string path_;
public:
	skill
	(
		std::string name,
		const ChType required_level,
		const ChType mana_cost,
		const ChType number_coefficient,
		std::string path
	) :
	name_(std::move(name)),
	required_level_(required_level),
	mana_cost_(mana_cost),
	number_coefficient_(number_coefficient),
	path_(std::move(path))
	{
		if (!texture_.loadFromFile(path_))
			std::cerr << "Error skill texture load on path:\n"
			"'" << path << "'";
	}
	const sf::Texture& get_texture() const { return texture_; }
	void draw_me(sf::RenderWindow& w, const sf::Vector2f& pos) const;
};

inline void skill::draw_me(sf::RenderWindow& w, const sf::Vector2f& pos) const
{
}
