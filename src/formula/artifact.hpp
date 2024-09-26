#pragma once

#include "artifact/set.hpp"// IWYU pragma: keep
#include "cstdint"
#include "formula/formulaContext.hpp"
#include "stats/artifactSheet.hpp"
#include "stats/loadout.hpp"
#include "stats/stat.hpp"
#include "step.hpp"


namespace Formula {
	struct ArtifactMainStat {
		::Stat stat;
		uint8_t level;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &) const {
			return Stats::Values::mainStat.at(stat).at(level);
		}
	};

	struct ArtifactSubStat {
		StatValue stat;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isPercentage(stat.stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &) const {
			return stat.value;
		}
	};

	template<class T>
	struct ArtifactModStatptr {
		T Stats::ModsSheet::*location;
		T::_Value T::*stat;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isSheetMemberPercentage(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto val1 = context.target.artifact.bonus1
							.transform([&](auto &&val) {
								const auto &frm = std::invoke(stat, std::invoke(location, val.bonusPtr.mods));
								return frm.hasValue() ? frm.eval(context) : 0.f;
							})
							.value_or(0.f);
			auto val2 = context.target.artifact.bonus2
							.transform([&](auto &&val) {
								const auto &frm = std::invoke(stat, std::invoke(location, val.bonusPtr.mods));
								return frm.hasValue() ? frm.eval(context) : 0.f;
							})
							.value_or(0.f);

			return val1 + val2;
		}
	};

	template<class T>
	struct ArtifactModSkillptr {
		T Stats::ModsSheet::*location;
		T::_SkillValue T::*skill;
		T::_Value T::_SkillValue::*stat;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = T::_SkillValue::isPercetange(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto val1 = context.target.artifact.bonus1
							.transform([&](auto &&val) {
								const auto &frm = std::invoke(stat, std::invoke(skill, std::invoke(location, val.bonusPtr.mods)));
								return frm.hasValue() ? frm.eval(context) : 0.f;
							})
							.value_or(0.f);
			auto val2 = context.target.artifact.bonus2
							.transform([&](auto &&val) {
								const auto &frm = std::invoke(stat, std::invoke(skill, std::invoke(location, val.bonusPtr.mods)));
								return frm.hasValue() ? frm.eval(context) : 0.f;
							})
							.value_or(0.f);

			return val1 + val2;
		}
	};

	struct ArtifactModTalent {
		Stats::ModsSheet::_Talents Stats::ModsSheet::*location;
		Stats::ModsSheet::_Talents::Type Stats::ModsSheet::_Talents::*talent;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Artifact {} {}", Utils::Stringify(talent), eval(context));
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto val1 = context.target.artifact.bonus1
							.transform([&](auto &&val) {
								const auto &frm = std::invoke(talent, std::invoke(location, val.bonusPtr.mods));
								return frm.hasValue() ? frm.eval(context) : 0.f;
							})
							.value_or(0.f);
			auto val2 = context.target.artifact.bonus2
							.transform([&](auto &&val) {
								const auto &frm = std::invoke(talent, std::invoke(location, val.bonusPtr.mods));
								return frm.hasValue() ? frm.eval(context) : 0.f;
							})
							.value_or(0.f);

			return val1 + val2;
		}
	};

	struct ArtifactStatPtr {
		using T = std::remove_pointer_t<decltype(Stats::ArtifactSheet::equippedArtifacts)::value_type::value_type>;
		T::_Value T::*stat;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = Stats::isSheetMemberPercentage(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			float sum = 0;
			for (const auto &val: context.target.artifact.sheet.equippedArtifacts) {
				sum += val.transform([&](auto &&val) {
							  return std::invoke(stat, *val);
						  })
						   .value_or(0.f);
			}

			return sum;
		}
	};
	struct ArtifactSkillPtr {
		using T = std::remove_pointer_t<decltype(Stats::ArtifactSheet::equippedArtifacts)::value_type::value_type>;
		T::_SkillValue T::*skill;
		T::_Value T::_SkillValue::*stat;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = T::_SkillValue::isPercetange(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			float sum = 0;
			for (const auto &val: context.target.artifact.sheet.equippedArtifacts) {
				sum += val.transform([&](auto &&val) {
							  return std::invoke(stat, std::invoke(skill, *val));
						  })
						   .value_or(0.f);
			}

			return sum;
		}
	};
}// namespace Formula