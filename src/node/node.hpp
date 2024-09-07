#pragma once

#include "array"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "string_view"
#include "vector"


namespace Node {
	struct Instance {
		std::string_view name;
		Data data;
		Formula::FloatNode formula;

		template<class T>
		Instance(const T &t) : name(t.name), data(t._data), formula(t._formula) {}
	};

	using Types = Instance;

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
	};

	using WeaponList = std::vector<Node::Types>;

	using ArtifactList = std::vector<Node::Types>;
}// namespace Node
