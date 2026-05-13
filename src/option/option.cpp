#include "option.hpp"

#include "formula/constant.hpp"
#include "formula/requirement.hpp"

[[nodiscard]] Formula::BoolNode Option::CharacterList::conditionFromCharacterSlot(Node::CharacterSlot slot) {
	switch (slot) {
		case Node::CharacterSlot::normal:
		case Node::CharacterSlot::charged:
		case Node::CharacterSlot::plunge:
		case Node::CharacterSlot::skill:
		case Node::CharacterSlot::burst:
			return Formula::ConstantBool({}, true);
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
