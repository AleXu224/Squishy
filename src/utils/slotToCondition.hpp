#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"
#include "formula/requires.hpp"
#include "node/entry.hpp"
#include "utility"

namespace Utils {
	static inline Formula::BoolNode slotToCondition(const Node::CharacterSlot &entry) {
		switch (entry) {
			case Node::CharacterSlot::normal:
				return Formula::ConstantBool(true);
			case Node::CharacterSlot::charged:
				return Formula::ConstantBool(true);
			case Node::CharacterSlot::plunge:
				return Formula::ConstantBool(true);
			case Node::CharacterSlot::skill:
				return Formula::ConstantBool(true);
			case Node::CharacterSlot::burst:
				return Formula::ConstantBool(true);
			case Node::CharacterSlot::passive1:
				return Requirement::passive1;
			case Node::CharacterSlot::passive2:
				return Requirement::passive2;
			case Node::CharacterSlot::passive3:
				return Requirement::passive3;
			case Node::CharacterSlot::constellation1:
				return Requirement::constellation1;
			case Node::CharacterSlot::constellation2:
				return Requirement::constellation2;
			case Node::CharacterSlot::constellation3:
				return Requirement::constellation3;
			case Node::CharacterSlot::constellation4:
				return Requirement::constellation4;
			case Node::CharacterSlot::constellation5:
				return Requirement::constellation5;
			case Node::CharacterSlot::constellation6:
				return Requirement::constellation6;
		}
		std::unreachable();
	}
}// namespace Utils