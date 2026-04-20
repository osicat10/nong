// Slot 0: The Camera Matrix
cbuffer CameraData : register(b0, space1) {
    float4x4 uViewProjection;
};

// Slot 1: The Object's Transform Matrix (THIS WAS MISSING!)
cbuffer ObjectData : register(b1, space1) {
    float4x4 uModel;
};

struct VSInput {
    float3 pos : TEXCOORD0;
    float2 uv  : TEXCOORD1;
};

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv  : TEXCOORD0;
};

VSOutput VSMain(VSInput input) {
    VSOutput output;
    
    // Multiply MATRIX by VECTOR
    float4 worldPos = mul(uModel, float4(input.pos, 1.0));
    output.pos = mul(uViewProjection, worldPos);
    
    output.uv = input.uv;
    return output;
}