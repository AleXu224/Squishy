#include "characterSheet.hpp"

#include "formula/formula.hpp"
#include "stats/sheet.hpp"
#include "utils.hpp"


void Stats::CharacterSheet::init(Stats::Sheet &stats) {
	// HP
	addModifier(
		baseHp,
		Formula::Custom("Character Base", [](const Stats::Sheet &stats) {
			return stats.character.base.getHpAt(stats.character.sheet.level, stats.character.sheet.ascension);
		})
	);
	addModifier(stats.character.sheet.hp, (Formula::Stat(Stat::hp_) + 1.f) * Formula::Stat(Stat::baseHp));

	// ATK
	addModifier(
		baseAtk,
		Formula::Custom("Character Base", [](const Stats::Sheet &stats) {
			return stats.character.base.getAtkAt(stats.character.sheet.level, stats.character.sheet.ascension);
		})
	);
	addModifier(stats.character.sheet.atk, (Formula::Stat(Stat::atk_) + 1.f) * Formula::Stat(Stat::baseAtk));

	// DEF
	addModifier(
		baseDef,
		Formula::Custom("Character Base", [](const Stats::Sheet &stats) {
			return stats.character.base.getDefAt(stats.character.sheet.level, stats.character.sheet.ascension);
		})
	);
	addModifier(stats.character.sheet.def, (Formula::Stat(Stat::def_) + 1.f) * Formula::Stat(Stat::baseDef));

	// Ascension stat
	addModifier(
		stats.character.sheet.fromStat(stats.character.base.ascensionStat),
		Formula::Custom(
			"Character Base", [](const Stats::Sheet &stats) {
				return stats.character.base.getAscensionStatAt(stats.character.sheet.ascension);
			},
			Stats::isPercentage(stats.character.base.ascensionStat)
		)
	);

	linkWeaponAndArtifacts(stats);
}

void addModifierD(Stats::Sheet &stats, auto &cPtr, const auto &mod) {
	auto &cStat = std::invoke(cPtr, stats.character.sheet);
	Stats::addModifier(cStat, mod);
}

template<class T, class U, class V, size_t... I>
[[nodiscard]] consteval auto _combineArrays(T r1, U r2, V r3, std::index_sequence<I...>) {
	return std::array{std::tuple{r1[I], r2[I], r3[I]}...};
};

template<class T, class U, class V>
[[nodiscard]] consteval auto combineArrays(T r1, U r2, V r3) {
	return _combineArrays<T, U, V>(r1, r2, r3, std::make_index_sequence<r1.size()>());
};

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
			auto ssvPtr = std::get<0>(data[Is]);
			auto dataPtrs = std::get<1>(data[Is]);
			auto wPtr = std::get<1>(dataPtrs);
			auto aPtr = std::get<2>(dataPtrs);
			return std::tuple{std::get<0>(dataPtrs), ssvPtr, Formula::SkillPtr(wPtr, ssvPtr) + Formula::SkillPtr(aPtr, ssvPtr)};
		}()...,
	};
}

template<size_t Size, class T>
consteval auto iterate2(const T &data) {
	return _iterate2(data, std::make_index_sequence<Size>());
}

[[nodiscard]] consteval auto getSheetElementsModifiers() {
	constexpr auto data = std::views::cartesian_product(
		Stats::SSV::getMembers(),
		std::views::zip(
			Stats::getSheetElementsMembers<Stats::CharacterSheet>(),
			Stats::getSheetElementsMembers<Stats::WeaponSheet>(),
			Stats::getSheetElementsMembers<Stats::ArtifactSheet>()
		)
	);
	return iterate2<data.size()>(data);
}
[[nodiscard]] consteval auto getSheetSkillsModifiers() {
	constexpr auto data = std::views::cartesian_product(
		Stats::SSV::getMembers(),
		std::views::zip(
			Stats::getSheetAttackSourceMembers<Stats::CharacterSheet>(),
			Stats::getSheetAttackSourceMembers<Stats::WeaponSheet>(),
			Stats::getSheetAttackSourceMembers<Stats::ArtifactSheet>()
		)
	);
	return iterate2<data.size()>(data);
}

void Stats::CharacterSheet::linkWeaponAndArtifacts(Stats::Sheet &stats) {
	constexpr auto sheetValueModifiers = getSheetValuesModifiers();
	squi::utils::iterateTuple(sheetValueModifiers, [&](const auto &lbd) {
		const auto &[cPtr, mod] = lbd;

		addModifier(std::invoke(cPtr, stats.character.sheet), mod);
	});

	constexpr auto sheetElementModifiers = getSheetElementsModifiers();
	squi::utils::iterateTuple(sheetElementModifiers, [&](const auto &lbd) {
		const auto &[cPtr, ssvPtr, mod] = lbd;

		addModifier(std::invoke(ssvPtr, std::invoke(cPtr, stats.character.sheet)), mod);
	});

	constexpr auto sheetSkillModifiers = getSheetSkillsModifiers();
	squi::utils::iterateTuple(sheetSkillModifiers, [&](const auto &lbd) {
		const auto &[cPtr, ssvPtr, mod] = lbd;

		addModifier(std::invoke(ssvPtr, std::invoke(cPtr, stats.character.sheet)), mod);
	});
}
