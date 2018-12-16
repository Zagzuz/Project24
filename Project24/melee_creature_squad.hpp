#pragma once

#include "creature_squad.hpp"
#include "melee_creature.hpp"

template <class SchoolType>
class melee_creature_squad : public creature_squad<SchoolType>
{
public:
	melee_creature_squad() {}
	explicit melee_creature_squad
	(
		const unsigned initiative,
		std::string way,
		unit<SchoolType>& unit_ref,
		const std::size_t size,
		std::string master_name = {}
	);
	melee_creature_squad(const melee_creature_squad&) = delete;
	melee_creature_squad(melee_creature_squad&&) = default;
	std::string save_info() const override;
	void load_info(std::ifstream& file_stream) override;
};

template <class SchoolType>
melee_creature_squad<SchoolType>::melee_creature_squad
(
	const unsigned initiative,
	std::string way,
	unit<SchoolType>& unit_ref, 
	const std::size_t size,
	std::string master_name
) :
	creature_squad<SchoolType>
	(
		initiative,
		std::move(way),
		unit_ref,
		size,
		std::move(master_name)
	)
{}

template <class SchoolType>
std::string melee_creature_squad<SchoolType>::save_info() const
{
	std::ostringstream oss;
	oss << "melee_creature_squad" << ' '
		<< std::string(typeid(SchoolType).name()).substr(6) << ' '
		<< this->initiative_ << ' '
		<< this->path_ << ' '
		<< this->dpath_ << ' '
		<< this->time_until_disappear_ << ' ';
	
	if (this->units_chs_) oss << this->units_chs_->save_info();
	else oss << nullptr << ' ';
	if (this->cur_unit_chs_) oss << this->cur_unit_chs_->save_info();
	else oss << nullptr << ' ';

	oss << this->damage_dealt_to_the_last_unit_ << ' '
		<< this->size_ << ' '
		<< this->dead_ << ' '
		<< this->master_ << ' ';
	return oss.str();
}

template <class SchoolType>
void melee_creature_squad<SchoolType>::load_info(std::ifstream& file_stream)
{
	file_stream >> this->initiative_;
	file_stream >> this->path_;
	file_stream >> this->dpath_;
	this->load_textures();
	file_stream >> this->time_until_disappear_;

	std::string token;
	file_stream >> token;
	if (token == "melee_creature")
	{
		this->units_chs_.reset(new melee_creature<SchoolType>());
		const_cast<unit<SchoolType>*>(this->units_chs_.get())->load_info(file_stream);
	}
	file_stream >> token;
	if (token == "melee_creature")
	{
		this->cur_unit_chs_.reset(new melee_creature<SchoolType>());
		this->cur_unit_chs_->load_info(file_stream);
	}
	
	file_stream >> this->damage_dealt_to_the_last_unit_;
	file_stream >> this->size_;
	file_stream >> this->dead_;
	file_stream >> this->master_;
	this->cook_color();
}
