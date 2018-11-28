#pragma once

#include "relations_holder.hpp"
#include <iostream>
#include <sstream>

class skill;
using pixels = unsigned;

#include <list>
#include <fstream>
#include <any>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class squad
{
protected:
	unsigned initiative_;
	std::list<std::any> buffs_;
	sf::Texture texture_, dead_texture_;
	std::string path_, dpath_; // to the texture
	int time_until_disappear_;
public:
	explicit squad
	(
		const unsigned ini,
		std::string w,
		std::string dw = "Images/dead.png",
		const int t = 0
	) :
		initiative_(ini),
		path_(std::move(w)),
		dpath_(std::move(dw)),
		time_until_disappear_(t)
	{}
	squad(const squad&) = delete;
	squad(squad&&) = default;
	virtual void apply_buff(const std::any& b) = 0;
	virtual void remove_buff(const std::any& b) = 0;
	virtual std::string get_name() const = 0;
	const sf::Texture& get_texture() const { return texture_; }
	void load_textures();
	unsigned get_initiative() const { return initiative_; }
	int& get_time_until_disappear() { return time_until_disappear_; }
	virtual std::string get_unit_name() const = 0;
	virtual void attack(squad*) = 0;
	virtual void victim(ChType damage, relation_coeffs_t&) = 0;	
	virtual void spell_cast(skill*) const { std::cout << "squad spell_cast"; }
	virtual void add_skill(skill*) { std::cout << "squad add skill"; }
	virtual void save(const std::string& filename) const;
	virtual std::size_t size() const { return 1; }
	virtual ~squad() = default;
};

inline void squad::load_textures()
{
	if (!texture_.loadFromFile(path_))
		std::cerr << "Error in squad texture loading: " << path_;
	if (!dead_texture_.loadFromFile(dpath_))
		std::cerr << "Error in dead squad texture loading " << dpath_;
}

inline void squad::save(const std::string& filename) const
{
	std::ofstream f(filename, std::ios_base::out | std::ios_base::trunc);
}

template <template <class = necromancy> class SquadType,
	typename = std::enable_if<std::is_base_of_v<squad, SquadType<>>>>
bool is_that_squad(squad* s)
{
	if (s == nullptr) return false;
	std::string name_to_check = typeid(*s).name();
	const std::string name_to_find = typeid(SquadType).name();
	std::string cut;
	for (size_t i = name_to_find.find("_squad") - 1;
		name_to_find[i] != ' ' && name_to_find[i] != '_'
		&& i != std::string::npos; --i)
		cut.insert(cut.begin(), name_to_find[i]);
	return name_to_check.find(cut) != std::string::npos;
}