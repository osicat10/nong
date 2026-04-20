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
    // Set the final screen position
    output.pos = float4(input.pos, 1.0);
    // Pass the UV coordinates straight through
    output.uv = input.uv;
    return output;
}