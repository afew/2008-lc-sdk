// HLSL for Splatting.
//
// Copyright (c) 2007 Heesung Oh(Galic Studio). All rights reserved.
//
// Written By Heesung Oh.
// 1. Create Date: 2006-08-13
// 2. Update Date: 2008-10-13
//
////////////////////////////////////////////////////////////////////////////////


// Global
float4x4	m_mtWVP;			// World * View * Projection
float4x4	m_mtViw;			// ����*��*�������� ��ȯ ���
float4x4	m_mtPrj;			// ����*��*�������� ��ȯ ���


texture m_TxDif0 ;
texture m_TxDif1 ;
texture m_TxDif2 ;
texture m_TxDif3 ;
texture m_TxDif4 ;
texture m_TxDif5 ;

sampler SampDif0 = sampler_state{ Texture = <m_TxDif0>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP; };
sampler SampDif1 = sampler_state{ Texture = <m_TxDif1>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP; };
sampler SampDif2 = sampler_state{ Texture = <m_TxDif2>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP; };
sampler SampDif3 = sampler_state{ Texture = <m_TxDif3>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP; };
sampler SampDif4 = sampler_state{ Texture = <m_TxDif4>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP; };
sampler SampDif5 = sampler_state{ Texture = <m_TxDif5>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP; };


struct SVsOut
{
	float4 Pos	: POSITION;
	float4 Dif	: COLOR0;
	float2 Tx0	: TEXCOORD0;
	float4 Tx1	: TEXCOORD6;
	float2 Tx2	: TEXCOORD7;
};



float3	vcLgt={1,1,0};

SVsOut VtxPrc(	float4	Pos : POSITION,		// ��ġ ����
				float3	Nor : NORMAL,
				float4	Dif : COLOR0,
				float2	Tx0 : TEXCOORD0,
				float4	Tx1 : TEXCOORD1,
				float2	Tx2 : TEXCOORD2
)
{
	SVsOut Out = (SVsOut)0;								// ��µ����� �ʱ�ȭ

	float4 P = mul(Pos, m_mtViw);						// ���� ��ǥ ȸ��, ������, �̵� ��ȯ
	float4 D = Dif;

	P = mul(P, m_mtPrj);

	D.w = 1.f;

	Out.Pos = P;
	Out.Dif = D;
	Out.Tx0 = Tx0;
	Out.Tx1 = Tx1;
	Out.Tx2 = Tx2;

	return Out;
}


// �ȼ� ���̴� ���μ���(�ȼ� ���̴� 2.0 �ʿ�:compile ps_2_0)
float4 PxlPrc(SVsOut In) : COLOR0
{
	float4 Out={0,0,0,0};

	float	w = In.Tx1.x + In.Tx1.y + In.Tx1.z + In.Tx1.w +
				In.Tx2.x + In.Tx2.y;

	Out += tex2D( SampDif0 , In.Tx0 ) * In.Tx1.x;
	Out += tex2D( SampDif1 , In.Tx0 ) * In.Tx1.y;
	Out += tex2D( SampDif2 , In.Tx0 ) * In.Tx1.z;
	Out += tex2D( SampDif3 , In.Tx0 ) * In.Tx1.w;
	Out += tex2D( SampDif4 , In.Tx0 ) * In.Tx2.x;
	Out += tex2D( SampDif5 , In.Tx0 ) * In.Tx2.y;

	if(w>0.f)
		Out /=w;

	Out *=In.Dif;

	Out = min(1, Out);

	return Out;
}


// ��ũ��

technique Tech0
{
    pass P0
    {
		ALPHABLENDENABLE	= TRUE;
		SRCBLEND			= SRCALPHA;
		DESTBLEND			= INVSRCALPHA;

		ColorArg1[0]		= TEXTURE;
		ColorArg2[0]		= DIFFUSE;
		ColorOp[0]			= MODULATE;

		VertexShader = compile vs_1_1 VtxPrc();
		PixelShader  = compile ps_2_0 PxlPrc();
    }
}
