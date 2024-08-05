#pragma once

#include "array"
#include "atkNode.hpp"
#include "variant"



namespace Node {
	using Types = std::variant<Node::Atk>;

	struct List {
		const std::vector<Node::Types> normal;
		const std::vector<Node::Types> charged;
		const std::vector<Node::Types> plunge;
		const std::vector<Node::Types> skill;
		const std::vector<Node::Types> burst;
		const std::vector<Node::Types> passive1{};
		const std::vector<Node::Types> passive2{};
		const std::vector<Node::Types> constellation1{};
		const std::vector<Node::Types> constellation2{};
		const std::vector<Node::Types> constellation4{};
		const std::vector<Node::Types> constellation6{};

		[[nodiscard]] static inline auto getMembers() {
			return std::array{
				&List::normal,
				&List::charged,
				&List::plunge,
				&List::skill,
				&List::burst,
				&List::passive1,
				&List::passive2,
				&List::constellation1,
				&List::constellation2,
				&List::constellation4,
				&List::constellation6,
			};
		}
	};
}// namespace Node
