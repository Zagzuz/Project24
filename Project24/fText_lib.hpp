#pragma once
#include "fText.hpp"

namespace msg
{
	inline fText empty(const unsigned ch_size)
	{
		return
		{
		"[The cell is empty]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Red,
		sf::Color::Black,
		0.6f
		};
	}

	inline fText saved(const unsigned ch_size)
	{
		return
		{
		"[Saved]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color(0, 107, 60),
		sf::Color::Green,
		0.6f
		};
	}

	inline fText commands(const unsigned ch_size)
	{
		return
		{
		"- Press: 'A' to attack, 'M' to move, 'S' to skip",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Red,
		sf::Color::White,
		0.5f
		};
	}

	inline fText far_cell(const unsigned ch_size)
	{
		return
		{
		"[The cell is too far]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Red,
		sf::Color::Black,
		0.6f
		};
	}

	inline fText far_victim(const unsigned ch_size)
	{
		return
		{
		"[The victim is too far]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Red,
		sf::Color::Black,
		0.6f
		};
	}

	inline fText attack_own_squad(const unsigned ch_size)
	{
		return
		{
		"[You can not attack your own squad]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Black,
		sf::Color::Red,
		0.6f
		};
	}

	inline fText wall_on_the_cell(const unsigned ch_size)
	{
		return
		{
		"[There is a wall on this cell]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Black,
		sf::Color::Red,
		0.6f
		};
	}

	inline fText another_squad_on_the_cell(const unsigned ch_size)
	{
		return
		{
		"[There is another squad on the cell]",
		"Fonts/04b30.ttf",
		ch_size,
		sf::Color::Red,
		sf::Color::Black,
		0.6f
		};
	}
}
