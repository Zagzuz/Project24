#pragma once

#include "creature.hpp"
#include <type_traits>

template <class SchoolType>
class melee_creature : public creature<SchoolType>
{
protected:
	using buff = void(*)(melee_creature<SchoolType>&);
public:
	melee_creature() : creature<SchoolType>() {}
	melee_creature
	(
		std::string name,
		const ChType& xp_for_the_kill,
		const ChType& health,
		const ChType& attack,
		const ChType& defense
	) :
		creature<SchoolType>(
		std::move(name), 
		xp_for_the_kill, 
		health, attack,
		defense)
	{}

	virtual void apply_buff(const std::any& b) override;
	virtual unit<SchoolType>* get_copy() override;

	std::string save_info() const override;
	void load_info(std::ifstream& file_stream) override;

	friend struct buff_library;
};

template <class SchoolType>
void melee_creature<SchoolType>::apply_buff(const std::any& b)
{
	std::any_cast<buff>(b)(*this);
}

template <class SchoolType>
unit<SchoolType>* melee_creature<SchoolType>::get_copy()
{
	return new melee_creature<SchoolType>(*this);
}

template <class SchoolType>
std::string melee_creature<SchoolType>::save_info() const
{
	std::ostringstream oss;
	oss << "melee_creature" << ' '
		<< this->name_ << ' '
		<< this->xp_for_the_kill_ << ' '
		<< this->health_ << ' '
		<< this->attack_ << ' '
		<< this->defense_ << ' ';
	return oss.str();
}

template <class SchoolType>
void melee_creature<SchoolType>::load_info(std::ifstream& file_stream)
{
	file_stream >> this->name_;
	file_stream >> this->xp_for_the_kill_;
	file_stream >> this->health_;
	file_stream >> this->attack_;
	file_stream >> this->defense_;
}
