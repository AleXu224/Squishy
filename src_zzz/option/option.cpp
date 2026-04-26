#include "option.hpp"

#include "formula/constant.hpp"
#include "formula/requirement.hpp"

std::array<std::pair<const std::vector<Option::Types> Option::AgentList::*, Formula::BoolNode>, 20> Option::AgentList::getMembersAndConditions() {
	return std::array<std::pair<const std::vector<Option::Types> Option::AgentList::*, Formula::BoolNode>, 20>{
		std::pair{&AgentList::basic, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::dodge, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::dashAttack, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::dodgeCounter, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::assist, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::quickAssist, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::defensiveAssist, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::evasiveAssist, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::assistFollowup, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::special, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::exspecial, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::chain, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::ultimate, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::core, Formula::ConstantBool({}, true)},
		std::pair{&AgentList::mindscape1, Requirement::mindscape1},
		std::pair{&AgentList::mindscape2, Requirement::mindscape2},
		std::pair{&AgentList::mindscape3, Requirement::mindscape3},
		std::pair{&AgentList::mindscape4, Requirement::mindscape4},
		std::pair{&AgentList::mindscape5, Requirement::mindscape5},
		std::pair{&AgentList::mindscape6, Requirement::mindscape6},
	};
}
