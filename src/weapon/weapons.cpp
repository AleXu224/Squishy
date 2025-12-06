#include "weapons.hpp"

#include "weapon/weapons/AThousandFloatingDreams.hpp"
#include "weapon/weapons/PeakPatrolSong.hpp"
#include "weapon/weapons/SplendorOfTranquilWaters.hpp"
#include "weapon/weapons/StaffOfTheScarletSands.hpp"
#include "weapons/AThousandBlazingSuns.hpp"
#include "weapons/AmenomaKageuchi.hpp"
#include "weapons/ApprenticeSNotes.hpp"
#include "weapons/AquilaFavonia.hpp"
#include "weapons/AstralVulturesCrimsonPlumage.hpp"
#include "weapons/Azurelight.hpp"
#include "weapons/BeginnerSProtector.hpp"
#include "weapons/BloodsoakedRuins.hpp"
#include "weapons/CalamityQueller.hpp"
#include "weapons/Deathmatch.hpp"
#include "weapons/DullBlade.hpp"
#include "weapons/FavoniusCodex.hpp"
#include "weapons/FavoniusGreatsword.hpp"
#include "weapons/FavoniusLance.hpp"
#include "weapons/FavoniusSword.hpp"
#include "weapons/FavoniusWarbow.hpp"
#include "weapons/FracturedHalo.hpp"
#include "weapons/HunterSBow.hpp"
#include "weapons/MistsplitterReforged.hpp"
#include "weapons/NocturnesCurtainCall.hpp"
#include "weapons/SacrificialBow.hpp"
#include "weapons/SacrificialFragments.hpp"
#include "weapons/SacrificialGreatsword.hpp"
#include "weapons/SacrificialSword.hpp"
#include "weapons/StaffOfHoma.hpp"
#include "weapons/StarcallersWatch.hpp"
#include "weapons/SymphonistOfScents.hpp"
#include "weapons/ThrillingTalesOfDragonSlayers.hpp"
#include "weapons/TomeOfTheEternalFlow.hpp"
#include "weapons/WasterGreatsword.hpp"


void Weapon::initWeapons() {
	list = {
		{Weapon::Datas::aThousandFloatingDreams.key, Weapon::Datas::aThousandFloatingDreams},
		{Weapon::Datas::peakPatrolSong.key, Weapon::Datas::peakPatrolSong},
		{Weapon::Datas::splendorOfTranquilWaters.key, Weapon::Datas::splendorOfTranquilWaters},
		{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
		{Weapon::Datas::aThousandBlazingSuns.key, Weapon::Datas::aThousandBlazingSuns},
		{Weapon::Datas::amenomaKageuchi.key, Weapon::Datas::amenomaKageuchi},
		{Weapon::Datas::apprenticeSNotes.key, Weapon::Datas::apprenticeSNotes},
		{Weapon::Datas::aquilaFavonia.key, Weapon::Datas::aquilaFavonia},
		{Weapon::Datas::astralVulturesCrimsonPlumage.key, Weapon::Datas::astralVulturesCrimsonPlumage},
		{Weapon::Datas::azurelight.key, Weapon::Datas::azurelight},
		{Weapon::Datas::beginnerSProtector.key, Weapon::Datas::beginnerSProtector},
		{Weapon::Datas::bloodsoakedRuins.key, Weapon::Datas::bloodsoakedRuins},
		{Weapon::Datas::calamityQueller.key, Weapon::Datas::calamityQueller},
		{Weapon::Datas::deathmatch.key, Weapon::Datas::deathmatch},
		{Weapon::Datas::dullBlade.key, Weapon::Datas::dullBlade},
		{Weapon::Datas::favoniusCodex.key, Weapon::Datas::favoniusCodex},
		{Weapon::Datas::favoniusGreatsword.key, Weapon::Datas::favoniusGreatsword},
		{Weapon::Datas::favoniusLance.key, Weapon::Datas::favoniusLance},
		{Weapon::Datas::favoniusSword.key, Weapon::Datas::favoniusSword},
		{Weapon::Datas::favoniusWarbow.key, Weapon::Datas::favoniusWarbow},
		{Weapon::Datas::fracturedHalo.key, Weapon::Datas::fracturedHalo},
		{Weapon::Datas::hunterSBow.key, Weapon::Datas::hunterSBow},
		{Weapon::Datas::mistsplitterReforged.key, Weapon::Datas::mistsplitterReforged},
		{Weapon::Datas::nocturnesCurtainCall.key, Weapon::Datas::nocturnesCurtainCall},
		{Weapon::Datas::sacrificialBow.key, Weapon::Datas::sacrificialBow},
		{Weapon::Datas::sacrificialFragments.key, Weapon::Datas::sacrificialFragments},
		{Weapon::Datas::sacrificialGreatsword.key, Weapon::Datas::sacrificialGreatsword},
		{Weapon::Datas::sacrificialSword.key, Weapon::Datas::sacrificialSword},
		{Weapon::Datas::staffOfHoma.key, Weapon::Datas::staffOfHoma},
		{Weapon::Datas::starcallersWatch.key, Weapon::Datas::starcallersWatch},
		{Weapon::Datas::symphonistOfScents.key, Weapon::Datas::symphonistOfScents},
		{Weapon::Datas::thrillingTalesOfDragonSlayers.key, Weapon::Datas::thrillingTalesOfDragonSlayers},
		{Weapon::Datas::tomeOfTheEternalFlow.key, Weapon::Datas::tomeOfTheEternalFlow},
		{Weapon::Datas::wasterGreatsword.key, Weapon::Datas::wasterGreatsword},
	};
}
