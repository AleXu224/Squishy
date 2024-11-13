#include "weapons.hpp"

#include "weapon/weapons/StaffOfTheScarletSands.hpp"
#include "weapons/ApprenticeSNotes.hpp"
#include "weapons/BeginnerSProtector.hpp"
#include "weapons/DullBlade.hpp"
#include "weapons/HunterSBow.hpp"
#include "weapons/WasterGreatsword.hpp"


void Weapon::initWeapons() {
	list = {
		{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
		{Weapon::Datas::beginnerSProtector.key, Weapon::Datas::beginnerSProtector},
		{Weapon::Datas::apprenticeSNotes.key, Weapon::Datas::apprenticeSNotes},
		{Weapon::Datas::dullBlade.key, Weapon::Datas::dullBlade},
		{Weapon::Datas::hunterSBow.key, Weapon::Datas::hunterSBow},
		{Weapon::Datas::wasterGreatsword.key, Weapon::Datas::wasterGreatsword},
	};
}
