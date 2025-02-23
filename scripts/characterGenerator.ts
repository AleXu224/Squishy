import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { Character, Skill } from "./characterType.d.ts";
import { camelCase } from "https://deno.land/x/case@2.2.0/mod.ts";
import { growCurve, propToStat, rarity, weaponType } from "./mapped.ts";
import lowerCase from "https://deno.land/x/case@2.2.0/lowerCase.ts";

if (Deno.args[0] == undefined) {
    console.error("Usage: characterGenerator.ts <characer id>\neg: deno run .\\scripts\\characterGenerator.ts 10000089");
    Deno.exit(1);
}

try {
    const cmakeFile = Deno.openSync("./CMakeLists.txt");
    cmakeFile.close();
} catch (_error) {
    console.error("You must run this script from the project root directory (where main.cpp and CMakeLists.txt reside)");
    Deno.exit(1);
}

const response = await fetch(`https://api.hakush.in/gi/data/en/character/${Deno.args[0]}.json`);
if (!response.ok) {
    console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: Character = await response.json();

const data = {
    key: Deno.args[0],
    name: contents.Name,
    ascensionStat: propToStat.get(Object.keys(contents.StatsModifier.Ascension[0])[3]),
    icon: `https://api.hakush.in/gi/UI/${contents.Icon}.webp`,
    namecardIcon: `https://api.hakush.in/gi/UI/${contents.CharaInfo.Namecard.Icon}.webp`,
};

const ascensionUpgradeHp = new Float32Array(7);
ascensionUpgradeHp[0] = 0;
for (const [key, value] of Object.entries(contents.StatsModifier.Ascension)) {
    ascensionUpgradeHp[parseInt(key) + 1] = value["FIGHT_PROP_BASE_HP"];
}

const ascensionUpgradeDef = new Float32Array(7);
ascensionUpgradeDef[0] = 0;
for (const [key, value] of Object.entries(contents.StatsModifier.Ascension)) {
    ascensionUpgradeDef[parseInt(key) + 1] = value["FIGHT_PROP_BASE_DEFENSE"];
}

const ascensionUpgradeAtk = new Float32Array(7);
ascensionUpgradeAtk[0] = 0;
for (const [key, value] of Object.entries(contents.StatsModifier.Ascension)) {
    ascensionUpgradeAtk[parseInt(key) + 1] = value["FIGHT_PROP_BASE_ATTACK"];
}

const ascensionUpgradeAscensionStat = new Float32Array(7);
ascensionUpgradeAscensionStat[0] = 0;
for (const [key, value] of Object.entries(contents.StatsModifier.Ascension)) {
    ascensionUpgradeAscensionStat[parseInt(key) + 1] = value[Object.keys(value)[3]];
}

function strGenerator(entries: Float32Array): string[] {
    const ret: string[] = [];
    for (const val of entries) {
        ret.push(val.toFixed(3));
    }
    return ret;
}

function skillContentsGenerator(skill: Skill, source: string): string {
    let ret: string = "";

    for (const [_, value] of Object.entries(skill.Promote[0].Desc)) {
        if (value == "") break;
        const name = value.split("|")[0];
        const paramsIndexes = value.match(/(?<=param)[0-9]+/g);
        console.log(paramsIndexes);
        if (paramsIndexes === null) continue;
        for (const index of paramsIndexes) {
            ret += `
					Node::Atk{
						.name = "${name}",
						.source = Misc::AttackSource::${source},
						.formula = Multiplier(total.atk, LevelableTalent::${source}, {${Object.entries(skill.Promote).map(([_, value]) => value.Param[parseInt(index) - 1].toFixed(4)).join(", ")}}),
					},`;
        }
    }

    return ret;
}

const retHeader: string = `#pragma once

#include "character/data.hpp"

namespace Character::Datas {
	const extern Character::Data ${camelCase(data.name)};
}// namespace Character::Datas
`;

const retSource: string = `#include "${pascalCase(data.name)}.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::${camelCase(data.name)}{
	.key{${data.key}},
	.name{"${data.name}"},
	.baseStats{
		.baseHp = ${contents.BaseHP.toFixed(3)},
		.baseAtk = ${contents.BaseATK.toFixed(3)},
		.baseDef = ${contents.BaseDEF.toFixed(3)},
		.ascensionStat = Stat::${data.ascensionStat},
		.rarity = ${rarity.get(contents.Rarity)},
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::${lowerCase(contents.CharaInfo.Vision)},
		.weaponType = ${weaponType.get(contents.Weapon)},
		.hpCurve = ${growCurve.get(contents.StatsModifier.PropGrowCurves[0].growCurve)},
		.atkCurve = ${growCurve.get(contents.StatsModifier.PropGrowCurves[1].growCurve)},
		.defCurve = ${growCurve.get(contents.StatsModifier.PropGrowCurves[2].growCurve)},
		.hpUpgrade = {${strGenerator(ascensionUpgradeHp).join(", ")}},
		.atkUpgrade = {${strGenerator(ascensionUpgradeAtk).join(", ")}},
		.defUpgrade = {${strGenerator(ascensionUpgradeDef).join(", ")}},
		.ascensionStatUpgrade = {${strGenerator(ascensionUpgradeAscensionStat).join(", ")}},
	},
	.setup = []() -> Data::Setup {
		return Data::Setup{
			.mods{},
			.opts{},
			.nodes{
				.normal{${skillContentsGenerator(contents.Skills[0], "normal")}
				},
				.charged{
				},
				.plunge{
				},
				.skill{${skillContentsGenerator(contents.Skills[1], "skill")}
				},
				.burst{${skillContentsGenerator(contents.Skills[2], "burst")}
				},
			},
		};
	},
};
`;

console.log(retHeader);
console.log(retSource);

console.log(data.icon);
console.log(data.namecardIcon);

Deno.mkdirSync(`./assets/Characters/${data.name}`, { recursive: true });

const avatarResponse = await fetch(data.icon);
if (!avatarResponse.ok) {
    console.log(`Failed getting the icon with code ${avatarResponse.status}, "${avatarResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Characters/${data.name}/avatar.webp`, await avatarResponse.bytes());

const namecardResponse = await fetch(data.namecardIcon);
if (!namecardResponse.ok) {
    console.log(`Failed getting the icon with code ${namecardResponse.status}, "${namecardResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Characters/${data.name}/banner.webp`, await namecardResponse.bytes());

Deno.writeFileSync(`./src/character/characters/${pascalCase(data.name)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src/character/characters/${pascalCase(data.name)}.cpp`, new TextEncoder().encode(retSource));