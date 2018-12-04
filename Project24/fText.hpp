#pragma once

#include <SFML/Graphics/Text.hpp>
#include <iostream>

class fText : public sf::Text
{
	sf::Font* font_ = nullptr;
public:
	fText
	(
		const std::string& msg,
		const std::string& font_path,
		const unsigned char_size,
		const sf::Color fill,
		const sf::Color outline,
		const float thick
	);
	fText(fText&& text) noexcept;
	fText(const fText& text);
	~fText() { delete font_; }
};

inline fText::fText
(
	const std::string& msg, 
	const std::string& font_path, 
	const unsigned char_size,
	const sf::Color fill,
	const sf::Color outline,
	const float thick
)
{
	font_ = new sf::Font;
	if (font_->loadFromFile(font_path) == 0)
	{
		std::cerr << "Error loading font for text:\n'" << msg << "'";
		return;
	}
	setCharacterSize(char_size);
	setFillColor(fill);
	setOutlineColor(outline);
	setOutlineThickness(thick);
	setString(msg);
	setFont(*font_);
}

inline fText::fText(fText&& text) noexcept : sf::Text(std::move(text))
{
	font_ = text.font_;
	text.font_ = nullptr;
}

inline fText::fText(const fText& text) : sf::Text(text)
{
	if (text.font_ == nullptr) font_ = nullptr;
	else font_ = new sf::Font(*text.font_);
}
