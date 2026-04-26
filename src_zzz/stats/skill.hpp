#pragma once

#include "array"
#include "print"
#include "utility"
#include "utils/stringify.hpp"
#include "vector"


enum class Skill : uint8_t {
	basic,
	dodge,
	dashAttack,
	dodgeCounter,
	assist,
	quickAssist,
	defensiveAssist,
	evasiveAssist,
	assistFollowup,
	special,
	exspecial,
	chain,
	ultimate,
	core,
	mindscape1,
	mindscape2,
	mindscape3,
	mindscape4,
	mindscape5,
	mindscape6,
};

enum class LevelableSkill : uint8_t {
	basic,
	dodge,
	assist,
	special,
	chain,
	core,
};

const inline std::vector<LevelableSkill> levelableSkills{
	LevelableSkill::basic,
	LevelableSkill::dodge,
	LevelableSkill::assist,
	LevelableSkill::special,
	LevelableSkill::chain,
	LevelableSkill::core,
};

template<class T>
struct Skills {
	using Type = T;

	T basic{};
	T dodge{};
	T assist{};
	T special{};
	T chain{};
	T core{};

	[[nodiscard]] const T &fromSkill(const Skill &t) const {
		switch (t) {
			case Skill::basic:
				return basic;
			case Skill::dodge:
			case Skill::dashAttack:
			case Skill::dodgeCounter:
				return dodge;
			case Skill::assist:
			case Skill::quickAssist:
			case Skill::defensiveAssist:
			case Skill::evasiveAssist:
			case Skill::assistFollowup:
				return assist;
			case Skill::special:
			case Skill::exspecial:
				return special;
			case Skill::chain:
			case Skill::ultimate:
				return chain;
			case Skill::core:
				return core;
			default:
				std::println("Warning: asked for a skill that isn't supported {}", static_cast<int>(t));
				return basic;
		}
	}

	[[nodiscard]] auto &&fromSkill(this auto &self, const LevelableSkill &t) {
		switch (t) {
			case LevelableSkill::basic:
				return self.basic;
			case LevelableSkill::dodge:
				return self.dodge;
			case LevelableSkill::assist:
				return self.assist;
			case LevelableSkill::special:
				return self.special;
			case LevelableSkill::chain:
				return self.chain;
			case LevelableSkill::core:
				return self.core;
			default:
				std::println("Warning: asked for a levelable skill that isn't supported {}", static_cast<int>(t));
				return self.basic;
		}
	}

	[[nodiscard]] static consteval auto getMembers() {
		return std::array{
			&T::basic,
			&T::dodge,
			&T::assist,
			&T::special,
			&T::chain,
			&T::core,
		};
	}
};

namespace Utils {
	template<>
	[[nodiscard]] inline std::string Stringify<LevelableSkill>(const LevelableSkill &skill) {
		switch (skill) {
			case LevelableSkill::basic:
				return "Basic";
			case LevelableSkill::dodge:
				return "Dodge";
			case LevelableSkill::assist:
				return "Assist";
			case LevelableSkill::special:
				return "Special";
			case LevelableSkill::chain:
				return "Chain";
			case LevelableSkill::core:
				return "Core";
		}
		std::unreachable();
	}
}// namespace Utils
