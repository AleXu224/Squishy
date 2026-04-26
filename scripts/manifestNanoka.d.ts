export interface ManifestNanoka {
    gi: Gi;
    zzz: Zzz;
}

export interface Gi {
    latest: string;
    available: string[];
    live: string;
    new: { [key: string]: number[] };
}

export interface Zzz {
    latest: string;
    available: string[];
    live: string;
    new: ZzzNew;
}

export interface ZzzNew {
    character: number[];
    bangboo: number[];
    weapon: number[];
    equipment: any[];
    monster: number[];
    item: number[];
}
