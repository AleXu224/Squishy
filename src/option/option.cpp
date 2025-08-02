#include "option.hpp"

#include "formula/constant.hpp"
#include "formula/requires.hpp"

std::array<std::pair<const std::vector<Option::Types> Option::CharacterList::*, Formula::BoolNode>, 12> Option::CharacterList::getMembersAndConditions() {
	return std::array<std::pair<const std::vector<Option::Types> Option::CharacterList::*, Formula::BoolNode>, 12>{
		std::pair{&CharacterList::normal, Formula::ConstantBool(true)},
		std::pair{&CharacterList::charged, Formula::ConstantBool(true)},
		std::pair{&CharacterList::plunge, Formula::ConstantBool(true)},
		std::pair{&CharacterList::skill, Formula::ConstantBool(true)},
		std::pair{&CharacterList::burst, Formula::ConstantBool(true)},
		std::pair{&CharacterList::passive1, Requirement::passive1},
		std::pair{&CharacterList::passive2, Requirement::passive2},
		std::pair{&CharacterList::passive3, Requirement::passive3},
		std::pair{&CharacterList::constellation1, Requirement::constellation1},
		std::pair{&CharacterList::constellation2, Requirement::constellation2},
		std::pair{&CharacterList::constellation4, Requirement::constellation4},
		std::pair{&CharacterList::constellation6, Requirement::constellation6},
	};
}
