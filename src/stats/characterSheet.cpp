#include "characterSheet.hpp"

#include "formula/formula.hpp"
#include "stats/sheet.hpp"
#include "utils.hpp"


void Stats::CharacterSheet::init(Stats::Sheet &stats) {
	// HP
	baseHp.modifiers.at(3) = Formula::Custom("Character Base", [](const Stats::Sheet &stats) {
		return stats.character.base.getHpAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	hp.modifiers.at(3) = (Formula::Stat(Stat::hp_) + 1.f) * Formula::Stat(Stat::baseHp);

	// ATK
	baseAtk.modifiers.at(3) = Formula::Custom("Character Base", [](const Stats::Sheet &stats) {
		return stats.character.base.getAtkAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	atk.modifiers.at(3) = (Formula::Stat(Stat::atk_) + 1.f) * Formula::Stat(Stat::baseAtk);

	// DEF
	baseDef.modifiers.at(3) = Formula::Custom("Character Base", [](const Stats::Sheet &stats) {
		return stats.character.base.getDefAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	def.modifiers.at(3) = (Formula::Stat(Stat::def_) + 1.f) * Formula::Stat(Stat::baseDef);

	// Ascension stat
	stats.character.sheet.fromStat(stats.character.base.ascensionStat).modifiers.at(3) = Formula::Custom(
		"Character Base", [](const Stats::Sheet &stats) {
			return stats.character.base.getAscensionStatAt(stats.character.sheet.ascension);
		},
		Stats::isPercentage(stats.character.base.ascensionStat)
	);
	linkWeaponAndArtifacts(stats);
}

template<class T, size_t... Is>
consteval auto _iterate(const T &data, std::index_sequence<Is...>) {
	return std::tuple{
		[&]() consteval {
			auto wPtr = std::get<1>(data[Is]);
			auto aPtr = std::get<2>(data[Is]);
			return std::tuple{std::get<0>(data[Is]), Formula::StatPtr(wPtr) + Formula::StatPtr(aPtr)};
		}()...,
	};
}

template<size_t Size, class T>
consteval auto iterate(const T &data) {
	return _iterate(data, std::make_index_sequence<Size>());
}

[[nodiscard]] consteval auto getSheetValuesModifiers() {
	constexpr auto data = std::views::zip(
		Stats::getSheetValuesMembers<Stats::CharacterSheet>(),
		Stats::getSheetValuesMembers<Stats::WeaponSheet>(),
		Stats::getSheetValuesMembers<Stats::ArtifactSheet>()
	);
	return iterate<data.size()>(data);
}

template<class T, size_t... Is>
consteval auto _iterate2(const T &data, std::index_sequence<Is...>) {
	return std::tuple{
		[&]() consteval {
			auto [character, weapon, artifact] = data[Is];
			return std::tuple{
				std::get<0>(character),
				std::get<1>(character),
				Formula::SkillPtr<Stats::WeaponSheet>(std::get<1>(weapon), std::get<0>(weapon)) +
					Formula::SkillPtr<Stats::ArtifactSheet>(std::get<1>(artifact), std::get<0>(artifact)),
			};
		}()...,
	};
}

template<size_t Size, class T>
consteval auto iterate2(const T &data) {
	return _iterate2(data, std::make_index_sequence<Size>());
}

[[nodiscard]] consteval auto getSheetElementsModifiers() {
	constexpr auto data = std::views::zip(
		std::views::cartesian_product(
			Stats::CharacterSheet::_SkillValue::getMembers(),
			Stats::getSheetElementsMembers<Stats::CharacterSheet>()
		),
		std::views::cartesian_product(
			Stats::WeaponSheet::_SkillValue::getMembers(),
			Stats::getSheetElementsMembers<Stats::WeaponSheet>()
		),
		std::views::cartesian_product(
			Stats::ArtifactSheet::_SkillValue::getMembers(),
			Stats::getSheetElementsMembers<Stats::ArtifactSheet>()
		)
	);
	return iterate2<data.size()>(data);
}
[[nodiscard]] consteval auto getSheetSkillsModifiers() {
	constexpr auto data = std::views::zip(
		std::views::cartesian_product(
			Stats::CharacterSheet::_SkillValue::getMembers(),
			Stats::getSheetAttackSourceMembers<Stats::CharacterSheet>()
		),
		std::views::cartesian_product(
			Stats::WeaponSheet::_SkillValue::getMembers(),
			Stats::getSheetAttackSourceMembers<Stats::WeaponSheet>()
		),
		std::views::cartesian_product(
			Stats::ArtifactSheet::_SkillValue::getMembers(),
			Stats::getSheetAttackSourceMembers<Stats::ArtifactSheet>()
		)
	);
	return iterate2<data.size()>(data);
}

constexpr auto sheetValueModifiers = getSheetValuesModifiers();
constexpr auto sheetElementModifiers = getSheetElementsModifiers();
constexpr auto sheetSkillModifiers = getSheetSkillsModifiers();

void Stats::CharacterSheet::linkWeaponAndArtifacts(Stats::Sheet &stats) {
	squi::utils::iterateTuple(sheetValueModifiers, [&](const auto &lbd) {
		const auto &[cPtr, mod] = lbd;

		std::invoke(cPtr, stats.character.sheet).modifiers[2] = mod;
	});

	squi::utils::iterateTuple(sheetElementModifiers, [&](const auto &lbd) {
		const auto &[ssvPtr, cPtr, mod] = lbd;

		std::invoke(ssvPtr, std::invoke(cPtr, stats.character.sheet)).modifiers[2] = mod;
	});

	squi::utils::iterateTuple(sheetSkillModifiers, [&](const auto &lbd) {
		const auto &[ssvPtr, cPtr, mod] = lbd;

		std::invoke(ssvPtr, std::invoke(cPtr, stats.character.sheet)).modifiers[2] = mod;
	});
}
