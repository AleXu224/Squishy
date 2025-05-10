#include "sets.hpp"

#include "artifact/sets/ArchaicPetra.hpp"
#include "artifact/sets/BlizzardStrayer.hpp"
#include "artifact/sets/BloodstainedChivalry.hpp"
#include "artifact/sets/CrimsonWitchOfFlames.hpp"
#include "artifact/sets/DeepwoodMemories.hpp"
#include "artifact/sets/EmblemOfSeveredFate.hpp"
#include "artifact/sets/FinaleOfTheDeepGalleries.hpp"
#include "artifact/sets/FragmentOfHarmonicWhimsy.hpp"
#include "artifact/sets/GildedDreams.hpp"
#include "artifact/sets/GladiatorsFinale.hpp"
#include "artifact/sets/GoldenTroupe.hpp"
#include "artifact/sets/HeartOfDepth.hpp"
#include "artifact/sets/HuskOfOpulentDreams.hpp"
#include "artifact/sets/Lavawalker.hpp"
#include "artifact/sets/MaidenBeloved.hpp"
#include "artifact/sets/MarechausseeHunter.hpp"
#include "artifact/sets/NoblesseOblige.hpp"
#include "artifact/sets/ObsidianCodex.hpp"
#include "artifact/sets/OceanHuedClam.hpp"
#include "artifact/sets/PaleFlame.hpp"
#include "artifact/sets/ScrollOfTheHeroOfCinderCity.hpp"
#include "artifact/sets/ShimenawasReminiscence.hpp"
#include "artifact/sets/TenacityOfTheMillelith.hpp"
#include "artifact/sets/ThunderingFury.hpp"
#include "artifact/sets/UnfinishedReverie.hpp"
#include "artifact/sets/ViridescentVenerer.hpp"
#include "artifact/sets/WanderersTroupe.hpp"


void Artifact::initArtifacts() {
	sets = {
		{Artifact::Sets::archaicPetra.key, Artifact::Sets::archaicPetra},
		{Artifact::Sets::bloodstainedChivalry.key, Artifact::Sets::bloodstainedChivalry},
		{Artifact::Sets::blizzardStrayer.key, Artifact::Sets::blizzardStrayer},
		{Artifact::Sets::crimsonWitchOfFlames.key, Artifact::Sets::crimsonWitchOfFlames},
		{Artifact::Sets::deepwoodMemories.key, Artifact::Sets::deepwoodMemories},
		{Artifact::Sets::emblemOfSeveredFate.key, Artifact::Sets::emblemOfSeveredFate},
		{Artifact::Sets::finaleOfTheDeepGalleries.key, Artifact::Sets::finaleOfTheDeepGalleries},
		{Artifact::Sets::fragmentOfHarmonicWhimsy.key, Artifact::Sets::fragmentOfHarmonicWhimsy},
		{Artifact::Sets::gildedDreams.key, Artifact::Sets::gildedDreams},
		{Artifact::Sets::gladiatorsFinale.key, Artifact::Sets::gladiatorsFinale},
		{Artifact::Sets::goldenTroupe.key, Artifact::Sets::goldenTroupe},
		{Artifact::Sets::heartOfDepth.key, Artifact::Sets::heartOfDepth},
		{Artifact::Sets::huskOfOpulentDreams.key, Artifact::Sets::huskOfOpulentDreams},
		{Artifact::Sets::lavawalker.key, Artifact::Sets::lavawalker},
		{Artifact::Sets::maidenBeloved.key, Artifact::Sets::maidenBeloved},
		{Artifact::Sets::marechausseeHunter.key, Artifact::Sets::marechausseeHunter},
		{Artifact::Sets::noblesseOblige.key, Artifact::Sets::noblesseOblige},
		{Artifact::Sets::obsidianCodex.key, Artifact::Sets::obsidianCodex},
		{Artifact::Sets::oceanHuedClam.key, Artifact::Sets::oceanHuedClam},
		{Artifact::Sets::paleFlame.key, Artifact::Sets::paleFlame},
		{Artifact::Sets::scrollOfTheHeroOfCinderCity.key, Artifact::Sets::scrollOfTheHeroOfCinderCity},
		{Artifact::Sets::shimenawasReminiscence.key, Artifact::Sets::shimenawasReminiscence},
		{Artifact::Sets::tenacityOfTheMillelith.key, Artifact::Sets::tenacityOfTheMillelith},
		{Artifact::Sets::thunderingFury.key, Artifact::Sets::thunderingFury},
		{Artifact::Sets::unfinishedReverie.key, Artifact::Sets::unfinishedReverie},
		{Artifact::Sets::viridescentVenerer.key, Artifact::Sets::viridescentVenerer},
		{Artifact::Sets::wanderersTroupe.key, Artifact::Sets::wanderersTroupe},
	};
}
