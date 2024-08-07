#pragma once

#include "array"
#include "atkNode.hpp"
#include "variant"


namespace Node {
	using Types = std::variant<Node::Atk>;

	struct List {
		std::vector<Node::Types> normal;
		std::vector<Node::Types> charged;
		std::vector<Node::Types> plunge;
		std::vector<Node::Types> skill;
		std::vector<Node::Types> burst;
		std::vector<Node::Types> passive1{};
		std::vector<Node::Types> passive2{};
		std::vector<Node::Types> constellation1{};
		std::vector<Node::Types> constellation2{};
		std::vector<Node::Types> constellation4{};
		std::vector<Node::Types> constellation6{};
		std::vector<Node::Types> weapon{};
		std::vector<Node::Types> artifact{};

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
				&List::weapon,
				&List::artifact,
			};
		}

		static inline void combineNodes(Node::List &to, const Node::List &from) {
			for (auto listPtr : List::getMembers()) {
				for (auto &node: std::invoke(listPtr, from)) {
					std::invoke(listPtr, to).emplace_back(std::move(node));
				}
			}
		}
	};
}// namespace Node
