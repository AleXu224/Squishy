#pragma once

#include "array"
#include "atkNode.hpp"
#include "variant"


namespace Node {
	using Types = std::variant<Node::Atk>;

	struct CharacterList {
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

		[[nodiscard]] static inline auto getMembers() {
			return std::array{
				&CharacterList::normal,
				&CharacterList::charged,
				&CharacterList::plunge,
				&CharacterList::skill,
				&CharacterList::burst,
				&CharacterList::passive1,
				&CharacterList::passive2,
				&CharacterList::constellation1,
				&CharacterList::constellation2,
				&CharacterList::constellation4,
				&CharacterList::constellation6,
			};
		}


		static inline void combineNodes(Node::CharacterList &to, const Node::CharacterList &from) {
			for (auto listPtr: CharacterList::getMembers()) {
				for (auto &node: std::invoke(listPtr, from)) {
					std::invoke(listPtr, to).emplace_back(std::move(node));
				}
			}
		}
	};
	
	using WeaponList = std::vector<Node::Types>;

	using ArtifactList = std::vector<Node::Types>;
}// namespace Node
