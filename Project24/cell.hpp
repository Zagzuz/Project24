#pragma once

#include <memory>
#include <sstream>
#include "squad.hpp"
#include "melee_creature_squad.hpp"
#include "melee_creature.hpp"

using pixels = unsigned;

template <pixels XCellSize, pixels  YCellSize>
class cell
{
	std::unique_ptr<squad> squad_pointer_;
	std::unique_ptr<std::any> buff_;
	pixels x_coord_, y_coord_;
	sf::Texture texture_,
		highlight_texture_,
		click_texture_,
		building_texture_,
		abyss_texture_;
public:
	cell();
	cell(pixels x_coord, pixels y_coord);
	cell(const cell&) = delete;
	cell(cell&&) = default;
	cell& operator= (cell&& c) noexcept;
	void fill(squad* s, std::any* b);
	void change_squad(squad* s);
	void change_buff(std::any&& b) { buff_.reset(&b); }
	squad* get_squad() const { return squad_pointer_.get(); }
	squad* remove_squad();
	void delete_squad();
	void click_me(sf::RenderWindow&) const;
	//void rclick_me(sf::RenderWindow&) const;
	void draw_size(sf::RenderWindow&) const;
	void draw_level(sf::RenderWindow&) const;
	void highlight_me(sf::RenderWindow&) const;
	void draw_me(sf::RenderWindow&) const;
	void draw_obstacle(sf::RenderWindow&) const;
	void draw_squad(sf::RenderWindow&) const;
	void highlight_squad(sf::RenderWindow&) const;
	bool inside(pixels, pixels) const;
	pixels get_x_coord() const { return x_coord_; }
	pixels get_y_coord() const { return y_coord_; }
	enum obstacle_t : int { normal, building, abyss };
	std::string save_info() const;
	void load_info(std::ifstream& file_stream);
	void build_a_wall();
	void dig_an_abyss();
	obstacle_t get_cell_type() const { return obstacle_; }
	~cell() = default;
private:
	obstacle_t obstacle_;
};

template <pixels XCellSize, pixels YCellSize>
cell<XCellSize, YCellSize>::cell(): x_coord_(1), y_coord_(1)
{
	obstacle_ = normal;

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
	obstacle_ = normal;
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
	obstacle_ = c.obstacle_;
	squad_pointer_ = std::move(c.squad_pointer_);
	buff_ = std::move(c.buff_);
	x_coord_ = c.x_coord_;
	y_coord_ = c.y_coord_;
	texture_ = c.texture_;
	return *this;
}

template <pixels XCellSize, pixels  YCellSize>
void cell<XCellSize, YCellSize>::fill(squad* s, std::any* b)
{
	squad_pointer_.reset(s);
	buff_.reset(b);
	squad_pointer_->load_textures();
}

template <pixels XCellSize, pixels  YCellSize>
void cell<XCellSize, YCellSize>::change_squad(squad* s)
{
	squad_pointer_.reset(s);
	buff_.release();
	squad_pointer_->load_textures();
}

