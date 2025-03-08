export interface ArtifactMainstatData {
    level: number;
    addProps: AddProp[];
    rank?: number;
    exp?: number;
}

export interface AddProp {
    propType: string;
    value: number;
}
