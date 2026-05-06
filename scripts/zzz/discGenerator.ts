import pascalCase from "https://deno.land/x/case@2.2.0/pascalCase.ts";
import camelCase from "https://deno.land/x/case@2.2.0/camelCase.ts";
import { ManifestNanoka } from "../manifestNanoka.d.ts";
import { Disc } from "./discTypeNanoka.d.ts";

if (Deno.args[0] == undefined) {
	console.error("Usage: discGenerator.ts <disc id>\neg: deno run .\\scripts\\zzz\\discGenerator.ts 31000");
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

const response = await fetch(`https://static.nanoka.cc/zzz/${manifestNanoka.zzz.latest}/en/equipment/${Deno.args[0]}.json`);
if (!response.ok) {
	console.error(`Response failed with code ${response.status}, "${response.statusText}"`);
}
const contents: Disc = await response.json();

const data = {
	key: contents.id,
	name: contents.name,
	caseableName: contents.name.replaceAll("'", ""),
	icon: `https://static.nanoka.cc/assets/zzz/${contents.icon.split("/").pop()?.replace(".png", ".webp")}`,
};

const retHeader: string = `#pragma once

#include "disc/setup.hpp"

namespace Disc::Sets {
	const extern Disc::Set ${camelCase(data.caseableName)};
}// namespace Disc::Sets
`;

const retSource: string = `#include "${pascalCase(data.caseableName)}.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::${camelCase(data.caseableName)}{
	.key{${data.key}},
	.goodKey{"${pascalCase(data.caseableName)}"},
	.name = "${data.name}",
	.setup = []() -> Set::Setup {
		return Set::Setup{
			.twoPc{
				.mods{
					.initial{},
				},
			},
			.fourPc{
				.opts{},
				.mods{},
			},
		};
	},
};
`;

console.log(retHeader);
console.log(retSource);

console.log(data.icon);

Deno.mkdirSync(`./assets/Discs/${data.name}`, { recursive: true });

const iconResponse = await fetch(data.icon);
if (!iconResponse.ok) {
	console.log(`Failed getting the icon with code ${iconResponse.status}, "${iconResponse.statusText}" (${data.icon})`);
}
Deno.writeFileSync(`./assets/Discs/${data.name}/icon.webp`, await iconResponse.bytes());

Deno.writeFileSync(`./src_zzz/disc/sets/${pascalCase(data.caseableName)}.hpp`, new TextEncoder().encode(retHeader));
Deno.writeFileSync(`./src_zzz/disc/sets/${pascalCase(data.caseableName)}.cpp`, new TextEncoder().encode(retSource));