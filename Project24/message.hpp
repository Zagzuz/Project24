#pragma once

#include "fText.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class message
{
	fText text_;
	const float duration_;
	float draw_duration_;
public:
	message
	(
		fText&& text,
		const float duration,
		const sf::Vector2f position
	);
	bool draw(sf::RenderWindow& window);
	const sf::Text& get_text() const { return text_; }
};

inline message::message
(
	fText&& text, 
	const float duration, 
	const sf::Vector2f position
):
	text_(std::move(text)),
	duration_(duration)
{
	draw_duration_ = 0;
	text_.setPosition(position);
}

inline bool message::draw(sf::RenderWindow& window)
{
	sf::Clock clock;
	window.draw(text_);
	draw_duration_ += clock.getElapsedTime().asSeconds();
	return draw_duration_ > duration_;
}
