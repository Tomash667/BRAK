float4x4 matWorldViewProj;
float4 color;

float4 vs_simple(in float4 pos : POSITION) : POSITION
{
	return pos * matWorldViewProj;
}

float4 ps_simple(in float4 pos : POSITION) : COLOR
{
	return color;
}

technique simple
{
	pass pass0
	{
		VertexShader = compile vs_5_0 vs_area();
		PixelShader = compile ps_5_0 ps_area();
	}
}
