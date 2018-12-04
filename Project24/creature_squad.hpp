#pragma once

#include <random>

#include "unit_squad.hpp"
#include "summoner_squad.hpp"

template <class SchoolType>
class creature_squad : public unit_squad<SchoolType>
{
protected:
	ChType damage_dealt_to_the_last_unit_;
	std::size_t size_, dead_;
	std::string master_;
	sf::Color team_color;
public:
	explicit creature_squad
	(
		const unsigned initiative,
		std::string way,
		unit<SchoolType>& unit_ref,
		const std::size_t size,
		std::string master_name = {}
	);
	creature_squad(const creature_squad&) = delete;
	creature_squad(creature_squad&&) = default;
	ChType get_damage_dealt_to_the_last_unit() const;
	std::size_t size() const override { return size_; }
	std::size_t dead() const { return dead_; }
	void attack(squad* victim_squad) override;
	void victim(ChType damage, relation_coeffs_t&) override;
	void set_master(squad* master_squad);
	std::string master_name() const override { return master_; }
	sf::Color get_color() const override { return team_color; }
};

template <class SchoolType>
creature_squad<SchoolType>::creature_squad
(
	const unsigned initiative,
	std::string way,
	unit<SchoolType>& unit_ref,
	const std::size_t size,
	std::string master_name
) :
	unit_squad<SchoolType>(initiative, std::move(way), unit_ref),
	damage_dealt_to_the_last_unit_(0),
	size_(size),
	dead_(0),
	master_(std::move(master_name))
{
	std::mt19937 machine;
	const std::hash<std::string> hash_fn;
	machine.seed(hash_fn(master_));
	const std::uniform_int_distribution<> dis(0, 255);
	team_color = { uint8_t(dis(machine)), uint8_t(dis(machine)), uint8_t(dis(machine)) };
}

template <class SchoolType>
ChType creature_squad<SchoolType>::get_damage_dealt_to_the_last_unit() const
{
	return damage_dealt_to_the_last_unit_;
}

template <class SchoolType>
void creature_squad<SchoolType>::attack(squad* victim_squad)
{
	relation_coeffs_t& r = relations_holder<SchoolType>::relation_coeffs;
	auto* squad_unit = dynamic_cast<creature<SchoolType>*>(this->cur_unit_chs_.get());
	victim_squad->victim(squad_unit->get_attack() * this->size(), r);
}

template <class SchoolType>
void creature_squad<SchoolType>::victim(ChType damage, relation_coeffs_t& r)
{
	auto* squad_unit = dynamic_cast<creature<SchoolType>*>(this->cur_unit_chs_.get());
	ChType defense = squad_unit->get_defense() * this->size_;
	damage *= std::get<coeff_holder<SchoolType>>(r).coeff;
	if (damage <= defense) return;
	damage -= defense;
	damage += this->damage_dealt_to_the_last_unit_;

	auto units_killed = static_cast<std::size_t>(damage / squad_unit->get_health()); 

	if (units_killed > size_)
	{
		dead_ = size_;
		size_ = 0;
		damage_dealt_to_the_last_unit_ = 0;
		this->texture_.swap(this->dead_texture_);
	}
	else
	{
		damage_dealt_to_the_last_unit_ = std::fmod(damage, squad_unit->get_health());
		dead_ += units_killed;
		size_ -= units_killed;
	}
}

template <class SchoolType>
void creature_squad<SchoolType>::set_master(squad* master_squad)
{
	if (is_that_squad<summoner_squad>(master_squad)) 
		master_ = master_squad->get_name();
}
