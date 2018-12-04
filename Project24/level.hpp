#pragma once

#include <queue>
#include <set>
#include "squad.hpp"
#include "gamemap.hpp"
#include "fText.hpp"
#include "message.hpp"
#include "fText_lib.hpp"

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
class level
{
	using lgamemap = gamemap<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>;
	using lmap_cell = typename lgamemap::map_cell;
	using cell_ptr = lmap_cell*;
	static inline auto cell_ptr_cmp_ = [](cell_ptr f, cell_ptr s)
	{
		if (!f->get_squad() || !s->get_squad()) throw std::exception("wrong cell here");
		return f->get_squad()->get_initiative() > s->get_squad()->get_initiative();
	};
	lgamemap map_;
	std::unique_ptr<message> popup_msg_;
	using cell_coordinate = std::pair<uint8_t, uint8_t>;
	std::priority_queue<cell_ptr, std::vector<cell_ptr>, decltype(cell_ptr_cmp_)> squads_queue_{cell_ptr_cmp_};
	inline static sf::Vector2f msg_pos_ = { WindowResolutionX * 3 / 4, WindowResolutionY - WindowResolutionY / 50 - 5 };
public:
	enum mode_t { battle_mode, book_mode, settings_mode };
	int get_mode() const { return mode_; }
	explicit level(lgamemap& m);
	void action(sf::RenderWindow& w, sf::Event& e);
	void draw(sf::RenderWindow& w);
	void open_book(sf::RenderWindow& w, sf::Event& e);
	void push_message(fText&& msg, const float duration, const sf::Vector2f& position);

private:
	mode_t mode_ = battle_mode;
};

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
level<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::level(lgamemap& m) :
	map_(std::move(m))
{
	for (auto& column : map_.cells_)
		for (auto& c : column)
			if (c.get_squad())
				squads_queue_.push(&c);
}

