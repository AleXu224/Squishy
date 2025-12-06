#pragma once

#include "array"
#include "entry.hpp"
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
		std::vector<Node::Types> passive3{};
		std::vector<Node::Types> constellation1{};
		std::vector<Node::Types> constellation2{};
		std::vector<Node::Types> constellation3{};
		std::vector<Node::Types> constellation4{};
		std::vector<Node::Types> constellation5{};
		std::vector<Node::Types> constellation6{};

		[[nodiscard]] static auto getMembers() {
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

		[[nodiscard]] const auto &fromEntry(CharacterSlot entry) const {
			switch (entry) {
				case CharacterSlot::normal:
					return normal;
				case CharacterSlot::charged:
					return charged;
				case CharacterSlot::plunge:
					return plunge;
				case CharacterSlot::skill:
					return skill;
				case CharacterSlot::burst:
					return burst;
				case CharacterSlot::passive1:
					return passive1;
				case CharacterSlot::passive2:
					return passive2;
				case CharacterSlot::passive3:
					return passive3;
				case CharacterSlot::constellation1:
					return constellation1;
				case CharacterSlot::constellation2:
					return constellation2;
				case CharacterSlot::constellation3:
					return constellation3;
				case CharacterSlot::constellation4:
					return constellation4;
				case CharacterSlot::constellation5:
					return constellation5;
				case CharacterSlot::constellation6:
					return constellation6;
			}
			std::unreachable();
		}
	};

	using WeaponList = std::vector<Node::Types>;

	using ArtifactList = std::vector<Node::Types>;
}// namespace Node
