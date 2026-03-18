import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { LunarisCharacter, Skill } from "./characterTypeLunaris.d.ts";
import { camelCase } from "https://deno.land/x/case@2.2.0/mod.ts";
import { growCurve, propToStat, rarity, weaponType } from "./mapped.ts";
import lowerCase from "https://deno.land/x/case@2.2.0/lowerCase.ts";
import { Version } from "./versionType.d.ts";
// import { YattaCharacter } from "./characterTypeYatta.d.ts";
import { NanokaCharacter } from "./characterTypeNanoka.d.ts";
import { ManifestNanoka } from "./manifestNanoka.d.ts";

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

const versionResponse = await fetch("https://api.lunaris.moe/data/version.json");
if (!versionResponse.ok) {
	console.error(`Failed to get the version with code ${versionResponse.status}, "${versionResponse.statusText}"`);
	Deno.exit(1);
}
const version: Version = await versionResponse.json();

const responseLunaris = await fetch(`https://api.lunaris.moe/data/${version.version}/en/char/${Deno.args[0]}.json`);
if (!responseLunaris.ok) {
	console.error(`Response failed with code ${responseLunaris.status}, "${responseLunaris.statusText}"`);
}
const contentsLunaris: LunarisCharacter = await responseLunaris.json();

const manifestNanokaResponse = await fetch("https://static.nanoka.cc/manifest.json");
if (!manifestNanokaResponse.ok) {
	console.error(`Failed to get the manifest with code ${manifestNanokaResponse.status}, "${manifestNanokaResponse.statusText}"`);
	Deno.exit(1);
}
const manifestNanoka: ManifestNanoka = await manifestNanokaResponse.json();

const responseNanoka = await fetch(`https://static.nanoka.cc/gi/${manifestNanoka.gi.latest}/en/character/${Deno.args[0]}.json`);
if (!responseNanoka.ok) {
	console.error(`Response failed with code ${responseNanoka.status}, "${responseNanoka.statusText}"`);
}
const contentsNanoka: NanokaCharacter = await responseNanoka.json();

const data = {
	key: Deno.args[0],
	name: contentsNanoka.name,
	caseableName: contentsNanoka.name.replaceAll("'", ""),
	ascensionStat: propToStat.get(Object.keys(contentsNanoka.stats_modifier.ascension[0])[3].toUpperCase()),
	icon: `https://static.nanoka.cc/assets/gi/${contentsNanoka.icon}.webp`,
	namecardIcon: `https://static.nanoka.cc/assets/gi/${contentsNanoka.chara_info.namecard.icon}.webp`,
};

const ascensionUpgradeHp = new Float32Array(7);
const ascensionUpgradeDef = new Float32Array(7);
const ascensionUpgradeAtk = new Float32Array(7);
const ascensionUpgradeAscensionStat = new Float32Array(7);
ascensionUpgradeHp[0] = 0;
ascensionUpgradeDef[0] = 0;
ascensionUpgradeAtk[0] = 0;
ascensionUpgradeAscensionStat[0] = 0;
for (const [ascension, value] of Object.entries(contentsNanoka.stats_modifier.ascension)) {
	const ascensionIndex = parseInt(ascension) + 1;
	ascensionUpgradeHp[ascensionIndex] = Object.values(value)[0];
	ascensionUpgradeDef[ascensionIndex] = Object.values(value)[1];
	ascensionUpgradeAtk[ascensionIndex] = Object.values(value)[2];
	ascensionUpgradeAscensionStat[ascensionIndex] = Object.values(value)[3];
}

function strGenerator(entries: Float32Array, precision: number = 3): string[] {
	const ret: string[] = [];
	for (const val of entries) {
		ret.push(val.toFixed(precision));
	}
	return ret;
}

enum MultiplierElement {
	Atk = "atk",
	Def = "def",
	Hp = "hp",
	Em = "em",
}

enum MultiplierType {
	Duration = "seconds",
	Energy = "energy",
	Percent = "percent",
	Points = "points",
}

interface MultiplierEntry {
	element?: MultiplierElement;
	type: MultiplierType;
	values: number[];
}

