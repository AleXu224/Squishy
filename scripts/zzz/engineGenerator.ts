import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { Engine } from "./engineTypeNanoka.d.ts";
import { camelCase } from "https://deno.land/x/case@2.2.0/mod.ts";
import { ManifestNanoka } from "../manifestNanoka.d.ts";
import { statMap } from "./mapped.ts";

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
};

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
	.setup = []() {
		return Data::Setup{};
	},
};
`;

console.log(retHeader);
console.log(retSource);

console.log(data.icon);

Deno.mkdirSync(`./assets/Engines/${data.name}`, { recursive: true });

const iconResponse = await fetch(data.icon);
if (!iconResponse.ok) {
	console.log(`Failed getting the icon with code ${iconResponse.status}, "${iconResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Engines/${data.name}/icon.webp`, await iconResponse.bytes());

Deno.writeFileSync(`./src_zzz/engine/engines/${pascalCase(data.caseableName)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src_zzz/engine/engines/${pascalCase(data.caseableName)}.cpp`, new TextEncoder().encode(retSource));