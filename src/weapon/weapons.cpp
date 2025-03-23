#include "weapons.hpp"

#include "weapon/weapons/AThousandFloatingDreams.hpp"
#include "weapon/weapons/PeakPatrolSong.hpp"
#include "weapon/weapons/SplendorOfTranquilWaters.hpp"
#include "weapon/weapons/StaffOfTheScarletSands.hpp"
#include "weapons/ApprenticeSNotes.hpp"
#include "weapons/BeginnerSProtector.hpp"
#include "weapons/DullBlade.hpp"
#include "weapons/FavoniusCodex.hpp"
#include "weapons/FavoniusGreatsword.hpp"
#include "weapons/FavoniusLance.hpp"
#include "weapons/FavoniusSword.hpp"
#include "weapons/FavoniusWarbow.hpp"
#include "weapons/HunterSBow.hpp"
#include "weapons/WasterGreatsword.hpp"


void Weapon::initWeapons() {
	list = {
		{Weapon::Datas::aThousandFloatingDreams.key, Weapon::Datas::aThousandFloatingDreams},
		{Weapon::Datas::peakPatrolSong.key, Weapon::Datas::peakPatrolSong},
		{Weapon::Datas::splendorOfTranquilWaters.key, Weapon::Datas::splendorOfTranquilWaters},
		{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
		{Weapon::Datas::beginnerSProtector.key, Weapon::Datas::beginnerSProtector},
		{Weapon::Datas::apprenticeSNotes.key, Weapon::Datas::apprenticeSNotes},
		{Weapon::Datas::dullBlade.key, Weapon::Datas::dullBlade},
		{Weapon::Datas::favoniusCodex.key, Weapon::Datas::favoniusCodex},
		{Weapon::Datas::favoniusGreatsword.key, Weapon::Datas::favoniusGreatsword},
		{Weapon::Datas::favoniusLance.key, Weapon::Datas::favoniusLance},
		{Weapon::Datas::favoniusSword.key, Weapon::Datas::favoniusSword},
		{Weapon::Datas::favoniusWarbow.key, Weapon::Datas::favoniusWarbow},
		{Weapon::Datas::hunterSBow.key, Weapon::Datas::hunterSBow},
		{Weapon::Datas::wasterGreatsword.key, Weapon::Datas::wasterGreatsword},
	};
}
