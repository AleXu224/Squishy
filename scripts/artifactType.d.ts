export interface Artifact {
    Id: number;
    Icon: string;
    Need: number[];
    Rank: number[];
    Affix: Affix[];
    Parts: Parts;
}

export interface Affix {
    affixId: number;
    id: number;
    openConfig: string;
    addProps: AddProp[];
    paramList: number[];
    Name: string;
    Desc: string;
    level?: number;
}

export interface AddProp {
}

export interface Parts {
    EQUIP_RING: Equip;
    EQUIP_NECKLACE: Equip;
    EQUIP_DRESS: Equip;
    EQUIP_BRACER: Equip;
    EQUIP_SHOES: Equip;
}

export interface Equip {
    Icon: string;
    Name: string;
    Desc: string;
    Story: number;
}
