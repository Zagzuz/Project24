#pragma once

#include <memory>

#include "squad.hpp"
#include "melee_creature_squad.hpp"
#include "melee_creature.hpp"

using pixels = unsigned;

template <pixels XCellSize, pixels  YCellSize>
class cell
{
	std::unique_ptr<squad> vec_;
	std::unique_ptr<std::any> buff_;
	pixels x_coord_, y_coord_;
	sf::Texture texture_, highlight_texture_, click_texture_;
public:
	cell();
	cell(pixels x_coord, pixels y_coord);
	cell(const cell&) = delete;
	cell(cell&&) = default;
	cell& operator= (cell&& c) noexcept;
	void fill(squad* s, std::any* b);
	void change_squad(squad* s);
	void change_buff(std::any&& b) { buff_.reset(&b); }
	squad* get_squad() const { return vec_.get(); }
	squad* remove_squad();
	void delete_squad();
	void click_me(sf::RenderWindow&) const;
	void rclick_me(sf::RenderWindow&) const;
	void draw_text(sf::RenderWindow&) const;
	void highlight_me(sf::RenderWindow&) const;
	void draw_me(sf::RenderWindow&) const;
	void draw_squad(sf::RenderWindow&) const;
	void highlight_squad(sf::RenderWindow&) const;
	bool inside(pixels, pixels) const;
	pixels get_x_coord() const { return x_coord_; }
	pixels get_y_coord() const { return y_coord_; }
	~cell() = default;
};

template <pixels XCellSize, pixels YCellSize>
cell<XCellSize, YCellSize>::cell(): x_coord_(1), y_coord_(1)
{
	if (texture_.loadFromFile("Images/cell.png") == 0)
		std::cerr << "Error loading cell texture";

	if (highlight_texture_.loadFromFile("Images/cell_highlight.png") == 0)
		std::cerr << "Error loading cell texture";
	
	if (click_texture_.loadFromFile("Images/cell_click.png") == 0)
		std::cerr << "Error loading cell click texture";
}

template <pixels XCellSize, pixels  YCellSize>
cell<XCellSize, YCellSize>::cell(const pixels x_coord, const pixels y_coord)
{
	x_coord_ = x_coord;
	y_coord_ = y_coord;

	if (texture_.loadFromFile("Images/cell.png") == 0)
		std::cerr << "Error loading cell texture";

	if (highlight_texture_.loadFromFile("Images/cell_highlight.png") == 0)
		std::cerr << "Error loading cell texture";

	if (click_texture_.loadFromFile("Images/cell_click.png") == 0)
		std::cerr << "Error loading cell click texture";
}

template <pixels XCellSize, pixels YCellSize>
cell<XCellSize, YCellSize>& cell<XCellSize, YCellSize>::operator=(cell&& c) noexcept
{
	vec_ = std::move(c.vec_);
	buff_ = std::move(c.buff_);
	x_coord_ = c.x_coord_;
	y_coord_ = c.y_coord_;
	texture_ = c.texture_;
	return *this;
}

template <pixels XCellSize, pixels  YCellSize>
void cell<XCellSize, YCellSize>::fill(squad* s, std::any* b)
{
	vec_.reset(s);
	buff_.reset(b);
	vec_->load_textures();
}

template <pixels XCellSize, pixels  YCellSize>
void cell<XCellSize, YCellSize>::change_squad(squad* s)
{
	vec_.reset(s);
	buff_.release();
	vec_->load_textures();
}

template <pixels XCellSize, pixels  YCellSize>
squad* cell<XCellSize, YCellSize>::remove_squad()
{
	buff_.reset();
	return vec_.release();
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::delete_squad()
{
	buff_.reset();
	vec_.reset();
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::click_me(sf::RenderWindow& w) const
{
	sf::Sprite sprite(click_texture_);
	sprite.setColor(sf::Color::Black);
	sprite.setScale(float(XCellSize) / texture_.getSize().x, float(YCellSize) / texture_.getSize().y);
	sprite.setPosition(x_coord_, y_coord_);
	w.draw(sprite);
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::rclick_me(sf::RenderWindow& w) const
{
	
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::draw_text(sf::RenderWindow& w) const
{
	if (vec_)
	{
		sf::Text squad_size_text;
		sf::Font font;
		if (font.loadFromFile("Fonts/04b30.ttf") == 0)
		{
			std::cerr << "Error loading squad size font";
			return;
		}
		squad_size_text.setFont(font);
		squad_size_text.setString(std::to_string(vec_->size()));
		squad_size_text.setCharacterSize(13);
		squad_size_text.setPosition(x_coord_ + 1, y_coord_ + YCellSize / 2 + 1);
		squad_size_text.setFillColor(sf::Color::Black);
		squad_size_text.setOutlineColor(sf::Color::White);
		squad_size_text.setOutlineThickness(2.0);
		w.draw(squad_size_text);
	}
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::highlight_me(sf::RenderWindow& w) const
{
	sf::Sprite sprite(highlight_texture_);
	sprite.setScale(float(XCellSize) / texture_.getSize().x, float(YCellSize) / texture_.getSize().y);
	sprite.setPosition(x_coord_, y_coord_);
	w.draw(sprite);
}

template <pixels XCellSize, pixels  YCellSize>
void cell<XCellSize, YCellSize>::draw_me(sf::RenderWindow& w) const
{
	sf::Sprite sprite(texture_);
	sprite.setScale(float(XCellSize) / texture_.getSize().x, float(YCellSize) / texture_.getSize().y);
	sprite.setPosition(x_coord_, y_coord_);
	w.draw(sprite);
}


template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::draw_squad(sf::RenderWindow& w) const
{
	if (vec_ == nullptr) return;
	sf::Sprite u_sprite(vec_->get_texture());
	auto s = vec_->get_texture().getSize();
	const float scale_coeff = float(XCellSize) / s.x * 0.9;
	u_sprite.setScale(scale_coeff, scale_coeff);
	u_sprite.setPosition(x_coord_, y_coord_ - XCellSize / 2);
	w.draw(u_sprite);
	draw_text(w);
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::highlight_squad(sf::RenderWindow& w) const
{
	if (vec_ == nullptr) return;
	sf::Sprite u_sprite(vec_->get_texture());
	u_sprite.setColor(sf::Color::Green);
	auto s = vec_->get_texture().getSize();
	const float scale_coeff = float(XCellSize) / s.x * 0.9;
	u_sprite.setScale(scale_coeff, scale_coeff);
	u_sprite.setPosition(x_coord_, y_coord_ - XCellSize / 2);
	w.draw(u_sprite);
	draw_text(w);
}

template <pixels XCellSize, pixels YCellSize>
bool cell<XCellSize, YCellSize>::inside(pixels x, pixels y) const
{
	return x < x_coord_ + XCellSize &&
		y < y_coord_ + YCellSize &&
		x > x_coord_ && y > y_coord_;
}
