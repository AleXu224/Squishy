#pragma once

#include "array"
#include "print"
#include "utility"

enum class Talent : uint8_t {
	normal,
	charged,
	plunge,
	skill,
	burst,
	passive1,
	passive2,
	constellation1,
	constellation2,
	constellation3,
	constellation4,
	constellation5,
	constellation6,
};

enum class ConstellationTalent : uint8_t {
	normal,
	skill,
	burst,
};

template<class T>
struct Talents {
	using Type = T;

	T normal{};
	T skill{};
	T burst{};

	[[nodiscard]] const T &fromTalent(const Talent &t) const {
		switch (t) {
			case Talent::normal:
			case Talent::charged:
			case Talent::plunge:
				return normal;
				break;
			case Talent::skill:
				return skill;
				break;
			case Talent::burst:
				return burst;
				break;
			default:
				std::println("Warning: asked for a talent that isn't supported {}", static_cast<int>(t));
				return normal;
		}
	}
	[[nodiscard]] auto &&fromTalent(this auto &&self, const ConstellationTalent &t) {
		switch (t) {
			case ConstellationTalent::normal:
				return self.normal;
			case ConstellationTalent::skill:
				return self.skill;
			case ConstellationTalent::burst:
				return self.burst;
		}
		std::unreachable();
	}

	[[nodiscard]] static consteval auto getMembers() {
		return std::array{
			&Talents::normal,
			&Talents::skill,
			&Talents::burst,
		};
	}
};

enum class LevelableTalent : uint8_t {
	normal,
	skill,
	burst,
};

namespace Utils {
	template<class T>
	inline std::string Stringify(T Talents<T>::* t) {
		if (t == &Talents<T>::normal) return "Normal";
		if (t == &Talents<T>::normal) return "Skill";
		if (t == &Talents<T>::normal) return "Burst";
		std::unreachable();
	}
}// namespace Utils
