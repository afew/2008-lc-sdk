
static const int MAX_MATRICES = 60;

// Blending 행렬
float4x4	g_mtBldWld[MAX_MATRICES];

// 월드 행렬
float4x4	g_mtWld;

// 회전 행렬
float4x4	g_mtRot;

// View * Projection 행렬
float4x4	g_mtViwPrj;

// Lighting Vector
float3	g_vcLgt={1,1,0};

// 카메라 위치
float3	g_vcCam;


// 디퓨즈 맵
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


// 노말 맵
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


// 스페큘러 맵
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



// Emissive 맵
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


// 버텍스 Process 출력
struct SVsOut
{
	float4 Pos	: POSITION;	// 정점 위치
	float4 Dif	: COLOR0;	// 디퓨즈 칼라
	float2 Tx0	: TEXCOORD0;	// 디퓨즈 맵 좌표
	float3 Nor	: TEXCOORD1;	// 법선 벡터
	float3 Rec	: TEXCOORD3;	// 반사 벡터
	float3 Eye	: TEXCOORD4;	// 시선 벡터
};


// 버텍스 Process 입력
struct SVsIn
{
	float4 Pos : POSITION;	// 위치 벡터
	float3 Nor : NORMAL;	// 법선 벡터
	float4 Dif : COLOR0;	// 정점 디퓨즈 색상
	float2 Tx0 : TEXCOORD0;	// 텍스처 좌표
	float4 Wgt : TEXCOORD1;	// 스키닝 행렬 비중 값
	float4 Idx : TEXCOORD2;	// 스키닝 행렬 인덱스
};


SVsOut VtxBlend(SVsIn In)
{
	// 출력데이터 초기화
	SVsOut Out = (SVsOut)0;

	// 스키닝 적용을 위한 행렬 설정
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
	// 법선 벡터 갱신

	//법선벡터를 위해 회전값만 행렬에서 추출
	mtR	= (float3x3)mtW;

	// 외부에서 주어진 행렬과 곱셈
	mtR	= mul(mtR, (float3x3)g_mtRot);

	// 법선은 회전만 적용
	vcN	= mul(In.Nor, (float3x3)mtR);
	vcN = normalize(vcN);
	Out.Nor = vcN;


	////////////////////////////////////////
	// 위치 갱신

	// 위치는 스키닝 행렬 갱신후 월드 행렬을 다시 곱한다.

	mtW = mul(mtW, g_mtWld);
	vcP   = mul(In.Pos, mtW);
	// 최종 정점
	Out.Pos = mul(float4(vcP.xyz, 1.0f), g_mtViwPrj);

	// 정점 디퓨즈
	Out.Dif = In.Dif;

	// 텍스처 좌표
	Out.Tx0  = In.Tx0;


	////////////////////////////////////////
	//스페큘러 계산
	 vcL = normalize(g_vcLgt);

	// 반사 벡터
	vcR = 2 * dot(vcN, vcL) * vcN - vcL;

	// 시선 벡터 카메라 - 정점 위치
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

	// 법선은 회전만 적용
	vcN	= mul(In.Nor, (float3x3)g_mtRot);
	vcN = normalize(vcN);
	Out.Nor = vcN;

	// 위치 갱신
	vcP   = mul(In.Pos, g_mtWld);
	Out.Pos = mul(float4(vcP.xyz, 1.0f), g_mtViwPrj);

	// 정점 디퓨즈
	Out.Dif = In.Dif;

	// 텍스처 좌표
	Out.Tx0  = In.Tx0;


	////////////////////////////////////////
	//스페큘러 계산
	 vcL = normalize(g_vcLgt);

	// 반사 벡터
	vcR = 2 * dot(vcN, vcL) * vcN - vcL;

	// 시선 벡터 카메라 - 정점 위치
	vcE = g_vcCam - vcP;

	Out.Rec = vcR;
	Out.Eye = vcE;

	return Out;
}

// 픽셀 쉐이더 프로세스
float4 PxlPrc0(SVsOut In) : COLOR
{
	float4 Out;

	float4 D = 0;		// Diffuse
	float4 S = 0;		// Specular

	float3 vcL = normalize(g_vcLgt);
	float3 vcN = normalize(In.Nor);
	float3 vcR = normalize(In.Rec);
	float3 vcE = normalize(In.Eye);

	// 램버트 확산에 의한 디퓨즈 계산
	D = (1 + dot(vcN, vcL)) * 0.5f;
	D.w = 1.f;

	// 스페큘러 계산
	S = pow( max(0, dot(vcR, vcE)), 20)*0.7f;
	S.w =0;
	S *= tex2D( SampSpc, In.Tx0 );

	// 최종: 정점 색 * 램버트 * Texture 색상 * 노말 + 퐁 쉐이딩
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


