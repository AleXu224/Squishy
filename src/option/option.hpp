#pragma once

#include "array"
#include "boolean.hpp"
#include "utils/hashedString.hpp"
#include "valueList.hpp"
#include "variant"
#include <functional>
#include <unordered_map>


namespace Option {
	using Types = std::variant<Boolean, ValueList>;

	using TypesMap = std::unordered_map<uint32_t, Types>;

	inline bool getBool(const TypesMap &options, const Utils::HashedString &key) {
		return std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) {
					return option.active;
				},
				[](const Option::ValueList &option) {
					return option.currentIndex.has_value();
				},
			},
			options.at(key.hash)
		);
	}

	inline float getFloat(const TypesMap &options, const Utils::HashedString &key, float defaultValue = 0.f) {
		return static_cast<float>(std::get<Option::ValueList>(options.at(key.hash)).getValue().value_or(defaultValue));
	}

	inline uint32_t getInt(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return std::get<Option::ValueList>(options.at(key.hash)).getValue().value_or(defaultValue);
	}

	inline uint32_t getIndex(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return std::get<Option::ValueList>(options.at(key.hash)).currentIndex.value_or(defaultValue);
	}

	struct CharacterList {
		std::vector<Types> normal{};
		std::vector<Types> charged{};
		std::vector<Types> plunge{};
		std::vector<Types> skill{};
		std::vector<Types> burst{};
		std::vector<Types> passive1{};
		std::vector<Types> passive2{};
		std::vector<Types> passive3{};
		std::vector<Types> constellation1{};
		std::vector<Types> constellation2{};
		std::vector<Types> constellation3{};
		std::vector<Types> constellation4{};
		std::vector<Types> constellation5{};
		std::vector<Types> constellation6{};

		[[nodiscard]] static inline auto getMembers() {
			return std::array{
				&CharacterList::normal,
				&CharacterList::charged,
				&CharacterList::plunge,
				&CharacterList::skill,
				&CharacterList::burst,
				&CharacterList::passive1,
				&CharacterList::passive2,
				&CharacterList::passive3,
				&CharacterList::constellation1,
				&CharacterList::constellation2,
				&CharacterList::constellation3,
				&CharacterList::constellation4,
				&CharacterList::constellation5,
				&CharacterList::constellation6,
			};
		}

		[[nodiscard]] inline const std::vector<Types> &fromCharacterSlot(Node::CharacterSlot slot) const {
			switch (slot) {
				case Node::CharacterSlot::normal:
					return normal;
				case Node::CharacterSlot::charged:
					return charged;
				case Node::CharacterSlot::plunge:
					return plunge;
				case Node::CharacterSlot::skill:
					return skill;
				case Node::CharacterSlot::burst:
					return burst;
				case Node::CharacterSlot::passive1:
					return passive1;
				case Node::CharacterSlot::passive2:
					return passive2;
				case Node::CharacterSlot::passive3:
					return passive3;
				case Node::CharacterSlot::constellation1:
					return constellation1;
				case Node::CharacterSlot::constellation2:
					return constellation2;
				case Node::CharacterSlot::constellation3:
					return constellation3;
				case Node::CharacterSlot::constellation4:
					return constellation4;
				case Node::CharacterSlot::constellation5:
					return constellation5;
				case Node::CharacterSlot::constellation6:
					return constellation6;
			}
			std::unreachable();
		}

		[[nodiscard]] static std::array<std::pair<const std::vector<Types> Option::CharacterList::*, Formula::BoolNode>, 14> getMembersAndConditions();
	};

	using WeaponList = std::vector<Types>;

	using ArtifactList = std::vector<Types>;

	inline void mapOptions(TypesMap &ret, const CharacterList &vals) {
		for (auto listMember: CharacterList::getMembers()) {
			const auto &_ = std::invoke(listMember, vals);

			for (const auto &val: _) {
				std::visit(
					[&](auto &&v) {
						ret.insert({v.key, v});
					},
					val
				);
			}
		}
	}

	inline void mapOptions(TypesMap &ret, const WeaponList &vals) {
		for (const auto &val: vals) {
			std::visit(
				[&](auto &&v) {
					ret.insert({v.key, v});
				},
				val
			);
		}
	}
}// namespace Option