template <pixels XCellSize, pixels  YCellSize>
squad* cell<XCellSize, YCellSize>::remove_squad()
{
	buff_.reset();
	return squad_pointer_.release();
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::delete_squad()
{
	buff_.reset();
	squad_pointer_.reset();
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::click_me(sf::RenderWindow& w) const
{
	sf::Sprite sprite(click_texture_);
	sprite.setColor(sf::Color::Black);
	sprite.setScale
	(
		float(XCellSize) / texture_.getSize().x, 
		float(YCellSize) / texture_.getSize().y
	);
	sprite.setPosition(x_coord_, y_coord_);
	w.draw(sprite);
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::draw_size(sf::RenderWindow& w) const
{
	if (squad_pointer_)
	{
		sf::Text squad_size_text;
		sf::Font font;
		if (font.loadFromFile("Fonts/04b30.ttf") == 0)
		{
			std::cerr << "Error loading squad size font";
			return;
		}
		squad_size_text.setFont(font);
		squad_size_text.setString(std::to_string(squad_pointer_->size()));
		squad_size_text.setCharacterSize(13);
		squad_size_text.setPosition(x_coord_ + 1, y_coord_ + YCellSize / 2 + 1);
		squad_size_text.setFillColor(squad_pointer_->get_color());
		squad_size_text.setOutlineColor(sf::Color::White);
		squad_size_text.setOutlineThickness(2.0);
		w.draw(squad_size_text);
	}
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::draw_level(sf::RenderWindow& w) const
{
	if (squad_pointer_)
	{
		sf::Text squad_size_text;
		sf::Font font;
		if (font.loadFromFile("Fonts/04b30.ttf") == 0)
		{
			std::cerr << "Error loading squad size font";
			return;
		}
		squad_size_text.setFont(font);
		squad_size_text.setString(
			std::to_string(
				int(squad_pointer_->get_level())
			)
		);
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
	sf::Sprite sprite;
	sprite.setTexture(texture_);
	sprite.setScale
	(
		float(XCellSize) / texture_.getSize().x,
		float(YCellSize) / texture_.getSize().y
	);
	sprite.setPosition(x_coord_, y_coord_);
	w.draw(sprite);
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::draw_obstacle(sf::RenderWindow& w) const
{
	switch (obstacle_)
	{
	case normal: break;
	case building:
	{
		sf::Sprite sprite_obstacle;
		sprite_obstacle.setTexture(building_texture_);
		sprite_obstacle.setScale
		(
			float(XCellSize) / building_texture_.getSize().x,
			float(XCellSize) / building_texture_.getSize().x * 0.9
		);
		sprite_obstacle.setPosition(x_coord_, y_coord_ - XCellSize / 2);
		w.draw(sprite_obstacle);
		break;
	}
	case abyss:
	{
		sf::Sprite sprite_obstacle(abyss_texture_);
		sprite_obstacle.setScale
		(
			float(XCellSize) / abyss_texture_.getSize().x,
			float(YCellSize) / abyss_texture_.getSize().y
		);
		sprite_obstacle.setPosition(x_coord_, y_coord_);
		w.draw(sprite_obstacle);
		break;
	}
	}
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::draw_squad(sf::RenderWindow& w) const
{
	if (squad_pointer_ == nullptr) return;
	sf::Sprite u_sprite(squad_pointer_->get_texture());
	auto s = squad_pointer_->get_texture().getSize();
	const float scale_coeff = float(XCellSize) / s.x * 0.9;
	u_sprite.setScale(scale_coeff, scale_coeff);
	u_sprite.setPosition(x_coord_, y_coord_ - XCellSize / 2);
	w.draw(u_sprite);
	if (is_that_squad<summoner_squad>(squad_pointer_.get())) draw_level(w);
	else draw_size(w);

	/*fText master_name_text
	(
		vec_->master_name() + "'s squad",
		"Fonts/04b30.ttf",
		8,
		sf::Color::Yellow,
		sf::Color::Black,
		0.1f
	);
	master_name_text.setPosition(u_sprite.getPosition());
	w.draw(master_name_text);*/
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::highlight_squad(sf::RenderWindow& w) const
{
	if (squad_pointer_ == nullptr) return;
	sf::Sprite u_sprite(squad_pointer_->get_texture());
	u_sprite.setColor(sf::Color::Green);
	auto s = squad_pointer_->get_texture().getSize();
	const float scale_coeff = float(XCellSize) / s.x * 0.9;
	u_sprite.setScale(scale_coeff, scale_coeff);
	u_sprite.setPosition(x_coord_, y_coord_ - XCellSize / 2);
	w.draw(u_sprite);
	if (is_that_squad<summoner_squad>(squad_pointer_.get())) draw_level(w);
	else draw_size(w);
}

template <pixels XCellSize, pixels YCellSize>
bool cell<XCellSize, YCellSize>::inside(pixels x, pixels y) const
{
	return x < x_coord_ + XCellSize &&
		y < y_coord_ + YCellSize &&
		x > x_coord_ && y > y_coord_;
}

template <pixels XCellSize, pixels YCellSize>
std::string cell<XCellSize, YCellSize>::save_info() const
{
	std::ostringstream oss;
	if (squad_pointer_) oss << squad_pointer_->save_info();
	else oss << nullptr << ' ';
	oss << obstacle_ << ' ';
	return oss.str();
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::load_info(std::ifstream& file_stream)
{
	std::string token;
	std::getline(file_stream, token, ' ');
	if (token == "summoner_squad")
	{
		std::getline(file_stream, token, ' ');
		if (token == "animalism")
			squad_pointer_.reset(new summoner_squad<animalism>);
		else if (token == "demonology")
			squad_pointer_.reset(new summoner_squad<demonology>);
		else if (token == "necromancy")
			squad_pointer_.reset(new summoner_squad<necromancy>);
		squad_pointer_->load_info(file_stream);
	}
	else if (token == "melee_creature_squad")
	{
		std::getline(file_stream, token, ' ');
		if (token == "animalism")
			squad_pointer_.reset(new melee_creature_squad<animalism>);
		else if (token == "demonology")
			squad_pointer_.reset(new melee_creature_squad<demonology>);
		else if (token == "necromancy")
			squad_pointer_.reset(new melee_creature_squad<necromancy>);
		squad_pointer_->load_info(file_stream);
	}
	/*else if (token == "range_creature_squad")
	{
		std::getline(file_stream, token, ' ');
		if (token == "animalism")
			squad_pointer_.reset(new melee_creature_squad<animalism>);
		else if (token == "demonology")
			squad_pointer_.reset(new melee_creature_squad<demonology>);
		else if (token == "necromancy")
			squad_pointer_.reset(new melee_creature_squad<necromancy>);
		squad_pointer_->load_info(file_stream);
	}*/

	file_stream >> token;
	obstacle_ = obstacle_t(std::stoi(token));
	switch(obstacle_)
	{
	case normal: 
		break;
	case building:
		if (!building_texture_.loadFromFile("Images/building.png"))
			std::cerr << "[LOAD] building texture load error";
		break;
	case abyss:
		if (!abyss_texture_.loadFromFile("Images/abyss.png"))
			std::cerr << "[LOAD] abyss texture load error";
		break;
	}
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::build_a_wall()
{
	obstacle_ = building;
	if (!building_texture_.loadFromFile("Images/building.png"))
		std::cerr << "building texture load error";
}

template <pixels XCellSize, pixels YCellSize>
void cell<XCellSize, YCellSize>::dig_an_abyss()
{
	obstacle_ = abyss;
	if (!abyss_texture_.loadFromFile("Images/abyss.png"))
		std::cerr << "abyss texture load error";
}
