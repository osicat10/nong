Texture2D uTexture : register(t0, space2);
SamplerState uSampler : register(s0, space2);

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv  : TEXCOORD0;
};

float4 PSMain(VSOutput input) : SV_Target 
{
    float4 texColor = uTexture.Sample(uSampler, input.uv);
    

    if (texColor.a < 0.1f) {
        discard; 
    }
    
    return texColor;
}