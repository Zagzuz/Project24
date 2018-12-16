#pragma once

#include "unit.hpp"
#include "relations_holder.hpp"
#include <fstream>
#include <sstream>
#include <string>

using knowledge_values_t = relation_coeffs_t;

template <class SchoolType>
class summoner : public unit<SchoolType>
{
protected:
	using buff = void(*)(summoner<SchoolType>&);
	ChType mana_;
	ChType experience_;
	ChType level_;
	knowledge_values_t values_;
public:
	summoner() : unit<SchoolType>(), values_{ 0, 0, 0 } {}

	summoner
	(
		std::string name,
		const ChType& mana,
		const ChType& experience,
		const ChType& level,
		knowledge_values_t values
	);

	summoner(const summoner&) = default;
	summoner(summoner&&) = default;
	summoner& operator=(const summoner& rhs);

	ChType get_mana() const { return mana_; }
	ChType get_experience() const { return experience_; }
	ChType get_level() const { return level_; }
	ChType& change_exp() { return experience_; }
	void set_mana(const ChType m) { mana_ = m; }
	
	std::string save_info() const override;
	void load_info(std::ifstream& file_stream) override;

	template <class KnownSchoolType> ChType get_knowledge_value() const;
	template <class KnownSchoolType> void set_knowledge_value(ChType v);

	void apply_buff(const std::any& b) override;
	unit<SchoolType>* get_copy() override;

	friend struct buff_library;
};

template <class SchoolType>
std::string summoner<SchoolType>::save_info() const
{
	std::ostringstream oss;
	oss << "summoner" << ' '
		<< this->name_ << ' '
		<< this->mana_ << ' '
		<< this->experience_ << ' '
		<< this->level_ << ' ';
	return oss.str();
}

template<class SchoolType>
void summoner<SchoolType>::load_info(std::ifstream& file_stream)
{
	file_stream >> this->name_;
	file_stream >> this->mana_;
	file_stream >> this->experience_;
	file_stream >> this->level_;
}

template <class SchoolType>
template <class KnownSchoolType>
ChType summoner<SchoolType>::get_knowledge_value() const
{
	return std::get<coeff_holder<KnownSchoolType>>(values_).coeff;
}

template <class SchoolType>
template <class KnownSchoolType>
void summoner<SchoolType>::set_knowledge_value(ChType v)
{
	std::get<coeff_holder<KnownSchoolType>>(values_).coeff = v;
}

template <class SchoolType>
summoner<SchoolType>::summoner
(
	std::string name,
	const ChType& mana,
	const ChType& experience,
	const ChType& level,
	knowledge_values_t values
) :
	unit<SchoolType>(std::move(name)),
	mana_(mana),
	experience_(experience),
	level_(level),
	values_(std::move(values))
{
	/*if (this->texture_.loadFromFile("Images/summoner.png") == 0)
	{
		std::cerr << "Error loading summoner texture";
		return;
	}*/
}

template <class SchoolType>
summoner<SchoolType>& summoner<SchoolType>::operator=(const summoner<SchoolType>& rhs)
{
	this->experience_ = rhs.experience_;
	this->level_ = rhs.level_;
	this->mana_ = rhs.mana_;
	return *this;
}

template <class SchoolType>
void summoner<SchoolType>::apply_buff(const std::any& b)
{
	std::any_cast<buff>(b)(*this);
}

template <class SchoolType>
unit<SchoolType>* summoner<SchoolType>::get_copy()
{
	return new summoner<SchoolType>(*this);
}
