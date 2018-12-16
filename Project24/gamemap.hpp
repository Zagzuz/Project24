#pragma once

#include "cell.hpp"
#include "resolution.hpp"

#include <sstream>
#include <vector>
#include "save_map.hpp"

using coord_t = unsigned;

constexpr cmp_resolution::ratio x_cells_number = 12, y_cells_number = 9;

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
class gamemap
{
	using res = cmp_resolution::cell_map_resolution<WindowRatioX, WindowRatioY, WindowResolutionX, WindowResolutionY>;
	static const pixels map_resolution_x = res::get_x();
	static const pixels map_resolution_y = res::get_y();
	static inline constexpr pixels x_cell_size_ = map_resolution_x / x_cells_number;
	static inline constexpr pixels y_cell_size_ = map_resolution_y / y_cells_number;
	sf::Texture texture_;
public:
	gamemap();
	void load();
	void save() const;
	gamemap(gamemap&&) = default;
	void draw_me(sf::RenderWindow& w);
	void add_skill(int x, int y, skill* s);
	using map_cell = cell<x_cell_size_, y_cell_size_>;
	static bool inside(pixels x_coord, pixels y_coord);
	void remove_squad(const coord_t x, const coord_t y);
	sf::Texture get_texture() const { return texture_; }
	void place_squad(const coord_t x, const coord_t y, squad* s);
	const map_cell& find_cell(pixels x_coord, pixels y_coord) const;
	squad* operator() (pixels x, pixels y) { return find_cell({ x, y })->get_squad(); } // get map squad
	static pixels left_coordinate() { return (WindowResolutionX - map_resolution_x) / 2; }
	static pixels bottom_coordinate() { return (WindowResolutionY - map_resolution_y) / 2; }
	template <pixels, pixels, cmp_resolution::ratio, cmp_resolution::ratio> friend class level;
	void dig_an_abyss(uint8_t x_cell, uint8_t y_cell) { cells_[x_cell][y_cell].dig_an_abyss(); }
	void build_a_wall(uint8_t x_cell, uint8_t y_cell) { cells_[x_cell][y_cell].build_a_wall (); }
	static pixels top_coordinate() { return WindowResolutionY - (WindowResolutionY - map_resolution_y) / 2; }
	static pixels right_coordinate() { return WindowResolutionX - (WindowResolutionX - map_resolution_x) / 2; }
private:
	std::vector<std::vector<map_cell>> cells_;
	map_cell* find_cell(const sf::Vector2i& vec);
	static std::pair<uint8_t, uint8_t> find_cell_coordinate(map_cell*);
	static std::pair<uint8_t, uint8_t> find_cell_coordinate(const float x, const float y);
	static std::pair<uint8_t, uint8_t> find_range(const std::pair<uint8_t, uint8_t>& f, const std::pair<uint8_t, uint8_t>& s);
};

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::gamemap()
{
	cells_.resize(x_cells_number);
	for (uint8_t i = 0; i < x_cells_number; ++i)
	{
		cells_.at(i).reserve(y_cells_number);
		for (uint8_t j = 0; j < y_cells_number; ++j)
		{
			cells_.at(i).emplace_back(map_cell
			(
				(WindowResolutionX - map_resolution_x) / 2 + i * x_cell_size_,
				(WindowResolutionY - map_resolution_y) / 2 + j * y_cell_size_
			));
		}
	}

	if (texture_.loadFromFile("Images/sand_map.png"/*, sf::IntRect(0, 0, 96, 54)*/) == 0)
	{
		std::cerr << "map texture load failure";
		return;
	}
	texture_.setRepeated(true);
}

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::place_squad(const coord_t x, const coord_t y, squad* s)
{
	cells_[x][y].change_squad(s);
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
const typename gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::map_cell& 
gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::find_cell(pixels x_coord, pixels y_coord) const
{
	return cells_.at(
		(x_coord - left_coordinate()) / x_cell_size_).at(
		(y_coord - bottom_coordinate()) / y_cell_size_);
}

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::add_skill(int x, int y, skill* s)
{
	squad* ptr = cells_[x][y].get_squad();
	if (ptr) ptr->add_skill(s);
}

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::remove_squad(const coord_t x, const coord_t y)
{
	cells_[x][y].remove_squad();
}

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::draw_me(sf::RenderWindow& w)
{
	// draw map sprite
	const sf::Sprite sprite(texture_, sf::IntRect(0, 0, WindowResolutionX, WindowResolutionY));
	w.draw(sprite);

	// get mouse position to highlight cells
	const auto v = sf::Mouse::getPosition(w);
	const bool lclick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	//const bool rclick = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	//for each cell
	for (auto& column : cells_)
		for (auto& c : column)
		{
			//check if mouse is inside this cell
			const bool mouse_inside = c.inside(v.x, v.y);

			c.draw_me(w);

			if (mouse_inside)
			{
				c.highlight_me(w);
				if (lclick) c.click_me(w);
			}

			c.draw_squad(w);
			c.draw_obstacle(w);

			/*if (mouse_inside && rclick)
				c.rclick_me(w);*/
		}
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
bool gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::inside(pixels x_coord, pixels y_coord)
{
	return x_coord < right_coordinate() && 
		x_coord > left_coordinate() && 
		y_coord < top_coordinate() && 
		y_coord > bottom_coordinate();
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
typename gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::map_cell* 
gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::find_cell(const sf::Vector2i& vec)
{
	return
		&cells_.at
		(
			(vec.x - left_coordinate()) / x_cell_size_
		).at
		(
			(vec.y - bottom_coordinate()) / y_cell_size_
		);
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
std::pair<uint8_t, uint8_t> 
gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::find_cell_coordinate(map_cell* ptr)
{
	return
	{
		(ptr->get_x_coord() - left_coordinate()) / x_cell_size_,
		(ptr->get_y_coord() - bottom_coordinate()) / y_cell_size_
	};
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
std::pair<uint8_t, uint8_t>
gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::find_cell_coordinate(const float x, const float y)
{
	return
	{
		(x - left_coordinate()) / x_cell_size_,
		(y - bottom_coordinate()) / y_cell_size_
	};
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
std::pair<uint8_t, uint8_t> gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::find_range
(
	const std::pair<uint8_t, uint8_t>& f, 
	const std::pair<uint8_t, uint8_t>& s
)
{
	return
	{
		std::abs(f.first - s.first),
		std::abs(f.second - s.second)
	};
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
	void gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::save() const
{
	/*std::ofstream ft_stream("Saves/map.smn");
	for (uint8_t i = 0; i < x_cells_number; ++i)
		for (uint8_t j = 0; j < y_cells_number; ++j)
		{
			if (cells_[i][j].get_squad())
			{
				ft_stream << int(i) << ' ' << int(j) << ' '
					<< cells_[i][j].get_squad()->get_unit_name() << ' '
					<< (cells_[i][j].get_squad()->master_name().empty() ?
						"-" : cells_[i][j].get_squad()->master_name());
				if (is_that_squad<melee_creature_squad>(cells_[i][j].get_squad()))
					ft_stream << cells_[i][j].get_squad()->size() << ' ';
			}
		}
	ft_stream.close();*/

	for (uint8_t i = 0; i < x_cells_number; ++i)
	{
		for (uint8_t j = 0; j < y_cells_number; ++j)
		{
			std::ofstream file_stream("Saves/cell" +
				std::to_string(int(i)) + '_' + std::to_string(int(j)) + ".smn");
			file_stream << cells_[i][j].save_info();
			file_stream.close();
		}
	}
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
	void gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::load()
{
	for (uint8_t i = 0; i < x_cells_number; ++i)
	{
		for (uint8_t j = 0; j < y_cells_number; ++j)
		{
			std::ifstream file_stream("Saves/cell" +
				std::to_string(int(i)) + '_' + std::to_string(int(j)) + ".smn");
			cells_[i][j].load_info(file_stream);
			file_stream.close();
		}
	}

	/*std::ifstream ft_stream("Saves/map.smn");
	squad_saver sq;

	std::string map_info;
	std::getline(ft_stream, map_info);
	std::istringstream iss(map_info);

	std::string token;
	int i, j;
	std::string squad_name;
	for (int counter = 0; std::getline(iss, token, ' '); ++counter)
	{
		std::istringstream token_iss(token);
		switch (counter)
		{
		case 0:
			token_iss >> i;
			break;
		case 1:
			token_iss >> j;
			break;
		case 2:
			token_iss >> squad_name;
			counter = -1;

			if (auto creatures_it = sq.creatures_map.find(squad_name);
				creatures_it != sq.creatures_map.end())
			{
				std::string master_name;
				std::getline(iss, master_name, ' ');
				if (master_name[0] == '-') master_name.clear();
				const auto func = creatures_it->second;
				std::size_t size;
				std::getline(iss, token, ' ');
				token_iss.clear();
				token_iss.str(token);
				token_iss >> size;
				cells_[i][j].change_squad(func(size, master_name));
			}
			else if (auto summoners_it = sq.summoners_map.find(squad_name);
				summoners_it != sq.summoners_map.end())
				cells_[i][j].change_squad(summoners_it->second());

			break;
		default:
			break;
		}
	}
	
	ft_stream.close();*/
}


