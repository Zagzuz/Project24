#pragma once

#include "squad.hpp"

#include <memory>

template <class SchoolType>
class unit_squad : public squad
{
protected:
	const std::unique_ptr<unit<SchoolType>> units_chs_; // original unit characteristics
	std::unique_ptr<unit<SchoolType>> cur_unit_chs_; // characteristics under buffs
public:
	explicit unit_squad(const unsigned, std::string, unit<SchoolType>&);
	unit_squad(const unit_squad&) = delete;
	unit_squad(unit_squad&&) = default;
	std::string get_name() const override { return units_chs_->get_name() + " squad"; }
	std::string get_unit_name() const override { return units_chs_->get_name(); }
	void apply_buff(const std::any& b) override;
	void remove_buff(const std::any& b) override;
	friend class attack;
};

template <class SchoolType>
void unit_squad<SchoolType>::apply_buff(const std::any& b)
{
	this->buffs_.push_back(b);
	this->cur_unit_chs_->apply_buff(b);
}

template <class SchoolType>
void unit_squad<SchoolType>::remove_buff(const std::any& b)
{
	for (auto it = buffs_.begin(); it != buffs_.end(); ++it)
		if (b.type() == it->type()) 
		{
			buffs_.erase(it);
			break;
		}

	this->cur_unit_chs_.reset(this->units_chs_.get());
	for (const auto& buff_effect : this->buffs_)
		this->cur_unit_chs_->apply_buff(buff_effect);
}

template <class SchoolType>
unit_squad<SchoolType>::unit_squad
(
	const unsigned initiative,
	std::string way,
	unit<SchoolType>& unit_ref
) :
	squad(initiative, std::move(way)),
	units_chs_(unit_ref.get_copy()),
	cur_unit_chs_(unit_ref.get_copy())
{}
