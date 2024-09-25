#include "artifactSheet.hpp"

#include "formula/artifact.hpp"

namespace {
	template<class A, class M>
	constexpr auto getMods(A Stats::ArtifactSheet::*, M Stats::ModsSheet::*location2) {
		constexpr auto tr = [](auto &&arg) {
			const auto &[location, tp] = arg;
			return Formula::ArtifactModStatptr{.location = location, .stat = tp};
		};
		constexpr auto tr2 = [](auto &&arg) {
			const auto &[location, tp] = arg;
			return Formula::ArtifactModSkillptr{.location = location, .skill = std::get<0>(tp), .stat = std::get<1>(tp)};
		};

		auto vals = std::array{location2};

		return std::tuple{
			std::views::zip(
				Stats::getSheetValuesMembers<A>(),
				std::views::transform(
					std::views::cartesian_product(vals, Stats::getSheetValuesMembers<M>()),
					tr
				)
			) | std::ranges::to<std::vector>(),
			std::views::zip(
				std::views::cartesian_product(Stats::getSheetSkillsMembers<A>(), A::_SkillValue::getMembers()),
				std::views::transform(
					std::views::cartesian_product(vals, std::views::cartesian_product(Stats::getSheetSkillsMembers<M>(), M::_SkillValue::getMembers())),
					tr2
				)
			) | std::ranges::to<std::vector>(),
		};
	}

	template<class A, class M>
	constexpr auto getTalents(A Stats::ArtifactSheet::*, M Stats::ModsSheet::*location2) {
		constexpr auto tr = [](auto &&arg) {
			const auto &[location, tp] = arg;
			return Formula::ArtifactModTalent{.location = location, .talent = tp};
		};

		auto vals = std::array{location2};

		return std::views::zip(
				   A::getMembers(),
				   std::views::transform(
					   std::views::cartesian_product(vals, M::getMembers()),
					   tr
				   )
			   )
			 | std::ranges::to<std::vector>();
	}

	constexpr auto getSubs() {
		constexpr auto tr = [](auto &&arg) {
			return Formula::ArtifactStatPtr{.stat = arg};
		};
		constexpr auto tr2 = [](auto &&arg) {
			const auto &[skill, stat] = arg;
			return Formula::ArtifactSkillPtr{.skill = skill, .stat = stat};
		};

		using TT = std::remove_pointer_t<decltype(Stats::ArtifactSheet::equippedArtifacts)::value_type::value_type>;
		return std::tuple{
			std::views::zip(
				Stats::getSheetValuesMembers<decltype(Stats::ArtifactSheet::preMods)>(),
				std::views::transform(
					Stats::getSheetValuesMembers<TT>(),
					tr
				)
			) | std::ranges::to<std::vector>(),
			std::views::zip(
				std::views::cartesian_product(
					Stats::getSheetSkillsMembers<decltype(Stats::ArtifactSheet::preMods)>(),
					decltype(Stats::ArtifactSheet::preMods)::_SkillValue::getMembers()
				),
				std::views::transform(
					std::views::cartesian_product(
						Stats::getSheetSkillsMembers<TT>(),
						TT::_SkillValue::getMembers()
					),
					tr2
				)
			) | std::ranges::to<std::vector>()
		};
	}
}// namespace

void Stats::ArtifactSheet::init() {
	auto initSheet = [&](auto &&location, auto &&vals) {
		const auto &[statData, skillData] = vals;
		for (const auto &[stat, formula]: statData) {
			std::invoke(stat, std::invoke(location, *this)).modifiers.at(0) = formula;
		}
		for (const auto &[oof, formula]: skillData) {
			const auto &[skill, stat] = oof;
			std::invoke(stat, std::invoke(skill, std::invoke(location, *this))).modifiers.at(0) = formula;
		}
	};
	initSheet(&Stats::ArtifactSheet::preMods, getMods(&ArtifactSheet::preMods, &ModsSheet::preMod));
	initSheet(&Stats::ArtifactSheet::postMods, getMods(&ArtifactSheet::postMods, &ModsSheet::postMod));
	initSheet(&Stats::ArtifactSheet::teamPreMods, getMods(&ArtifactSheet::teamPreMods, &ModsSheet::teamPreMod));
	initSheet(&Stats::ArtifactSheet::teamPostMods, getMods(&ArtifactSheet::teamPostMods, &ModsSheet::teamPostMod));
	initSheet(&Stats::ArtifactSheet::enemySheet, getMods(&ArtifactSheet::enemySheet, &ModsSheet::enemy));

	auto initSheetTalent = [&](auto &&location, auto &&vals) {
		for (const auto &[stat, formula]: vals) {
			std::invoke(stat, std::invoke(location, *this)).modifiers.at(0) = formula;
		}
	};

	initSheetTalent(&Stats::ArtifactSheet::talents, getTalents(&Stats::ArtifactSheet::talents, &ModsSheet::talents));
	initSheetTalent(&Stats::ArtifactSheet::teamTalents, getTalents(&Stats::ArtifactSheet::teamTalents, &ModsSheet::teamTalents));

	auto initSheetSubs = [&](auto &&vals) {
		const auto &[statData, skillData] = vals;
		for (const auto &[stat, formula]: statData) {
			std::invoke(stat, this->preMods).modifiers.at(1) = formula;
		}
		for (const auto &[oof, formula]: skillData) {
			const auto &[skill, stat] = oof;
			std::invoke(stat, std::invoke(skill, this->preMods)).modifiers.at(1) = formula;
		}
	};

	initSheetSubs(getSubs());
}
