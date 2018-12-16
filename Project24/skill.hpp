#pragma once

#include <string>
#include <iostream>
#include <SFML/Graphics/Texture.hpp>

#include "squad.hpp"

class skill
{
protected:
	std::string name_;
	ChType required_level_;
	ChType mana_cost_;
	ChType number_coefficient_;
	sf::Texture texture_;
	using func = squad * (*)(const std::size_t, std::string);
	func f_;
	std::string path_;
	std::string school_;
public:
	skill() : f_(nullptr) {}
	skill
	(
		std::string name,
		const ChType required_level,
		const ChType mana_cost,
		const ChType number_coefficient,
		func f,
		std::string path,
		std::string school
	) :
	name_(std::move(name)),
	required_level_(required_level),
	mana_cost_(mana_cost),
	number_coefficient_(number_coefficient),
	f_(f),
	path_(std::move(path)),
	school_(std::move(school))
	{
		if (!texture_.loadFromFile(path_))
			std::cerr << "Error skill texture load on path:\n"
			"'" << path << "'";
	}
	const sf::Texture& get_texture() const { return texture_; }
	std::string get_school() const { return school_; }
	ChType get_mana_cost() const { return mana_cost_; }
	ChType get_required_level() const { return required_level_; }
	std::string save_info() const;
	void load_info(std::ifstream& file_stream);
	squad* operator() (const std::size_t size, std::string master = {}) const { return f_(size, std::move(master)); }
};

inline std::string skill::save_info() const
{
	std::ostringstream oss;
	for (auto& name_ch : name_)
		oss << (name_ch == ' ' ? '&' : name_ch);
	oss << ' ' << required_level_ << ' ' << mana_cost_ << ' '
		<< number_coefficient_ << ' ' << path_ << ' ' << school_ << ' ';
	// save skill function (saver function)
	return oss.str();
}

inline void skill::load_info(std::ifstream& file_stream)
{
	file_stream >> name_;
	std::replace(name_.begin(), name_.end(), '&', ' ');
	file_stream >> required_level_;
	file_stream >> mana_cost_;
	file_stream >> number_coefficient_;
	file_stream >> path_;

	if (!texture_.loadFromFile(path_))
		std::cerr << "[LOAD] Error skill texture load on path:\n"
		"'" << path_ << "'";

	file_stream >> school_;
}
