Texture2D tex2D;

SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer ConstantBuffer : register( b0)
{
matrix World;
matrix View;
matrix Projection;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR, float2 inpTexCoord : TEXCOORD)
{
	VS_OUTPUT outpt = (VS_OUTPUT)0;
	
	outpt.Pos = mul( Pos, World);
	outpt.Pos = mul( outpt.Pos, View );
	outpt.Pos = mul( outpt.Pos, Projection );
	outpt.Color = Color;

	
	outpt.TexCoord = inpTexCoord;

	return outpt;
}

float4 PS(VS_OUTPUT vsout) : SV_TARGET
{
	float depthz;
	float4 kolor;
	float4 texcol;
	depthz = vsout.Pos.z / vsout.Pos.w;

 	kolor = float4(depthz,depthz,depthz,1.0f);
	
	
//	vsout.Color = (kolor+vsout.Color)/2;
        float4 texkolor = tex2D.Sample( linearSampler, vsout.TexCoord );
	//texkolor = texkolor*kolor+0.10f*-1.0f;
	return texkolor;
	//return vsout.Color ;
}
