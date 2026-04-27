Texture2D uTexture : register(t0, space2);
SamplerState uSampler : register(s0, space2);
cbuffer ColorBuffer : register(b0, space3) { float4 uColor; }

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
    
    float2 centerUV = input.uv - 0.5f;
    float distance = length(centerUV);
    
    if (distance > 0.5f) {
        discard; 
    }
    
    return texColor * uColor;
}