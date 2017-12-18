cbuffer globals
{
	matrix matWorldViewProj;
	float4 color;
};

struct VS_INPUT
{
    float3 pos : POSITION;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT vs_simple(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = mul(float4(In.pos,1), matWorldViewProj);
	return Out;
}

float4 ps_simple() : SV_TARGET
{
	return color;
}
