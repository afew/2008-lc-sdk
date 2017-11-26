
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

	if(mtW._44 <0.0001f)
	{
		mtW._11= 1.0f;
		mtW._12= 0.0f;
		mtW._13= 0.0f;
		mtW._14= 0.0f;

		mtW._21= 0.0f;
		mtW._22= 1.0f;
		mtW._23= 0.0f;
		mtW._24= 0.0f;

		mtW._31= 0.0f;
		mtW._32= 0.0f;
		mtW._33= 1.0f;
		mtW._34= 0.0f;

		mtW._41= 0.0f;
		mtW._42= 0.0f;
		mtW._43= 0.0f;
		mtW._44= 1.0f;

	}



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
float4 PxlPrc(SVsOut In) : COLOR
{
	float4 Out;

	float4 D = 0;		// Diffuse
	float4 S = 0;		// Specular
	float4 B = 0;		// Bump
	float4 E = 0;		// Emissive

	float3 vcL = normalize(g_vcLgt);

	float3 vcN = normalize(In.Nor);
	float3 vcR = normalize(In.Rec);
	float3 vcE = normalize(In.Eye);


	// ����Ʈ Ȯ�꿡 ���� ��ǻ�� ���
	D = (1 + dot(vcN, vcL)) * 0.5f;


	// ���� ���
	float3 Tn = 2*tex2D( SampNor, In.Tx0 ).xyz-1;	// ���������κ��� ����
	float3 Ty = vcN;								// y������ ����
	float3 Tz = {0,0,1};							// ������ �������Ϳ� 0,0,1�� ������ ������ ���� ����.
	float3 Tx = 0;

	Tx = normalize(cross(Ty, Tz));
	Tz = normalize(cross(Ty, Tx));

	B.x = dot(Tx, Tn);
	B.y = dot(Ty, Tn);
	B.z = dot(Tz, Tn);

	B = (1 + dot(B, vcL)) * 0.7f;
	B = pow(B, 2);
	B = 1;


	// ����ŧ�� ���
	S = pow( max(0, dot(vcR, vcE)), 20)*0.7f;
//	S *= tex2D( SampSpc, In.Tx0 );


	// ����: ���� �� * ����Ʈ * Texture ���� * �븻 + �� ���̵�
	Out = In.Dif * D * tex2D( SampDif, In.Tx0 ) * B;
	Out += S;
	Out.w =1.f;
	return Out;
}

technique TShader
{
	// For Vertex Blending
	pass P0
	{
		CullMode	= NONE;
		VertexShader = compile vs_2_0 VtxBlend();
		PixelShader  = compile ps_2_0 PxlPrc();
	}

	// For Non Blending
	pass P1
	{
		AlphatestEnable	= FALSE;
		AlphablendEnable	= FALSE;
		CullMode	= NONE;
		VertexShader = compile vs_2_0 VtxRigid();
		PixelShader  = compile ps_2_0 PxlPrc();
	}
}
