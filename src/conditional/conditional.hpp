#pragma once

#include "boolean.hpp"
#include "ranges"
#include "valueList.hpp"
#include "variant"
#include <unordered_map>


namespace Conditional {
	using Types = std::variant<Boolean, ValueList>;

	struct List {
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
			return std::array{&List::normal, &List::charged, &List::plunge, &List::skill, &List::burst, &List::passive1, &List::passive2, &List::constellation1, &List::constellation2, &List::constellation4, &List::constellation6};
		}
	};

	struct ListMapped {
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
			return std::array{&ListMapped::normal, &ListMapped::charged, &ListMapped::plunge, &ListMapped::skill, &ListMapped::burst, &ListMapped::passive1, &ListMapped::passive2, &ListMapped::constellation1, &ListMapped::constellation2, &ListMapped::constellation4, &ListMapped::constellation6};
		}
	};

	inline ListMapped mapConditionals(const List &vals) {
		ListMapped ret{};

		for (auto [listMember, mappedMember]: std::views::zip(List::getMembers(), ListMapped::getMembers())) {
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

		return ret;
	}
}// namespace Conditional
