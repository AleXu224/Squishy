import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { Weapon } from "./weaponType.d.ts";
import { camelCase } from "https://deno.land/x/case@2.2.0/mod.ts";

if (Deno.args[0] == undefined) {
	console.error("Usage: weaponGenerator.ts <weapon id>\neg: deno run .\\scripts\\weaponGenerator.ts 13101");
	Deno.exit(1);
}

try {
	const cmakeFile = Deno.openSync("./CMakeLists.txt");
	cmakeFile.close();
} catch (_error) {
	console.error("You must run this script from the project root directory (where main.cpp and CMakeLists.txt reside)");
	Deno.exit(1);
}

const response = await fetch(`https://api.hakush.in/gi/data/en/weapon/${Deno.args[0]}.json`);
if (!response.ok) {
	console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: Weapon = await response.json();

const weaponType = new Map<string, string>([
	["WEAPON_POLE", "Misc::WeaponType::polearm"],
	["WEAPON_BOW", "Misc::WeaponType::bow"],
	["WEAPON_CATALYST", "Misc::WeaponType::catalyst"],
	["WEAPON_CLAYMORE", "Misc::WeaponType::claymore"],
	["WEAPON_SWORD_ONE_HAND", "Misc::WeaponType::sword"],
]);

const stat = new Map<string, string>([
	["FIGHT_PROP_HP_PERCENT", "hp_"],
	["FIGHT_PROP_ATTACK_PERCENT", "atk_"],
	["FIGHT_PROP_DEFENSE_PERCENT", "def_"],
	["FIGHT_PROP_ELEMENT_MASTERY", "em"],
	["FIGHT_PROP_CHARGE_EFFICIENCY", "er"],
	["FIGHT_PROP_HEAL_ADD", "hb"],
	["FIGHT_PROP_CRITICAL_HURT", "cd"],
	["FIGHT_PROP_CRITICAL", "cr"],
	["FIGHT_PROP_PHYSICAL_ADD_HURT", "physicalDmg,"],
	["FIGHT_PROP_GRASS_ADD_HURT", "dendroDmg,"],
	["FIGHT_PROP_ROCK_ADD_HURT", "geoDmg,"],
	["FIGHT_PROP_WIND_ADD_HURT", "anemoDmg,"],
	["FIGHT_PROP_WATER_ADD_HURT", "hydroDmg,"],
	["FIGHT_PROP_ICE_ADD_HURT", "cryoDmg,"],
	["FIGHT_PROP_ELEC_ADD_HURT", "electroDmg,"],
	["FIGHT_PROP_FIRE_ADD_HURT", "pyroDmg,"],
]);

const data = {
	key: Deno.args[0],
	name: contents.Name,
	type: weaponType.get(contents.WeaponType),
	growCurve: contents.WeaponProp[0].type.replace("GROW_CURVE_", ""),
	hasSubstat: Object.entries(contents.StatsModifier).length > 1,
	subStat: stat.get(Object.keys(contents.StatsModifier)[1]),
	subStatValue: Object.values(contents.StatsModifier)[1]?.Base,
	subStatCurve: contents.WeaponProp[1].type.replace("GROW_CURVE_", ""),
	icon: `https://api.hakush.in/gi/UI/${contents.Icon}.webp`,
	iconAwaken: `https://api.hakush.in/gi/UI/${contents.Icon}_Awaken.webp`,
};

const subStatStr = `.subStat = SubStat{
			.stat{
				.stat = Stat::${data.subStat},
				.value = ${data.subStatValue?.toFixed(3)},
			},
			.curve = Curves::WeaponGrow::${data.subStatCurve},
		},`;

const ascensionUpgrade = new Float32Array(7);

for (const [key, value] of Object.entries(contents.Ascension)) {
	ascensionUpgrade[parseInt(key)] = value.FIGHT_PROP_BASE_ATTACK;
}

const ascensionUpgradeStr: string[] = [];

for (const val of ascensionUpgrade) {
	ascensionUpgradeStr.push(val.toFixed(1));
}


const retHeader: string = `#pragma once

#include "weapon/data.hpp"

namespace Weapon::Datas {
	const extern Weapon::Data ${camelCase(data.name)};
}// namespace Weapon::Datas
`;

const retSource: string = `#include "${pascalCase(data.name)}.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::${camelCase(data.name)}{
	.key{${data.key}},
	.name = "${data.name}",
	.baseStats{
		.type = ${data.type},
		.rarity = ${contents.Rarity},
		.baseAtk = ${contents.StatsModifier.ATK.Base.toFixed(3)},
		.atkCurve = Curves::WeaponGrow::${data.growCurve},
		${data.hasSubstat ? subStatStr : ".subStat{},"}
		.ascensionUpgrade{${ascensionUpgradeStr.join(", ")}}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
`;

console.log(retHeader);

console.log(data.icon);
console.log(data.iconAwaken);

Deno.mkdirSync(`./assets/Weapons/${data.name}`, { recursive: true });

const iconResponse = await fetch(data.icon);
if (!iconResponse.ok) {
	console.log(`Failed getting the icon with code ${iconResponse.status}, "${iconResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Weapons/${data.name}/icon.webp`, await iconResponse.bytes());

const iconResponseAwaken = await fetch(data.iconAwaken);
if (!iconResponseAwaken.ok) {
	console.log(`Failed getting the icon with code ${iconResponseAwaken.status}, "${iconResponseAwaken.statusText}" (${data.iconAwaken})`);
}
Deno.writeFileSync(`./assets/Weapons/${data.name}/icon_ascended.webp`, await iconResponseAwaken.bytes());

Deno.writeFileSync(`./src/weapon/weapons/${pascalCase(data.name)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src/weapon/weapons/${pascalCase(data.name)}.cpp`, new TextEncoder().encode(retSource));