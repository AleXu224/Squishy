#pragma once

#include "array"
#include "boolean.hpp"
#include "utils/hashedString.hpp"
#include "valueList.hpp"
#include "variant"
#include <functional>
#include <unordered_map>


namespace Option {
	using Types = std::variant<Boolean, ValueList>;

	using TypesMap = std::unordered_map<uint32_t, Types>;

	inline bool getBool(const TypesMap &options, const Utils::HashedString &key) {
		return std::visit(
			Utils::overloaded{
				[](const Option::Boolean &option) {
					return option.active;
				},
				[](const Option::ValueList &option) {
					return option.currentIndex.has_value();
				},
			},
			options.at(key.hash)
		);
	}

	[[nodiscard]] inline float getFloat(const TypesMap &options, const Utils::HashedString &key, float defaultValue = 0.f) {
		return static_cast<float>(std::get<Option::ValueList>(options.at(key.hash)).getValue().value_or(defaultValue));
	}

	[[nodiscard]] inline uint32_t getInt(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return std::get<Option::ValueList>(options.at(key.hash)).getValue().value_or(defaultValue);
	}

	[[nodiscard]] inline uint32_t getIndex(const TypesMap &options, const Utils::HashedString &key, uint32_t defaultValue = 0) {
		return std::get<Option::ValueList>(options.at(key.hash)).currentIndex.value_or(defaultValue);
	}

	[[nodiscard]] inline const ValueList &getValueListOption(const TypesMap &options, const Utils::HashedString &key) {
		return std::get<Option::ValueList>(options.at(key.hash));
	}

	struct AgentList {
		std::vector<Types> basic{};
		std::vector<Types> dodge{};
		std::vector<Types> dashAttack{};
		std::vector<Types> dodgeCounter{};
		std::vector<Types> assist{};
		std::vector<Types> quickAssist{};
		std::vector<Types> defensiveAssist{};
		std::vector<Types> evasiveAssist{};
		std::vector<Types> assistFollowup{};
		std::vector<Types> special{};
		std::vector<Types> exspecial{};
		std::vector<Types> chain{};
		std::vector<Types> ultimate{};
		std::vector<Types> core{};
		std::vector<Types> mindscape1{};
		std::vector<Types> mindscape2{};
		std::vector<Types> mindscape3{};
		std::vector<Types> mindscape4{};
		std::vector<Types> mindscape5{};
		std::vector<Types> mindscape6{};

		[[nodiscard]] static inline auto getMembers() {
			return std::array{
				&AgentList::basic,
				&AgentList::dodge,
				&AgentList::dashAttack,
				&AgentList::dodgeCounter,
				&AgentList::assist,
				&AgentList::quickAssist,
				&AgentList::defensiveAssist,
				&AgentList::evasiveAssist,
				&AgentList::assistFollowup,
				&AgentList::special,
				&AgentList::exspecial,
				&AgentList::chain,
				&AgentList::ultimate,
				&AgentList::core,
				&AgentList::mindscape1,
				&AgentList::mindscape2,
				&AgentList::mindscape3,
				&AgentList::mindscape4,
				&AgentList::mindscape5,
				&AgentList::mindscape6,
			};
		}

		[[nodiscard]] inline const std::vector<Types> &fromAgentSlot(Node::AgentSlot slot) const {
			switch (slot) {
				case Node::AgentSlot::basic:
					return basic;
				case Node::AgentSlot::dodge:
					return dodge;
				case Node::AgentSlot::dashAttack:
					return dashAttack;
				case Node::AgentSlot::dodgeCounter:
					return dodgeCounter;
				case Node::AgentSlot::assist:
					return assist;
				case Node::AgentSlot::quickAssist:
					return quickAssist;
				case Node::AgentSlot::defensiveAssist:
					return defensiveAssist;
				case Node::AgentSlot::evasiveAssist:
					return evasiveAssist;
				case Node::AgentSlot::assistFollowup:
					return assistFollowup;
				case Node::AgentSlot::special:
					return special;
				case Node::AgentSlot::exspecial:
					return exspecial;
				case Node::AgentSlot::chain:
					return chain;
				case Node::AgentSlot::ultimate:
					return ultimate;
				case Node::AgentSlot::core:
					return core;
				case Node::AgentSlot::mindscape1:
					return mindscape1;
				case Node::AgentSlot::mindscape2:
					return mindscape2;
				case Node::AgentSlot::mindscape3:
					return mindscape3;
				case Node::AgentSlot::mindscape4:
					return mindscape4;
				case Node::AgentSlot::mindscape5:
					return mindscape5;
				case Node::AgentSlot::mindscape6:
					return mindscape6;
			}
			std::unreachable();
		}

		[[nodiscard]] static std::array<std::pair<const std::vector<Types> Option::AgentList::*, Formula::BoolNode>, 20> getMembersAndConditions();
	};

	using EngineList = std::vector<Types>;

	using DiscList = std::vector<Types>;

	inline void mapOptions(TypesMap &ret, const AgentList &vals) {
		for (auto listMember: AgentList::getMembers()) {
			const auto &_ = std::invoke(listMember, vals);

			for (const auto &val: _) {
				std::visit(
					[&](auto &&v) {
						ret.insert({v.key, v});
					},
					val
				);
			}
		}
	}

	inline void mapOptions(TypesMap &ret, const EngineList &vals) {
		for (const auto &val: vals) {
			std::visit(
				[&](auto &&v) {
					ret.insert({v.key, v});
				},
				val
			);
		}
	}
}// namespace Option
