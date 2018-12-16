#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "squad.hpp"
#include <iostream>
#include "fText.hpp"

class school_improvement
{
	sf::Texture background_texture_;
	sf::Texture button_texture_;
	sf::Texture press_button_texture_;
public:
	school_improvement();
	bool draw(sf::RenderWindow& w, sf::Event& e, squad* s) const;
};

inline school_improvement::school_improvement()
{
	if (!background_texture_.loadFromFile("Images/obryv.jpg"))
		std::cerr << "error loading background texture in school improvement menu";
	if (!button_texture_.loadFromFile("Images/button.png"))
		std::cerr << "error loading button texture in school improvement menu";
	if (!press_button_texture_.loadFromFile("Images/press_button.png"))
		std::cerr << "error loading press button texture in school improvement menu";
}

inline bool school_improvement::draw(sf::RenderWindow& w, sf::Event& e, squad* s) const
{
	static bool changed = false;

	sf::Sprite background_sprite(background_texture_);
	background_sprite.setScale
	(
		float(w.getSize().x) / background_texture_.getSize().x,
		float(w.getSize().y) / background_texture_.getSize().y
	);
	w.draw(background_sprite);

	sf::Sprite squad_image(s->get_texture());
	squad_image.setScale
	(
		float(w.getSize().x) / 3.0f / s->get_texture().getSize().x,
		float(w.getSize().y) / 1.2f / s->get_texture().getSize().y
	);
	squad_image.setPosition
	(
		float(w.getSize().x) / 35,
		w.getSize().y - squad_image.getLocalBounds().height * 
			squad_image.getScale().y - float(w.getSize().y) / 50
	);
	w.draw(squad_image);

	fText squad_name
	(
		s->get_name(),
		"Fonts/04b30.ttf",
		float(w.getSize().y) / 15,
		sf::Color(0, 0, 0, 200),
		sf::Color(255, 255, 255, 128),
		2.0f
	);
	squad_name.setPosition
	(
		squad_image.getPosition().x,
		squad_image.getPosition().y - squad_name.getCharacterSize() - 10
	);
	w.draw(squad_name);

	fText exp
	(
		"experience points: " +
		std::to_string(int(s->get_cur_exp())) + "/"
		+ std::to_string(int(s->get_exp())),
		"Fonts/04b30.ttf",
		20,
		sf::Color(0, 0, 0, 200),
		sf::Color(255, 255, 255, 128),
		2.0f
	);
	exp.setPosition
	(
		squad_name.getPosition().x,
		squad_name.getPosition().y - exp.getCharacterSize() - 10
	);
	w.draw(exp);

	std::vector<std::string> schools =
	{
		"animalism:",
		"demonology:",
		"necromancy:"
	};

	float x = squad_image.getLocalBounds().width * squad_image.getScale().x + squad_image.getPosition().x +
		(w.getSize().x - squad_image.getLocalBounds().width * squad_image.getScale().x) * 1 / 4;
	float y = squad_image.getPosition().y;

	for (uint8_t i = 0; i < schools.size(); ++i)
	{
		w.pollEvent(e);
		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			return changed;

		fText school_text
		(
			schools[i] + ' ' + std::to_string(int(s->get_knowledge_value(schools[i]))),
			"Fonts/04b30.ttf",
			38,
			sf::Color(255, 0, 0, 128),
			sf::Color(255, 255, 255, 128),
			2.0f
		);
		school_text.setPosition(x, y);
		w.draw(school_text);

		sf::Sprite button(button_texture_);
		button.setScale
		(
			float(school_text.getCharacterSize()) / button_texture_.getSize().x,
			float(school_text.getCharacterSize()) / button_texture_.getSize().y
		);
		button.setPosition
		(
			x + 10 + school_text.getLocalBounds().width,
			y
		);

		sf::Sprite press_button(press_button_texture_);
		press_button.setScale
		(
			float(school_text.getCharacterSize()) / button_texture_.getSize().x,
			float(school_text.getCharacterSize()) / button_texture_.getSize().y
		);
		press_button.setPosition
		(
			x + 10 + school_text.getLocalBounds().width,
			y
		);

		const auto mouse_x = sf::Mouse::getPosition(w).x;
		const auto mouse_y = sf::Mouse::getPosition(w).y;

		ChType school_improve_cost = 20;

		if (mouse_x >= button.getPosition().x &&
			mouse_x <= button.getPosition().x + 
				button.getLocalBounds().width * button.getScale().x &&
			mouse_y >= button.getPosition().y &&
			mouse_y <= button.getPosition().y + 
				button.getLocalBounds().height * button.getScale().y)
		{
			w.draw(press_button);
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
				if (s->get_cur_exp() >= school_improve_cost)
				{
					s->set_knowledge_value(schools[i], s->get_knowledge_value(schools[i]) + 1);
					s->set_cur_exp(s->get_cur_exp() - 20);
					changed = true;
					e = {};
				}
		}
		else w.draw(button);

		y += school_text.getLocalBounds().height + 10;
	}

	return changed;
}
