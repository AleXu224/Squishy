#include "weapons.hpp"

#include "weapon/weapons/StaffOfTheScarletSands.hpp"


void Weapon::initWeapons() {
	list = {
		{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
	};
}
