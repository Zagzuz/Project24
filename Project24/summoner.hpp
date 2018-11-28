#pragma once

#include "unit.hpp"
#include "relations_holder.hpp"

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

	template <class KnownSchoolType>
	ChType knowledge_value() const;
	void apply_buff(const std::any& b) override;
	unit<SchoolType>* get_copy() override;

	friend struct buff_library;
};

template <class SchoolType>
template <class KnownSchoolType>
ChType summoner<SchoolType>::knowledge_value() const
{
	return std::get<KnownSchoolType>(values_);
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