function skillContentsGenerator(skill: Skill, source: string): string {
	let ret: string = "";
	if (skill.multipliers === undefined) return ret;
	for (const [name, values] of Object.entries(skill.multipliers)) {
		const firstValue = values[0];
		const alternatives = firstValue.split(/[\|\/]/).length;
		for (let i = 0; i < alternatives; i++) {
			const firstIndex = firstValue.split(/[\|\/]/)[i];
			let type: MultiplierType = MultiplierType.Points;
			if (/^\d+\.*\d+s$/.test(firstIndex))
				type = MultiplierType.Duration;
			else if (name.toLocaleLowerCase().includes("energy"))
				type = MultiplierType.Energy;
			else if (firstIndex.includes("%"))
				type = MultiplierType.Percent;

			const firstParts = firstIndex.split("+");
			const entries: MultiplierEntry[] = [];

			for (const part of firstParts) {
				let scalingElement: MultiplierElement | undefined = undefined;
				let typeToUse = type;
				if (typeToUse == MultiplierType.Percent) {
					if (!part.includes("%")) {
						typeToUse = MultiplierType.Points;
					} else if (part.toLocaleLowerCase().includes("% max hp")) {
						scalingElement = MultiplierElement.Hp;
					} else if (/%(?:×\d+)? DEF/.test(part)) {
						scalingElement = MultiplierElement.Def;
					} else if (part.toLocaleLowerCase().includes("% elemental mastery")) {
						scalingElement = MultiplierElement.Em;
					} else {
						scalingElement = MultiplierElement.Atk;
					}
				}

				entries.push({
					element: scalingElement,
					type: typeToUse,
					values: [],
				});
			}

			for (const value of values) {
				const parts = value.split(/[\|\/]/)[i].split("+");
				for (let j = 0; j < parts.length; j++) {
					const part = parts[j];
					const numberMatch = /\d+\.*\d*/.exec(part);
					if (numberMatch == null) continue;
					let number = parseFloat(numberMatch[0]);
					if (entries[j].type == MultiplierType.Percent) {
						number = number / 100;
					}
					entries[j].values.push(number);
				}
			}

			const formulaRet: string[] = [];
			for (let i = 0; i < firstParts.length; i++) {
				const entry = entries[i];
				let typeStr = "";
				if (entry.type != MultiplierType.Percent) {
					typeStr = `Utils::EntryType::${entry.type}, `;
				}

				let scalingStr = "";
				if (entry.element !== undefined) {
					scalingStr = `total.${entry.element}, `;
				}

				formulaRet.push(`Multiplier(${typeStr}${scalingStr}LevelableTalent::${source}, {${entry.values.map(v => v.toFixed(4)).join(", ")}})`);
			}

			const formulaStr = formulaRet.join("\n\t\t\t\t\t\t\t\t + ") + ",";

			let nodeType = "Atk";
			let typeStr = "";
			if (type != MultiplierType.Percent) {
				nodeType = "Info";
				typeStr = `\n						.type = Utils::EntryType::${type},`;
			}

			if (name.toLocaleLowerCase().includes("heal") && type != MultiplierType.Duration && type != MultiplierType.Energy) {
				nodeType = "Heal";
				typeStr = "";
			}

			let sourceStr = "";
			if (nodeType == "Atk") {
				sourceStr = `\n						.source = Misc::AttackSource::${source},`;
			}

			ret += `
					Node::${nodeType}{
						.name = "${name}",${typeStr}${sourceStr}
						.formula = ${formulaStr}
					},`;
		}
	}

	return ret;
}

const retHeader: string = `#pragma once

#include "character/data.hpp"

namespace Character::Datas {
	const extern Character::Data ${camelCase(data.caseableName)};
}// namespace Character::Datas
`;

const retSource: string = `#include "${pascalCase(data.caseableName)}.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::${camelCase(data.caseableName)}{
	.key{${data.key}},
	.goodKey{"${pascalCase(data.caseableName)}"},
	.name{"${data.name}"},
	.baseStats{
		.baseHp = ${contentsNanoka.base_hp.toFixed(3)},
		.baseAtk = ${contentsNanoka.base_atk.toFixed(3)},
		.baseDef = ${contentsNanoka.base_def.toFixed(3)},
		.ascensionStat = Stat::${data.ascensionStat},
		.rarity = ${rarity.get(contentsNanoka.rarity)},
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::${lowerCase(contentsNanoka.element)},
		.weaponType = ${weaponType.get(contentsNanoka.weapon)},
		.hpCurve = ${growCurve.get(contentsNanoka.stats_modifier.prop_grow_curves[0].grow_curve)},
		.atkCurve = ${growCurve.get(contentsNanoka.stats_modifier.prop_grow_curves[1].grow_curve)},
		.defCurve = ${growCurve.get(contentsNanoka.stats_modifier.prop_grow_curves[2].grow_curve)},
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
				.normal{${skillContentsGenerator(contentsLunaris.skills.normalattack, "normal")}
				},
				.charged{
				},
				.plunge{
				},
				.skill{${skillContentsGenerator(contentsLunaris.skills.elementalskill, "skill")}
				},
				.burst{${skillContentsGenerator(contentsLunaris.skills.elementalburst, "burst")}
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
	console.log(`Failed getting the icon with code ${namecardResponse.status}, "${namecardResponse.statusText}" (${data.namecardIcon})`);
}
Deno.writeFileSync(`./assets/Characters/${data.name}/banner.webp`, await namecardResponse.bytes());

Deno.writeFileSync(`./src/character/characters/${pascalCase(data.caseableName)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src/character/characters/${pascalCase(data.caseableName)}.cpp`, new TextEncoder().encode(retSource));