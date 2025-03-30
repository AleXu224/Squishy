#include "ScrollOfTheHeroOfCinderCity.hpp"

#include "artifact/setup.hpp"

static inline auto scrollOfTheHeroStrings = []() {
	std::unordered_map<Misc::Element, std::array<std::string, 4>> ret{};
	for (const auto &element: Misc::characterElements) {
		ret[element] = {
			fmt::format("scroll_{}", Utils::Stringify(element)),
			fmt::format("scroll_{}_nightsoul", Utils::Stringify(element)),
			fmt::format("After triggering a {} reaction", Utils::Stringify(element)),
			fmt::format("{} reaction triggered while in Nightsoul's Blessing state", Utils::Stringify(element)),
		};
	}
	return ret;
}();

const Artifact::Set Artifact::Sets::scrollOfTheHeroOfCinderCity{
	.key{15037},
	.goodKey = "ScrollOfTheHeroOfCinderCity",
	.name = "Scroll of the Hero of Cinder City",
	.setup = []() -> Set::Setup {
		auto anyOpt1 = IsActive(scrollOfTheHeroStrings.at(Misc::Element::pyro).at(0).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::hydro).at(0).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::cryo).at(0).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::electro).at(0).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::dendro).at(0).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::anemo).at(0).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::geo).at(0).c_str());
		auto anyOpt2 = IsActive(scrollOfTheHeroStrings.at(Misc::Element::pyro).at(1).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::hydro).at(1).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::cryo).at(1).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::electro).at(1).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::dendro).at(1).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::anemo).at(1).c_str())
					|| IsActive(scrollOfTheHeroStrings.at(Misc::Element::geo).at(1).c_str());
		Stats::ModsSheet mods{};

		auto getOptKeys = [](Misc::Element element) {
			return std::tuple{
				scrollOfTheHeroStrings.at(element)[0].c_str(),
				scrollOfTheHeroStrings.at(element)[1].c_str()
			};
		};

		auto makeFormulas = [&](Misc::Element element) {
			auto [optKey1, optKey2] = getOptKeys(element);

			return std::tuple{
				Requires(
					IsActive(optKey1),
					Constant(0.12f)
				),
				Requires(
					anyOpt1 && IsCharacterElement{element},
					Constant(0.12f)
				),
				Requires(
					IsActive(optKey1) && IsActive(optKey2),
					Constant(0.28)
				),
				Requires(
					anyOpt2 && IsCharacterElement{element},
					Constant(0.28f)
				)
			};
		};

		std::vector<Option::Types> opts{};
		for (const auto &element: Misc::characterElements) {
			auto [optKey1, optKey2] = getOptKeys(element);
			auto [formula1, formula2, formula3, formula4] = makeFormulas(element);
			Stats::ModsSheet optMods1{};
			Stats::ModsSheet optMods2{};

			for (const auto &element2: Misc::characterElements) {
				auto formula2_2 = Requires(
					IsActive(optKey1) && IsCharacterElement{element2},
					Constant(0.12f)
				);
				auto formula4_2 = Requires(
					IsActive(optKey2) && IsCharacterElement{element2},
					Constant(0.28f)
				);
				if (element == element2) {
					optMods1.teamPreMod.fromElement(element2).DMG = formula1 + formula2_2;
					optMods2.teamPreMod.fromElement(element2).DMG = formula3 + formula4_2;
				} else {
					optMods1.teamPreMod.fromElement(element2).DMG = formula2_2;
					optMods2.teamPreMod.fromElement(element2).DMG = formula4_2;
				}
			}
			opts.emplace_back(Option::Boolean{
				.key = optKey1,
				.name = scrollOfTheHeroStrings.at(element)[2],
				.teamBuff = true,
				.displayCondition = ElementCountOthers{element} >= 1 && !IsCharacterElement{element},
				.mods = optMods1,
			});
			opts.emplace_back(Option::Boolean{
				.key = optKey2,
				.name = scrollOfTheHeroStrings.at(element)[3],
				.teamBuff = true,
				.displayCondition = IsActive(optKey1) && ElementCountOthers{element} >= 1 && !IsCharacterElement{element},
				.mods = optMods2,
			});

			mods.teamPreMod.fromElement(element).DMG = formula1 + formula3 + formula2 + formula4;
		}

		return Set::Setup{
			.twoPc{},
			.fourPc{
				.opts = opts,
				.mods = mods,
			},
		};
	},
};