//template <pixels WindowResolutionX, pixels WindowResolutionY,
//	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
//void level<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::place_squad(const coord_t x, const coord_t y, squad* s)
//{
//	map_.place_squad(x, y, s);
//	squads_queue_.push(map_.cells_[x][y]);
//}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void level<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::action(sf::RenderWindow& w, sf::Event& e)
{
	if (squads_queue_.empty())
	{
	bool one_team = true;
		std::string master;
		for (auto& column : map_.cells_)
		{
			for (auto& c : column)
			{
				if (c.get_squad())
				{
					if (master.empty()) master = c.get_squad()->master_name();
					else if (master != c.get_squad()->master_name() &&
						!c.get_squad()->master_name().empty()) one_team = false;
					if (c.get_squad()->size() == 0 &&
						--c.get_squad()->get_time_until_disappear() <= 0)
						c.delete_squad();
					else squads_queue_.push(&c);
				}
			}
		}

		if (one_team)
		{
			w.close();
			return;
		}
	}


	if (squads_queue_.empty())
	{
		w.close();
		return;
	}

	auto this_cell = squads_queue_.top();
	this_cell->highlight_squad(w);
	
	if (is_that_squad<summoner_squad>(this_cell->get_squad()))
	{
		fText commands
		(
			"- Press: 'B' to open book, 'S' to skip",
			"Fonts/04b30.ttf",
			WindowResolutionY / 50,
			sf::Color::Red,
			sf::Color::White,
			1.0
		);

		commands.setPosition(10.0f, WindowResolutionY - WindowResolutionY / 50 - 5);
		w.draw(commands);

		static auto command_key = -1;

		if (command_key == -1)
		{
			w.pollEvent(e);

			switch (e.type)
			{
			case sf::Event::Closed:
				w.close();
				return;
			case sf::Event::KeyPressed:
				switch (e.key.code)
				{
				case sf::Keyboard::Escape:
					w.close();
					return;
				case sf::Keyboard::S:
					squads_queue_.pop();
					return;
				case sf::Keyboard::B:
				{
					sf::Cursor cursor;
					if (cursor.loadFromSystem(sf::Cursor::Hand))
						w.setMouseCursor(cursor);
					mode_ = book_mode;
					return;
				}
				default:
				{
					sf::Cursor cursor;
					if (cursor.loadFromSystem(sf::Cursor::Arrow))
						w.setMouseCursor(cursor);
					break;
				}
				}
				command_key = e.key.code;
				break;
			default:
				return;
			}
		}

		w.pollEvent(e);

		if (command_key == sf::Keyboard::LControl &&
			e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::S)
		{
			map_.save();
			push_message(msg::saved(WindowResolutionY / 60), 0.01f, msg_pos_);
			command_key = -1;
		}
		
		command_key = -1;

	}
	else if (is_that_squad<creature_squad>(this_cell->get_squad()))
	{
		auto cmd = msg::commands(WindowResolutionY / 50);
		cmd.setPosition(10.0f, WindowResolutionY - WindowResolutionY / 50 - 5);
		w.draw(cmd);

		static auto command_key = -1;

		if (command_key == -1)
		{
			w.pollEvent(e);

			switch(e.type)
			{
			case sf::Event::Closed:
				w.close();
				return;
			case sf::Event::KeyPressed:
				switch(e.key.code)
				{
				case sf::Keyboard::Escape:
					w.close();
					return;
				case sf::Keyboard::S:
					squads_queue_.pop();
					return;
				case sf::Keyboard::A:
				{
					sf::Cursor cursor;
					if (cursor.loadFromSystem(sf::Cursor::Cross))
						w.setMouseCursor(cursor);
					break;
				}
				case sf::Keyboard::M:
				{
					sf::Cursor cursor;
					if (cursor.loadFromSystem(sf::Cursor::Hand))
						w.setMouseCursor(cursor);
					break;
				}
				default: break;
				}
				command_key = e.key.code;
				break;
			default: 
				return;
			}
		}

		w.pollEvent(e);
		
		if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
		{
			sf::Cursor cursor;
			if (cursor.loadFromSystem(sf::Cursor::Arrow))
				w.setMouseCursor(cursor);

			switch (command_key)
			{
			case sf::Keyboard::A:
			{
				if (!map_.inside(e.mouseButton.x, e.mouseButton.y)) break;
				cell_ptr victim_cell = map_.find_cell({ e.mouseButton.x, e.mouseButton.y });
				if (!victim_cell->get_squad()) { push_message(std::move(msg::empty(WindowResolutionY / 60)), 0.01f, msg_pos_); command_key = -1; return; }
				if (this_cell->get_squad()->master_name() == victim_cell->get_squad()->master_name())
					push_message( std::move(msg::attack_own_squad(WindowResolutionY / 60)), 0.01f, msg_pos_);
				else
				{
					std::pair<uint8_t, uint8_t> attack_cell_coordinate = lgamemap::find_cell_coordinate(this_cell);
					std::pair<uint8_t, uint8_t> victim_cell_coordinate = lgamemap::find_cell_coordinate(victim_cell);
					auto range = lgamemap::find_range(attack_cell_coordinate, victim_cell_coordinate);
					if (range.first <= 1 && range.second <= 1) this_cell->get_squad()->attack(victim_cell->get_squad());
					else { push_message(std::move(msg::far_victim(WindowResolutionY / 60)), 0.01f, msg_pos_); command_key = -1; return; }
					squads_queue_.pop();
				}
				command_key = -1;
				return;
			}
			case sf::Keyboard::M:
			{
				if (!map_.inside(e.mouseButton.x, e.mouseButton.y)) break;
				cell_ptr move_cell = map_.find_cell({ e.mouseButton.x, e.mouseButton.y });
				if (move_cell->get_cell_type() == lmap_cell::building)
					{ push_message(std::move(msg::wall_on_the_cell(WindowResolutionY / 60)), 0.01f, msg_pos_); command_key = -1; break; }
				if (move_cell->get_cell_type() == lmap_cell::abyss)
				{
					squads_queue_.pop();
					this_cell->delete_squad();
					command_key = -1;
					return;
				}
				if (move_cell->get_squad())
					{ push_message(std::move(msg::another_squad_on_the_cell(WindowResolutionY / 60)), 0.01f, msg_pos_); command_key = -1; return; }
				std::pair<uint8_t, uint8_t> this_cell_coordinate = lgamemap::find_cell_coordinate(this_cell);
				std::pair<uint8_t, uint8_t> move_cell_coordinate = lgamemap::find_cell_coordinate(move_cell);
				auto range = lgamemap::find_range(this_cell_coordinate, move_cell_coordinate);
				if (range.first <= 1 && range.second <= 1) move_cell->change_squad(this_cell->remove_squad());
				else { push_message(std::move(msg::far_cell(WindowResolutionY / 60)), 0.01f, msg_pos_); command_key = -1; return; }
				squads_queue_.pop();
				command_key = -1;
				return;
			}
			default:
				command_key = -1;
				break;
			}
		}
		else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right)
		{
			command_key = -1;
			sf::Cursor cursor;
			if (cursor.loadFromSystem(sf::Cursor::Arrow))
				w.setMouseCursor(cursor);
		}
		else if (command_key == sf::Keyboard::LControl &&
			e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::S)
		{
			map_.save();
			push_message(std::move(msg::saved(WindowResolutionY / 60)), 0.06f, msg_pos_);
			command_key = -1;
		}

	}
}

template <pixels WindowResolutionX, pixels WindowResolutionY,
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void level<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::draw(sf::RenderWindow& w)
{
	map_.draw_me(w);

	sf::RectangleShape bottom_line({WindowResolutionX, float(WindowResolutionY) / 50 + 10});
	bottom_line.setPosition(0, WindowResolutionY - bottom_line.getSize().y);
	bottom_line.setFillColor(sf::Color(255, 255, 255, 160));
	w.draw(bottom_line);

	if (popup_msg_ ? popup_msg_->draw(w) : false) popup_msg_.reset();
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void level<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::open_book(sf::RenderWindow& w, sf::Event& e)
{
	w.pollEvent(e);
	auto this_cell = squads_queue_.top();
	dynamic_cast<summoner_squad<necromancy>*>(this_cell->get_squad())->get_book().draw_me(w);

	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
	{
		mode_ = battle_mode;
		return;
	}
	
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Right)
	{
		// to the next page
	}
}

template <pixels WindowResolutionX, pixels WindowResolutionY, 
	cmp_resolution::ratio WindowRatioX, cmp_resolution::ratio WindowRatioY>
void level<WindowResolutionX, WindowResolutionY, WindowRatioX, WindowRatioY>::push_message
(
	fText&& msg,
	const float duration, 
	const sf::Vector2f& position
)
{
	popup_msg_.reset
	(
		new message
		(
			std::move(msg),
			duration,
			position
		)
	);
}
