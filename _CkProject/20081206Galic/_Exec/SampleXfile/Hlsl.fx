//
// Skinned Mesh Effect file 
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
//

float4 lhtDir = {0.0f, 0.0f, -1.0f, 1.0f};    //light Direction 
float4 lightDiffuse = {0.8, 0.8f, 0.8f, 1.0f}; // Light Diffuse
float4 MaterialAmbient : MATERIALAMBIENT = {0.1f, 0.1f, 0.1f, 1.0f};
float4 MaterialDiffuse : MATERIALDIFFUSE = {0.8f, 0.8f, 0.8f, 1.0f};

// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    mViewProj : VIEWPROJECTION;


float3		m_vcCam;		// 카메라 위치

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


///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos          : POSITION;
    float4  BlendWeights : BLENDWEIGHT;
    float4  BlendIndices : BLENDINDICES;
    float3  Nor       : NORMAL;
    float2  Tx0          : TEXCOORD0;
};

struct SVsOut
{
	float4 Pos	: POSITION;
	float4 Dff	: COLOR0;		// 색상 변환
	float2 Tx0	: TEXCOORD0;	// 디퓨즈 맵 좌표
	float3 Rec	: TEXCOORD1;	// 반사 벡터
	float3 Eye	: TEXCOORD2;	// 시선 벡터
	float3 Nor	: TEXCOORD3;	// 노말 벡터
};


float3 Diffuse(float3 Nor)
{
    float CosTheta;
    
    // N.L Clamped
    CosTheta = max(0.0f, dot(Nor, lhtDir.xyz));
       
    // propogate scalar result to vector
    return (CosTheta);
}


SVsOut VShade(VS_INPUT i, uniform int NumBones)
{
    SVsOut   Out;
    float3      Pos = 0.0f;
    float3      Nor = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])i.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Nor += mul(i.Nor, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);
    Nor += (mul(i.Nor, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); 
    
    // transform position from world space into view and then projection space
    Out.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);

    // normalize normals
    Nor = normalize(Nor);

    // Shade (Ambient + etc.)
//    Out.Dff.xyz = MaterialAmbient.xyz + Diffuse(Nor) * MaterialDiffuse.xyz;
//    Out.Dff.w = 1.0f;

    // copy the input texture coordinate through
    Out.Tx0  = i.Tx0.xy;


	float3 R = 2 * dot(Nor, lhtDir) * Nor - lhtDir;	// 반사 벡터
	float3 E = m_vcCam - Pos.xyz;			// 시선 벡터 카메라 - 정점 위치

	Out.Dff = (1+dot(Nor, lhtDir)) * 0.5f;						// 램버트 확산 I = N*L

	Out.Rec = R;
	Out.Eye = E;
	Out.Nor = Nor;

    return Out;
}



// 픽셀 쉐이더 프로세스(픽셀 쉐이더 2.0 필요:compile ps_2_0)
float4 PxlPrc(SVsOut In) : COLOR
{   
	float4 Out;
	float4 tDif = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse

	float3 R = normalize(In.Rec);
	float3 E = normalize(In.Eye);
	float3 L = normalize(lhtDir);
	float3 N = normalize(In.Nor);

	float4 B = (1+dot(N, L))/2.f;										// Bump 강도

	float4 S = tex2D( SampSpc, In.Tx0 );

	B = pow(B, 1.5f)*0.9;											// Bump 강도
	B.a = 1.0f;

	S = pow(S, 16) *0.1f;											// Sharpness

	Out = In.Dff * B * tex2D( SampDif, In.Tx0 )*1.2f;		// 정점 색변환 * Bump 강도 * Texture 색상
	Out *=lightDiffuse;

	Out += pow( max(0, dot(R, E)), 20)*tDif + S;					// 퐁 쉐이딩 I = V*N ^ Power * 스페큘러 맵

	

	return Out;
}



int CurNumBones = 2;
VertexShader vsArray[4] = { compile vs_1_1 VShade(1), 
                            compile vs_1_1 VShade(2),
                            compile vs_1_1 VShade(3),
                            compile vs_1_1 VShade(4)
                          };


//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
		PixelShader  = compile ps_2_0 PxlPrc();
    }
}

