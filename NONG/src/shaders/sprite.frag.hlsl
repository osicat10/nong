// Explicitly define space2 for Fragment shader textures!
Texture2D uTexture : register(t0, space2);
SamplerState uSampler : register(s0, space2);

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv  : TEXCOORD0;
};

float4 PSMain(VSOutput input) : SV_Target {
    return uTexture.Sample(uSampler, input.uv);
}