#include "weapons.hpp"

#include "weapon/weapons/AThousandFloatingDreams.hpp"
#include "weapon/weapons/PeakPatrolSong.hpp"
#include "weapon/weapons/SplendorOfTranquilWaters.hpp"
#include "weapon/weapons/StaffOfTheScarletSands.hpp"
#include "weapons/AmenomaKageuchi.hpp"
#include "weapons/ApprenticeSNotes.hpp"
#include "weapons/BeginnerSProtector.hpp"
#include "weapons/DullBlade.hpp"
#include "weapons/FavoniusCodex.hpp"
#include "weapons/FavoniusGreatsword.hpp"
#include "weapons/FavoniusLance.hpp"
#include "weapons/FavoniusSword.hpp"
#include "weapons/FavoniusWarbow.hpp"
#include "weapons/HunterSBow.hpp"
#include "weapons/MistsplitterReforged.hpp"
#include "weapons/SacrificialBow.hpp"
#include "weapons/SacrificialFragments.hpp"
#include "weapons/SacrificialGreatsword.hpp"
#include "weapons/SacrificialSword.hpp"
#include "weapons/StarcallersWatch.hpp"
#include "weapons/ThrillingTalesOfDragonSlayers.hpp"
#include "weapons/WasterGreatsword.hpp"


void Weapon::initWeapons() {
	list = {
		{Weapon::Datas::aThousandFloatingDreams.key, Weapon::Datas::aThousandFloatingDreams},
		{Weapon::Datas::peakPatrolSong.key, Weapon::Datas::peakPatrolSong},
		{Weapon::Datas::splendorOfTranquilWaters.key, Weapon::Datas::splendorOfTranquilWaters},
		{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
		{Weapon::Datas::amenomaKageuchi.key, Weapon::Datas::amenomaKageuchi},
		{Weapon::Datas::apprenticeSNotes.key, Weapon::Datas::apprenticeSNotes},
		{Weapon::Datas::beginnerSProtector.key, Weapon::Datas::beginnerSProtector},
		{Weapon::Datas::dullBlade.key, Weapon::Datas::dullBlade},
		{Weapon::Datas::favoniusCodex.key, Weapon::Datas::favoniusCodex},
		{Weapon::Datas::favoniusGreatsword.key, Weapon::Datas::favoniusGreatsword},
		{Weapon::Datas::favoniusLance.key, Weapon::Datas::favoniusLance},
		{Weapon::Datas::favoniusSword.key, Weapon::Datas::favoniusSword},
		{Weapon::Datas::favoniusWarbow.key, Weapon::Datas::favoniusWarbow},
		{Weapon::Datas::hunterSBow.key, Weapon::Datas::hunterSBow},
		{Weapon::Datas::mistsplitterReforged.key, Weapon::Datas::mistsplitterReforged},
		{Weapon::Datas::sacrificialBow.key, Weapon::Datas::sacrificialBow},
		{Weapon::Datas::sacrificialFragments.key, Weapon::Datas::sacrificialFragments},
		{Weapon::Datas::sacrificialGreatsword.key, Weapon::Datas::sacrificialGreatsword},
		{Weapon::Datas::sacrificialSword.key, Weapon::Datas::sacrificialSword},
		{Weapon::Datas::starcallersWatch.key, Weapon::Datas::starcallersWatch},
		{Weapon::Datas::thrillingTalesOfDragonSlayers.key, Weapon::Datas::thrillingTalesOfDragonSlayers},
		{Weapon::Datas::wasterGreatsword.key, Weapon::Datas::wasterGreatsword},
	};
}
