#pragma once

#include "boolean.hpp"
#include "ranges"
#include "valueList.hpp"
#include "variant"
#include <functional>
#include <unordered_map>


namespace Conditional {
	using Types = std::variant<Boolean, ValueList>;

	struct CharacterList {
		const std::vector<Types> normal{};
		const std::vector<Types> charged{};
		const std::vector<Types> plunge{};
		const std::vector<Types> skill{};
		const std::vector<Types> burst{};
		const std::vector<Types> passive1{};
		const std::vector<Types> passive2{};
		const std::vector<Types> constellation1{};
		const std::vector<Types> constellation2{};
		const std::vector<Types> constellation4{};
		const std::vector<Types> constellation6{};

		[[nodiscard]] static inline auto getMembers() {
			return std::array{&CharacterList::normal, &CharacterList::charged, &CharacterList::plunge, &CharacterList::skill, &CharacterList::burst, &CharacterList::passive1, &CharacterList::passive2, &CharacterList::constellation1, &CharacterList::constellation2, &CharacterList::constellation4, &CharacterList::constellation6};
		}
	};

	using WeaponList = std::vector<Types>;

	using ArtifactList = std::vector<Types>;

	struct CharacterMap {
		std::unordered_map<std::string_view, Types> normal{};
		std::unordered_map<std::string_view, Types> charged{};
		std::unordered_map<std::string_view, Types> plunge{};
		std::unordered_map<std::string_view, Types> skill{};
		std::unordered_map<std::string_view, Types> burst{};
		std::unordered_map<std::string_view, Types> passive1{};
		std::unordered_map<std::string_view, Types> passive2{};
		std::unordered_map<std::string_view, Types> constellation1{};
		std::unordered_map<std::string_view, Types> constellation2{};
		std::unordered_map<std::string_view, Types> constellation4{};
		std::unordered_map<std::string_view, Types> constellation6{};

		[[nodiscard]] static inline auto getMembers() {
			return std::array{&CharacterMap::normal, &CharacterMap::charged, &CharacterMap::plunge, &CharacterMap::skill, &CharacterMap::burst, &CharacterMap::passive1, &CharacterMap::passive2, &CharacterMap::constellation1, &CharacterMap::constellation2, &CharacterMap::constellation4, &CharacterMap::constellation6};
		}
	};

	using WeaponMap = std::unordered_map<std::string_view, Types>;

	using ArtifactMap = std::unordered_map<std::string_view, Types>;

	inline void mapConditionals(CharacterMap &ret, const CharacterList &vals) {
		for (auto [listMember, mappedMember]: std::views::zip(CharacterList::getMembers(), CharacterMap::getMembers())) {
			const auto &_ = std::invoke(listMember, vals);

			for (auto &val: _) {
				std::visit(
					[&](auto &&v) {
						std::invoke(mappedMember, ret).insert({v.key, v});
					},
					val
				);
			}
		}
	}

	inline void mapConditionals(WeaponMap &ret, const WeaponList &vals) {
		for (auto &val: vals) {
			std::visit(
				[&](auto &&v) {
					ret.insert({v.key, v});
				},
				val
			);
		}
	}

	// No need for this at the moment
	// inline void mapConditionals(ArtifactMap &ret, const ArtifactList &vals) {
	// 	for (auto &val: vals) {
	// 		std::visit(
	// 			[&](auto &&v) {
	// 				ret.insert({v.key, v});
	// 			},
	// 			val
	// 		);
	// 	}
	// }
}// namespace Conditional
