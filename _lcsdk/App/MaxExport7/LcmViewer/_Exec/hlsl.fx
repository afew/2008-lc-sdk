
static const int MAX_MATRICES = 60;

// Blending ���
float4x4	g_mtBldWld[MAX_MATRICES];

// ���� ���
float4x4	g_mtWld;

// ȸ�� ���
float4x4	g_mtRot;

// View * Projection ���
float4x4	g_mtViwPrj;

// Lighting Vector
float3	g_vcLgt={1,1,0};

// ī�޶� ��ġ
float3	g_vcCam;


// ��ǻ�� ��
texture m_TxDif;
sampler SampDif = sampler_state
{
	Texture = <m_TxDif>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


// �븻 ��
texture m_TxNor;
sampler SampNor = sampler_state
{
	Texture = <m_TxNor>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


// ����ŧ�� ��
texture m_TxSpc;
sampler SampSpc = sampler_state
{
	Texture = <m_TxSpc>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};



// Emissive ��
texture m_TxEms;
sampler SampEms = sampler_state
{
	Texture = <m_TxEms>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


// ���ؽ� Process ���
struct SVsOut
{
	float4 Pos	: POSITION;	// ���� ��ġ
	float4 Dif	: COLOR0;	// ��ǻ�� Į��
	float2 Tx0	: TEXCOORD0;	// ��ǻ�� �� ��ǥ
	float3 Nor	: TEXCOORD1;	// ���� ����
	float3 Rec	: TEXCOORD3;	// �ݻ� ����
	float3 Eye	: TEXCOORD4;	// �ü� ����
};


// ���ؽ� Process �Է�
struct SVsIn
{
	float4 Pos : POSITION;	// ��ġ ����
	float3 Nor : NORMAL;	// ���� ����
	float4 Dif : COLOR0;	// ���� ��ǻ�� ����
	float2 Tx0 : TEXCOORD0;	// �ؽ�ó ��ǥ
	float4 Wgt : TEXCOORD1;	// ��Ű�� ��� ���� ��
	float4 Idx : TEXCOORD2;	// ��Ű�� ��� �ε���
};


SVsOut VtxBlend(SVsIn In)
{
	// ��µ����� �ʱ�ȭ
	SVsOut Out = (SVsOut)0;

	// ��Ű�� ������ ���� ��� ����
	float4  vcP = 0.0f;
	float4x4 mtW = 0.0f;
	float3x3 mtR= 0.0f;
	float3 vcN= 0.0f;
	float3 vcR = 0.0f;
	float3 vcE = 0.0f;
	float3 vcL = 0.0f;


	mtW += In.Wgt.x * g_mtBldWld[ In.Idx.x ];
	mtW += In.Wgt.y * g_mtBldWld[ In.Idx.y ];
	mtW += In.Wgt.z * g_mtBldWld[ In.Idx.z ];
	mtW += In.Wgt.w * g_mtBldWld[ In.Idx.w ];


	////////////////////////////////////////
	// ���� ���� ����

	//�������͸� ���� ȸ������ ��Ŀ��� ����
	mtR	= (float3x3)mtW;

	// �ܺο��� �־��� ��İ� ����
	mtR	= mul(mtR, (float3x3)g_mtRot);

	// ������ ȸ���� ����
	vcN	= mul(In.Nor, (float3x3)mtR);
	vcN = normalize(vcN);
	Out.Nor = vcN;


	////////////////////////////////////////
	// ��ġ ����

	// ��ġ�� ��Ű�� ��� ������ ���� ����� �ٽ� ���Ѵ�.

	mtW = mul(mtW, g_mtWld);
	vcP   = mul(In.Pos, mtW);
	// ���� ����
	Out.Pos = mul(float4(vcP.xyz, 1.0f), g_mtViwPrj);

	// ���� ��ǻ��
	Out.Dif = In.Dif;

	// �ؽ�ó ��ǥ
	Out.Tx0  = In.Tx0;


	////////////////////////////////////////
	//����ŧ�� ���
	 vcL = normalize(g_vcLgt);

	// �ݻ� ����
	vcR = 2 * dot(vcN, vcL) * vcN - vcL;

	// �ü� ���� ī�޶� - ���� ��ġ
	vcE = g_vcCam - vcP;

	Out.Rec = vcR;
	Out.Eye = vcE;

	return Out;
}



SVsOut VtxRigid(SVsIn In)
{
	SVsOut Out = (SVsOut)0;

	float4 vcP = 0.0f;
	float3 vcN = 0.0f;
	float3 vcR = 0.0f;
	float3 vcE = 0.0f;
	float3 vcL = 0.0f;

	// ������ ȸ���� ����
	vcN	= mul(In.Nor, (float3x3)g_mtRot);
	vcN = normalize(vcN);
	Out.Nor = vcN;

	// ��ġ ����
	vcP   = mul(In.Pos, g_mtWld);
	Out.Pos = mul(float4(vcP.xyz, 1.0f), g_mtViwPrj);

	// ���� ��ǻ��
	Out.Dif = In.Dif;

	// �ؽ�ó ��ǥ
	Out.Tx0  = In.Tx0;


	////////////////////////////////////////
	//����ŧ�� ���
	 vcL = normalize(g_vcLgt);

	// �ݻ� ����
	vcR = 2 * dot(vcN, vcL) * vcN - vcL;

	// �ü� ���� ī�޶� - ���� ��ġ
	vcE = g_vcCam - vcP;

	Out.Rec = vcR;
	Out.Eye = vcE;

	return Out;
}

// �ȼ� ���̴� ���μ���
float4 PxlPrc0(SVsOut In) : COLOR
{
	float4 Out;

	float4 D = 0;		// Diffuse
	float4 S = 0;		// Specular

	float3 vcL = normalize(g_vcLgt);
	float3 vcN = normalize(In.Nor);
	float3 vcR = normalize(In.Rec);
	float3 vcE = normalize(In.Eye);

	// ����Ʈ Ȯ�꿡 ���� ��ǻ�� ���
	D = (1 + dot(vcN, vcL)) * 0.5f;
	D.w = 1.f;

	// ����ŧ�� ���
	S = pow( max(0, dot(vcR, vcE)), 20)*0.7f;
	S.w =0;
	S *= tex2D( SampSpc, In.Tx0 );

	// ����: ���� �� * ����Ʈ * Texture ���� * �븻 + �� ���̵�
	Out = In.Dif * D * tex2D( SampDif, In.Tx0 );

	Out += S;

	return Out;
}


float4 PxlPrc1(SVsOut In) : COLOR
{
	float4 Out;
	float4 D;
	D = tex2D( SampDif, In.Tx0 );

	D = pow( max(0, D), 2.f)*3.f;
	Out	= D;
	return Out;
}



technique TShader
{
	// For Vertex Blending
	pass P0
	{
		AlphatestEnable	= True;
		AlphaFunc		= Greater;
		AlphaREF		= 0x10;

		CullMode	= NONE;
		AlphablendEnable	= FALSE;
		VertexShader = compile vs_2_0 VtxBlend();
		PixelShader  = compile ps_2_0 PxlPrc0();
	}

	// For Non Blending
	pass P1
	{
		AlphatestEnable	= False;
		CullMode		= NONE;

		AlphaFunc		= Greater;
		AlphaREF		= 0x20;

		AlphablendEnable= TRUE;
		SRCBLEND		= SrcAlpha;
		DESTBLEND		= InvSrcAlpha;
		VertexShader = compile vs_2_0 VtxRigid();
		PixelShader  = compile ps_2_0 PxlPrc0();
	}


	// For Non Blending
	pass P2
	{
		AlphatestEnable	= False;
		CullMode		= NONE;
		AlphablendEnable= TRUE;
		SRCBLEND		= SrcAlpha;
		DESTBLEND		= DestAlpha;
		VertexShader = compile vs_2_0 VtxRigid();
		PixelShader  = compile ps_2_0 PxlPrc1();
	}
}


