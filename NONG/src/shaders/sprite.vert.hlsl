struct VSInput {
    float3 pos : TEXCOORD0;
    float2 uv  : TEXCOORD1;
};

struct VSOutput {
    float4 pos : SV_Position;
    float2 uv  : TEXCOORD0;
};

cbuffer CameraBuffer : register(b0, space1) { float4x4 uViewProj; }
cbuffer ModelBuffer  : register(b1, space1) { float4x4 uModel; }
cbuffer UVBuffer     : register(b2, space1) { float4 uUVBounds; } 

VSOutput VSMain(VSInput input) {
    VSOutput output;
    float4 worldPos = mul(uModel, float4(input.pos, 1.0f));
    output.pos = mul(uViewProj, worldPos);
    
    float width = uUVBounds.z - uUVBounds.x;
    float height = uUVBounds.w - uUVBounds.y;
    output.uv = float2(
        uUVBounds.x + (input.uv.x * width), 
        uUVBounds.y + (input.uv.y * height)
    );
    
    return output;
}