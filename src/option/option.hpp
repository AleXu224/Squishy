#pragma once

#include "array"
#include "boolean.hpp"
#include "utils/hashedString.hpp"
#include "valueList.hpp"
#include "valueSlider.hpp"
#include "variant"
#include <functional>
#include <unordered_map>


namespace Option {
	using Types = std::variant<Boolean, ValueList, ValueSlider>;

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
				[](const Option::ValueSlider &option) {
					return true;
				},
			},
			options.at(key.hash)
		);
	}

	[[nodiscard]] inline float getFloat(const TypesMap &options, const Utils::HashedString &key, float defaultValue = 0.f) {
		return std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) {
					return option.active ? 1.f : 0.f;
				},
				[&defaultValue](const Option::ValueList &option) {
					return static_cast<float>(option.getValue().value_or(defaultValue));
				},
				[](const Option::ValueSlider &option) {
					return option.getValue();
				},
			},
			options.at(key.hash)
		);
	}

	[[nodiscard]] inline uint32_t getInt(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return static_cast<uint32_t>(std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) -> float {
					return option.active ? 1 : 0;
				},
				[&defaultValue](const Option::ValueList &option) -> float {
					return option.getValue().value_or(defaultValue);
				},
				[](const Option::ValueSlider &option) -> float {
					return option.getValue();
				},
			},
			options.at(key.hash)
		));
	}

	[[nodiscard]] inline uint32_t getIndex(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return std::get<Option::ValueList>(options.at(key.hash)).currentIndex.value_or(defaultValue);
	}

	[[nodiscard]] inline const ValueList &getValueListOption(const TypesMap &options, const Utils::HashedString &key) {
		return std::get<Option::ValueList>(options.at(key.hash));
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

		[[nodiscard]] static Formula::BoolNode conditionFromCharacterSlot(Node::CharacterSlot slot);
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
