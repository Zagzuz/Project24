#pragma once

#include "unit_squad.hpp"
#include "summoner.hpp"
#include "unit.hpp"
#include "book.hpp"

template <class SchoolType>
class summoner_squad : public unit_squad<SchoolType>
{
	book book_of_magic_;
public:
	explicit summoner_squad(const unsigned, std::string, unit<SchoolType>&);
	summoner_squad(const summoner_squad&) = delete;
	summoner_squad(summoner_squad&&) = default;
	template <class KnownSchoolType> ChType knowledge_value() const;
	void attack(squad* victim) override {}
	void victim(ChType damage, relation_coeffs_t& r) override {}
	const book& get_book() const { return book_of_magic_; }
	void spell_cast(skill* s) const override;
	void add_skill(skill* s) override { book_of_magic_.add_skill(s); }
	std::string get_name() const override { return this->cur_unit_chs_->get_name(); }
	template <class T> friend std::ostream& operator<< (std::ostream&, const summoner_squad<T>&);
};

template <class SchoolType>
summoner_squad<SchoolType>::summoner_squad
(
	const unsigned initiative,
	std::string way,
	unit<SchoolType>& unit_ref
) :
	unit_squad<SchoolType>(initiative, std::move(way), unit_ref)
{}

template <class SchoolType>
template <class KnownSchoolType>
ChType summoner_squad<SchoolType>::knowledge_value() const
{
	return static_cast<summoner<SchoolType>*>
		(this->cur_unit_chs_.get())->template knowledge_value<KnownSchoolType>();
}

template <class SchoolType>
void summoner_squad<SchoolType>::spell_cast(skill* s) const
{
	std::cout << "cast it!";
}

template <class SchoolType>
std::ostream& operator<<(std::ostream& os, const summoner_squad<SchoolType>& s)
{
	os << typeid(s).name() << '\n'
		<< s.initiative_ << '\n';
	for (auto& b : s.buffs_)
		os << b << ',';
	os << s.units_chs_ << '\n'
		<< s.cur_units_chs_ << '\n'
		<< s.path_ << '\n';
	return os;
}
