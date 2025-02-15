import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import { Artifact } from "./artifactType.d.ts";
import camelCase from "https://deno.land/x/case@2.2.0/camelCase.ts";

if (Deno.args[0] == undefined) {
	console.error("Usage: artifactGenerator.ts <artifact set id>\neg: deno run .\\scripts\\artifactGenerator.ts 15026");
	Deno.exit(1);
}

try {
	const cmakeFile = Deno.openSync("./CMakeLists.txt");
	cmakeFile.close();
} catch (_error) {
	console.error("You must run this script from the project root directory (where main.cpp and CMakeLists.txt reside)");
	Deno.exit(1);
}

const response = await fetch(`https://api.hakush.in/gi/data/en/artifact/${Deno.args[0]}.json`);
if (!response.ok) {
	console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: Artifact = await response.json();

const data = {
	key: contents.Id,
	name: contents.Affix[0].Name,
	iconGoblet: `https://api.hakush.in/gi/UI/UI_RelicIcon_${contents.Id}_1.webp`,
	iconPlume: `https://api.hakush.in/gi/UI/UI_RelicIcon_${contents.Id}_2.webp`,
	iconCirclet: `https://api.hakush.in/gi/UI/UI_RelicIcon_${contents.Id}_3.webp`,
	iconFlower: `https://api.hakush.in/gi/UI/UI_RelicIcon_${contents.Id}_4.webp`,
	iconSands: `https://api.hakush.in/gi/UI/UI_RelicIcon_${contents.Id}_5.webp`,
};

const ret: string = `#pragma once

#include "artifact/setup.hpp"

namespace Artifact::Sets {
	const inline Artifact::Set ${camelCase(data.name)}{
		.key{${data.key}},
		.name = "${data.name}",
		.setup = []() -> Set::Setup {
			return Set::Setup{
				.twoPc{
					.mods{
						.preMod{},
					},
				},
				.fourPc{
					.opts{},
					.mods{},
				},
			};
		},
	};
}// namespace Artifact::Sets
`;

console.log(ret);

console.log(data.iconGoblet);
console.log(data.iconPlume);
console.log(data.iconCirclet);
console.log(data.iconFlower);
console.log(data.iconSands);

Deno.mkdirSync(`./assets/Artifacts/${data.name}`, { recursive: true });

const iconResponseGoblet = await fetch(data.iconGoblet);
if (!iconResponseGoblet.ok) {
	console.log(`Failed getting the icon with code ${iconResponseGoblet.status}, "${iconResponseGoblet.statusText}" (${data.iconGoblet})`);
}
Deno.writeFileSync(`./assets/Artifacts/${data.name}/Goblet.webp`, await iconResponseGoblet.bytes());

const iconResponsePlume = await fetch(data.iconPlume);
if (!iconResponsePlume.ok) {
	console.log(`Failed getting the icon with code ${iconResponsePlume.status}, "${iconResponsePlume.statusText}" (${data.iconPlume})`);
}
Deno.writeFileSync(`./assets/Artifacts/${data.name}/Plume.webp`, await iconResponsePlume.bytes());

const iconResponseCirclet = await fetch(data.iconCirclet);
if (!iconResponseCirclet.ok) {
	console.log(`Failed getting the icon with code ${iconResponseCirclet.status}, "${iconResponseCirclet.statusText}" (${data.iconCirclet})`);
}
Deno.writeFileSync(`./assets/Artifacts/${data.name}/Circlet.webp`, await iconResponseCirclet.bytes());

const iconResponseFlower = await fetch(data.iconFlower);
if (!iconResponseFlower.ok) {
	console.log(`Failed getting the icon with code ${iconResponseFlower.status}, "${iconResponseFlower.statusText}" (${data.iconFlower})`);
}
Deno.writeFileSync(`./assets/Artifacts/${data.name}/Flower.webp`, await iconResponseFlower.bytes());

const iconResponseSands = await fetch(data.iconSands);
if (!iconResponseSands.ok) {
	console.log(`Failed getting the icon with code ${iconResponseSands.status}, "${iconResponseSands.statusText}" (${data.iconSands})`);
}
Deno.writeFileSync(`./assets/Artifacts/${data.name}/Sands.webp`, await iconResponseSands.bytes());

Deno.writeFileSync(`./src/artifact/sets/${pascalCase(data.name)}.hpp`, new TextEncoder().encode(ret));