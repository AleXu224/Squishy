import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { Engine } from "./engineTypeNanoka.d.ts";
import { camelCase } from "https://deno.land/x/case@2.2.0/mod.ts";
import { ManifestNanoka } from "../manifestNanoka.d.ts";
import { statMap } from "./mapped.ts";

interface TalentMultiplier {
	values: number[];
	isPercentage: boolean;
}

function formatCppFloat(value: number, isPercentage: boolean): string {
	const floatValue = isPercentage ? value / 100 : value;
	return Number.isInteger(floatValue) ? `${floatValue}.f` : `${floatValue}f`;
}

function extractVaryingTalentNumbers(talents: Engine["talents"]): TalentMultiplier[] {
	const orderedDescriptions = Object.entries(talents)
		.sort(([left], [right]) => parseInt(left) - parseInt(right))
		.map(([, talent]) => talent.desc.replaceAll(/<color=[^>]+>|<\/color>/g, ""));

	if (orderedDescriptions.length === 0) return [];

	const extractedValues = orderedDescriptions.map(desc => {
		const matches = [...desc.matchAll(/(\d+(?:\.\d+)?)(%)?/g)];
		return matches.map(match => ({
			value: parseFloat(match[1]),
			isPercentage: match[2] === "%",
		}));
	});

	const firstEntry = extractedValues[0];
	for (const values of extractedValues) {
		if (values.length !== firstEntry.length) {
			throw new Error("Talent descriptions do not have matching numeric token counts across overclocks");
		}
	}

	const varyingValues: TalentMultiplier[] = [];
	for (let index = 0; index < firstEntry.length; index++) {
		const valuesAtIndex = extractedValues.map(values => values[index].value);
		const isPercentage = firstEntry[index].isPercentage;
		if (!extractedValues.every(values => values[index].isPercentage === isPercentage)) {
			throw new Error("Talent descriptions do not have matching percentage tokens across overclocks");
		}

		if (!valuesAtIndex.every(value => value === valuesAtIndex[0])) {
			varyingValues.push({
				values: valuesAtIndex,
				isPercentage,
			});
		}
	}

	return varyingValues;
}

if (Deno.args[0] == undefined) {
	console.error("Usage: engineGenerator.ts <engine id>\neg: deno run .\\scripts\\zzz\\engineGenerator.ts 14140");
	Deno.exit(1);
}

try {
	const cmakeFile = Deno.openSync("./CMakeLists.txt");
	cmakeFile.close();
} catch (_error) {
	console.error("You must run this script from the project root directory (where main.cpp and CMakeLists.txt reside)");
	Deno.exit(1);
}

const manifestNanokaResponse = await fetch("https://static.nanoka.cc/manifest.json");
if (!manifestNanokaResponse.ok) {
	console.error(`Failed to get the manifest with code ${manifestNanokaResponse.status}, "${manifestNanokaResponse.statusText}"`);
	Deno.exit(1);
}
const manifestNanoka: ManifestNanoka = await manifestNanokaResponse.json();

const response = await fetch(`https://static.nanoka.cc/zzz/${manifestNanoka.zzz.latest}/en/weapon/${Deno.args[0]}.json`);
if (!response.ok) {
	console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: Engine = await response.json();

const data = {
	key: Deno.args[0],
	name: contents.name,
	caseableName: contents.name.replaceAll("'", ""),
	type: Object.values(contents.weapon_type).pop()?.toLowerCase(),
	subStat: statMap.get(contents.rand_property.name),
	icon: `https://static.nanoka.cc/assets/zzz/${contents.icon.split("/").pop()?.replace(".png", ".webp")}`,
	talentNumbers: extractVaryingTalentNumbers(contents.talents),
};

const talentMultiplierStr = data.talentNumbers
	.map((entry, index) => `\n\t\tauto multiplier${index + 1} = EngineMultiplier(${entry.isPercentage}, {${entry.values.map(value => formatCppFloat(value, entry.isPercentage)).join(", ")}});`)
	.join("");

const retHeader: string = `#pragma once

#include "engine/data.hpp"

namespace Engine::Datas {
	const extern Engine::Data ${camelCase(data.caseableName)};
}// namespace Engine::Datas
`;

const retSource: string = `#include "${pascalCase(data.caseableName)}.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::${camelCase(data.caseableName)}{
	.key{${data.key}},
	.goodKey{"${pascalCase(data.caseableName)}"},
	.name = "${data.name}",
	.baseStats{
		.specialty = Misc::Specialty::${data.type},
		.rarity = ${contents.rarity + 1},
		.baseAtk = ${contents.base_property.value},
		.subStat{
			.stat = Stat::${data.subStat},
			.value = ${(contents.rand_property.value / 10000).toFixed(3)},
		},
	},
	.setup = []() {${talentMultiplierStr}
		return Data::Setup{};
	},
};
`;

console.log(retHeader);
console.log(retSource);

console.log(data.talentNumbers);

console.log(data.icon);

Deno.mkdirSync(`./assets/Engines/${data.name}`, { recursive: true });

const iconResponse = await fetch(data.icon);
if (!iconResponse.ok) {
	console.log(`Failed getting the icon with code ${iconResponse.status}, "${iconResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Engines/${data.name}/icon.webp`, await iconResponse.bytes());

Deno.writeFileSync(`./src_zzz/engine/engines/${pascalCase(data.caseableName)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src_zzz/engine/engines/${pascalCase(data.caseableName)}.cpp`, new TextEncoder().encode(retSource));