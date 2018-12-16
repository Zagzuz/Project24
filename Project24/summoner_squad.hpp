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
	summoner_squad() {}
	explicit summoner_squad(const unsigned, std::string, unit<SchoolType>&);
	summoner_squad(const summoner_squad&) = delete;
	summoner_squad(summoner_squad&&) = default;
	template <class KnownSchoolType> ChType knowledge_value() const;
	void attack(squad* victim) override {}
	void victim(ChType damage, relation_coeffs_t& r) override {}
	const book& get_book() const override { return book_of_magic_; }
	void spell_cast(skill* s) const override;
	void add_skill(skill* s) override { book_of_magic_.add_skill(s); }
	std::string get_name() const override { return this->cur_unit_chs_->get_name(); }
	template <class T> friend std::ostream& operator<< (std::ostream&, const summoner_squad<T>&);
	ChType get_mana() const override;
	ChType get_cur_mana() const override;
	void set_cur_mana(const ChType m) override;
	ChType get_exp() const override;
	ChType get_cur_exp() const override;
	void set_cur_exp(ChType) override;
	ChType get_level() const override;
	std::string save_info() const override;
	void load_info(std::ifstream& file_stream) override;
	ChType get_knowledge_value(const std::string& school) const override;
	void set_knowledge_value(const std::string& school, const ChType v) override;
	template <class KnownSchool> void knowledge_set(const ChType v);
	template <class KnownSchool>
	ChType knowledge_get() const;
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
		(this->cur_unit_chs_.get())->template get_knowledge_value<KnownSchoolType>();
}

template <class SchoolType>
void summoner_squad<SchoolType>::spell_cast(skill* s) const
{
	std::cout << "cast it!";
}

template <class SchoolType>
ChType summoner_squad<SchoolType>::get_mana() const
{
	return dynamic_cast<const summoner<SchoolType>*>(this->units_chs_.get())->get_mana();
}

template <class SchoolType>
ChType summoner_squad<SchoolType>::get_cur_mana() const
{
	return dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())->get_mana();
}

template <class SchoolType>
void summoner_squad<SchoolType>::set_cur_mana(const ChType m)
{
	dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())->set_mana(m);
}

template <class SchoolType>
ChType summoner_squad<SchoolType>::get_exp() const
{
	return dynamic_cast<const summoner<SchoolType>*>(this->units_chs_.get())->get_experience();
}

template <class SchoolType>
ChType summoner_squad<SchoolType>::get_cur_exp() const
{
	return dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())->get_experience();
}

template <class SchoolType>
void summoner_squad<SchoolType>::set_cur_exp(ChType value)
{
	dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())->change_exp() = value;
}

template <class SchoolType>
ChType summoner_squad<SchoolType>::get_level() const
{
	return dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())->get_level();
}

template <class SchoolType>
std::string summoner_squad<SchoolType>::save_info() const
{
	std::ostringstream oss;
	oss << "summoner_squad" << ' '
		<< std::string(typeid(SchoolType).name()).substr(6) << ' ' // without 'class' keyword
		<< this->initiative_ << ' '
		<< this->path_ << ' '
		<< this->dpath_ << ' '
		<< this->time_until_disappear_ << ' '
		<< this->units_chs_->save_info()
		<< this->cur_unit_chs_->save_info()
		<< book_of_magic_.save_info();
	return oss.str();
}

template <class SchoolType>
void summoner_squad<SchoolType>::load_info(std::ifstream& file_stream)
{
	file_stream >> this->initiative_;
	file_stream >> this->path_;
	file_stream >> this->dpath_;
	this->load_textures();
	file_stream >> this->time_until_disappear_;

	std::string token;
	file_stream >> token;
	if (token == "summoner")
	{
		this->units_chs_.reset(new summoner<SchoolType>());
		const_cast<unit<SchoolType>*>(this->units_chs_.get())->load_info(file_stream);
	}
	file_stream >> token;
	if (token == "summoner")
	{
		this->cur_unit_chs_.reset(new summoner<SchoolType>());
		this->cur_unit_chs_->load_info(file_stream);
	}

	book_of_magic_.load_info(file_stream);
}

template <class SchoolType>
ChType summoner_squad<SchoolType>::get_knowledge_value(const std::string& school) const
{
	ChType val = 0;
	if (school.find("animalism") != std::string::npos) val = knowledge_get<animalism>();
	else if (school.find("demonology") != std::string::npos) val = knowledge_get<demonology>();
	else if (school.find("necromancy") != std::string::npos) val = knowledge_get<necromancy>();
	return val;
}

template <class SchoolType>
void summoner_squad<SchoolType>::set_knowledge_value(const std::string& school, const ChType v)
{
	if (school.find("animalism") != std::string::npos) knowledge_set<animalism>(v);
	else if (school.find("demonology") != std::string::npos) knowledge_set<demonology>(v);
	else if (school.find("necromancy") != std::string::npos) knowledge_set<necromancy>(v);
}

template <class SchoolType>
template <class KnownSchool>
void summoner_squad<SchoolType>::knowledge_set(const ChType v)
{
	dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())
		->template set_knowledge_value<KnownSchool>(v);
}

template <class SchoolType>
template <class KnownSchool>
ChType summoner_squad<SchoolType>::knowledge_get() const
{
	return dynamic_cast<summoner<SchoolType>*>(this->cur_unit_chs_.get())
		->template get_knowledge_value<KnownSchool>();
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
