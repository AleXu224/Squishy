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
		return std::get<Option::Boolean>(options.at(key.hash)).active;
	}

	inline float getFloat(const TypesMap &options, const Utils::HashedString &key, float defaultValue = 0.f) {
		return static_cast<float>(std::get<Option::ValueList>(options.at(key.hash)).getValue().value_or(defaultValue));
	}

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

		[[nodiscard]] static auto getMembers() {
			return std::array{&CharacterList::normal, &CharacterList::charged, &CharacterList::plunge, &CharacterList::skill, &CharacterList::burst, &CharacterList::passive1, &CharacterList::passive2, &CharacterList::constellation1, &CharacterList::constellation2, &CharacterList::constellation4, &CharacterList::constellation6};
		}
	};

	using WeaponList = std::vector<Types>;

	using ArtifactList = std::vector<Types>;

	using CharacterMap = TypesMap;

	using WeaponMap = TypesMap;

	using ArtifactMap = TypesMap;

	inline void mapOptions(CharacterMap &ret, const CharacterList &vals) {
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

	inline void mapOptions(WeaponMap &ret, const WeaponList &vals) {
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
