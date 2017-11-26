#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "LcSplt.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


INT m_IdxLA[][33] =	 															// 텍스처 인덱스
{
	{0,0,0,1,1,1,1,1,	1,1,3,3,3,3,0,0,	0,2,2,2,2,1,1,1,	3,0,3,3,1,1,1,1,1,	},
	{0,0,0,1,1,1,1,1,	1,1,3,3,3,3,0,0,	0,2,2,2,2,1,1,1,	3,0,3,3,1,1,1,1,1,	},
	{0,0,0,1,1,1,1,1,	1,1,3,3,3,3,0,0,	0,2,2,2,2,1,1,1,	3,0,3,3,1,1,1,1,1,	},
	{0,0,0,1,1,1,1,1,	1,1,3,3,3,3,0,0,	0,2,2,2,2,1,1,1,	3,0,3,3,1,1,1,1,1,	},

	{0,0,0,1,1,1,1,1,	3,3,3,3,3,3,3,3,	0,1,1,2,0,0,2,1,	2,3,3,2,0,1,1,1,1,	},
	{0,0,0,1,1,1,1,1,	3,3,3,3,3,3,3,3,	0,1,1,2,2,3,0,1,	2,1,1,2,0,1,1,1,1,	},
	{0,0,0,1,1,1,1,1,	3,3,3,2,3,3,3,1,	0,0,1,3,3,1,1,1,	1,1,1,2,0,1,1,1,1,	},
	{0,0,0,1,1,1,3,3,	3,3,3,3,3,2,2,0,	0,1,1,1,3,1,1,1,	2,2,2,1,0,1,1,1,1,	},

	{0,0,0,1,1,1,3,3,	3,3,3,3,2,2,2,0,	0,1,1,1,3,1,1,1,	1,1,1,1,1,1,1,1,1,	},
	{0,0,0,3,4,4,4,4,	3,3,2,2,2,2,2,0,	0,2,2,2,3,1,2,2,	2,2,2,2,1,2,2,2,2,	},
	{0,0,0,3,3,3,4,4,	3,3,2,2,2,2,0,0,	0,1,2,0,2,2,2,1,	1,1,2,2,2,1,1,1,1,	},
	{0,0,0,3,3,3,4,4,	3,3,2,2,2,2,2,0,	0,0,2,2,0,2,2,0,	2,0,2,2,1,1,1,1,1,	},

	{0,0,0,3,3,3,3,4,	4,2,2,2,2,2,2,1,	0,0,1,2,0,0,2,1,	2,1,1,2,3,1,1,1,1,	},
	{0,0,0,3,3,3,3,4,	4,2,2,2,2,2,2,0,	0,0,1,2,0,0,0,1,	2,1,3,3,3,1,1,1,1,	},
	{0,0,0,1,1,1,2,4,	4,2,2,2,2,2,1,0,	0,0,1,2,0,1,3,3,	1,0,1,2,1,1,1,1,1,	},
	{0,0,0,1,1,1,2,4,	4,4,4,2,2,1,1,0,	0,0,2,1,0,1,3,3,	2,2,2,1,1,1,1,1,1,	},

	{0,0,0,1,1,1,1,1,	2,4,2,2,2,1,1,0,	0,0,1,3,3,1,1,1,	3,1,3,3,1,2,2,2,2,	},
	{0,0,0,2,2,2,2,2,	2,4,2,2,2,2,2,0,	5,5,2,2,2,2,2,2,	2,2,2,2,2,2,2,2,2,	},
	{0,0,0,2,2,2,2,0,	0,2,2,3,2,2,2,1,	5,5,0,5,2,2,2,1,	1,3,2,3,2,1,1,1,1,	},
	{0,0,0,2,2,2,2,0,	0,3,3,3,3,2,2,0,	5,5,5,5,0,2,2,2,	2,3,3,3,2,1,1,1,1,	},

	{0,0,0,2,2,0,2,0,	0,3,3,3,3,0,0,4,	4,4,5,5,0,0,2,2,	2,3,3,2,2,1,1,1,1,	},
	{0,0,0,2,2,0,0,0,	1,1,1,3,3,4,4,4,	0,0,5,2,2,2,2,2,	2,1,1,2,2,1,1,1,1,	},
	{0,0,0,0,0,0,0,0,	0,0,1,4,4,4,3,0,	0,0,0,3,3,1,1,1,	1,1,1,1,1,1,1,1,1,	},
	{0,0,0,2,2,0,0,0,	0,4,4,4,1,1,1,0,	0,0,0,1,3,1,1,1,	1,1,1,1,1,1,1,1,1,	},

	{0,0,0,2,2,0,0,0,	4,4,4,4,4,1,1,0,	0,0,0,1,3,1,1,1,	1,1,1,1,1,1,1,1,1,	},
	{0,0,0,2,2,2,4,4,	4,4,2,2,2,1,1,0,	0,0,2,2,3,1,2,2,	2,2,2,2,1,1,1,1,1,	},
	{0,0,0,2,2,2,4,4,	4,4,2,2,2,1,1,0,	0,0,2,0,0,2,2,1,	1,1,2,1,1,1,1,1,1,	},
	{0,0,0,0,0,2,4,4,	4,4,2,2,2,2,2,0,	0,0,2,0,0,2,2,1,	2,1,2,2,1,1,1,1,1,	},

	{0,0,0,0,4,4,4,4,	1,1,1,1,1,2,2,0,	0,1,1,0,0,0,2,1,	2,1,2,2,2,1,1,1,1,	},
	{0,0,0,0,4,4,4,4,	1,1,1,1,0,0,0,0,	0,1,1,1,1,1,1,1,	1,1,1,1,2,2,2,2,2,	},
	{0,0,0,0,0,4,4,4,	1,1,1,1,0,0,0,0,	0,1,1,1,1,1,1,1,	1,1,1,1,2,2,2,2,2,	},
	{0,0,0,0,0,4,4,4,	1,1,1,1,0,0,0,0,	0,1,1,1,1,1,1,1,	1,1,1,1,2,2,2,2,2,	},
	{0,0,0,0,0,4,4,4,	1,1,1,1,0,0,0,0,	0,1,1,1,1,1,1,1,	1,1,1,1,2,2,2,2,2,	},
};



char* sTsLst[] =
{
	"Base01.png",
	"Base02.png",
	"Base03.png",
	"Base04.png",
	"Base05.png",
	"Base06.png",
};



CLcSplt::CLcSplt()
{
	m_pDev	= NULL;
	m_nVtx	= 0;
	m_pFce	= 0;

	m_pFce	= NULL;
	m_pVtx	= NULL;

	m_iTile	= 0;
	m_fWidth= 0;

	m_nTex	= 0;
	m_pTex	= NULL;

	m_pFVF	= NULL;
	m_pEft	= NULL;
}

CLcSplt::~CLcSplt()
{
	Destroy();	
}


INT CLcSplt::Create(PDEV pDev)
{
	m_pDev	= pDev;

	int i, j, n, m;

	int	nTile	= 32;

	m_iTile		= nTile + 1;		//nTile in x, z
	m_fWidth	= 32;				// Width
	m_nTex		= 6;				// Number of Texture Layer

	m_nVtx = m_iTile * m_iTile;
	m_nFce =  nTile * nTile * 2;

	m_pVtx = new VtxSpl[m_nVtx];
	m_pFce = new VtxIdx[m_nFce];

	
	i=0;
	for(m=0; m< nTile/2; ++m)
	{
		for(n=0;n<nTile/2; ++n)
		{
			WORD s;
			WORD d;
			WORD a;
			
			WORD f[9];
			
			s = nTile + 2;
			
			d=  (nTile+ 1)*2;
			
			a = m * d + n * 2 + s;
			
			f[1] = a +nTile+0;	f[2] = a + nTile+1;	f[3] = a +nTile+2;
			f[8] = a - 1;		f[0] = a;			f[4] = a + 1;
			f[7] = a -nTile-2;	f[6] = a - nTile-1;	f[5] = a -nTile-0;
			
			m_pFce[i+0] = VtxIdx( f[0], f[1], f[2]);
			m_pFce[i+1] = VtxIdx( f[0], f[2], f[3]);
			m_pFce[i+2] = VtxIdx( f[0], f[3], f[4]);
			m_pFce[i+3] = VtxIdx( f[0], f[4], f[5]);
			m_pFce[i+4] = VtxIdx( f[0], f[5], f[6]);
			m_pFce[i+5] = VtxIdx( f[0], f[6], f[7]);
			m_pFce[i+6] = VtxIdx( f[0], f[7], f[8]);
			m_pFce[i+7] = VtxIdx( f[0], f[8], f[1]);
			
			i +=8;
		}
	}
		
	for(j=0; j<m_iTile; ++j)
	{
		for(i=0; i<m_iTile; ++i)
		{
			VtxSpl*	pVtx = &m_pVtx[j*m_iTile+i];
			
			pVtx->p =	VEC3(i * m_fWidth, 0, j * m_fWidth);
			pVtx->u =	i/5.f;
			pVtx->v =	j/5.f;

			FLOAT h;
			FLOAT fTx = i-m_iTile/2.f;		// 지형의 중심점 X
			FLOAT fTz = j-m_iTile/2.f;		// 지형의 중심점 Z

			fTx *= fTx;
			fTz *= fTz;
			fTz += fTx;
			fTz *=-0.05f;
			h = expf( fTz) *200.f;
//			h = 40*(cosf(x/ FLOAT(m_TileN*.1F) )*cosf(z/ FLOAT(m_TileN*.1F) )+1.0f);

			pVtx->p.y = h;
		}
	}

	
	m_pTex = new TexWgt[m_nTex];
	
	for (i = 0; i < m_nTex; ++i)
	{
		char	sFile[MAX_PATH]={0};
		sprintf(sFile, "Texture/%s", sTsLst[i + 0]);
		TextureLoad(sFile, m_pTex[i].pTexB);
	}


	if(FAILED(VertexDeclarator()))
		return -1;

	if(FAILED(BuildHlsl()))
		return -1;


	CalculateMap();

	SetupNormal();

	return 0;
}


void CLcSplt::Destroy()
{
	SAFE_DELETE_ARRAY(	m_pFce	);
	SAFE_DELETE_ARRAY(	m_pVtx	);
	
	SAFE_DELETE_ARRAY(	m_pTex	);

	SAFE_RELEASE(	m_pFVF		);
	SAFE_RELEASE(	m_pEft	);		// 셰이더
}



INT CLcSplt::Restore()
{
	m_pEft->OnResetDevice();
	
	return 0;
}


void CLcSplt::Invalidate()
{
	m_pEft->OnLostDevice();
}



INT CLcSplt::FrameMove()
{
	return 0;
}


void CLcSplt::Render()
{
	INT	i =0;
	HRESULT hr = -1;


	// 행렬설정
	D3DXMATRIX mtViw;
	D3DXMATRIX mtPrj;
	
	D3DXVECTOR4 vcLgt;
	char	sTx[128]={0};
	
	m_pDev->GetTransform( D3DTS_VIEW,  &mtViw );
	m_pDev->GetTransform( D3DTS_PROJECTION,  &mtPrj );

	hr = m_pDev->SetVertexDeclaration( m_pFVF );			// 정점선언
	
	hr = m_pEft->SetMatrix( "m_mtViw", &mtViw);				// 프로젝션 행렬
	hr = m_pEft->SetMatrix( "m_mtPrj", &mtPrj);				// 뷰 행렬
	hr = m_pEft->SetTechnique( "Tech0" );

	for(i=0; i<m_nTex;++i)
	{
		sprintf(sTx, "m_TxDif%d", i);
		hr = m_pEft->SetTexture( sTx, m_pTex[i].pTexB);		// 색상맵
	}

	hr = m_pEft->Begin( NULL, 0 );
	hr = m_pEft->Pass( 0 );
	
	hr = m_pDev->DrawIndexedPrimitiveUP(
						D3DPT_TRIANGLELIST
						, 0
						, m_nVtx
						, m_nFce
						, m_pFce
						, D3DFMT_INDEX16
						, m_pVtx
						, sizeof(CLcSplt::VtxSpl)
						);
	
	m_pEft->End();

	hr = m_pDev->SetVertexDeclaration(NULL);
	hr = m_pDev->SetVertexShader(NULL);
	hr = m_pDev->SetPixelShader(NULL);


	m_pDev->SetTexture(0, NULL);

	D3DMATERIAL9	m_Mtl;
	D3DLIGHT9		m_Lgt;
	memset( &m_Mtl, 0, sizeof(m_Mtl) );
	memset( &m_Lgt, 0, sizeof(m_Lgt) );
	m_Lgt.Type = D3DLIGHT_DIRECTIONAL;
	
	VEC3 vecDir;
	vecDir = VEC3( 1.f, -1.f, 1.0f);
	D3DXVec3Normalize( (VEC3*)&m_Lgt.Direction, &vecDir );
	
	m_Lgt.Specular.r = 1.f;
	m_Lgt.Specular.g = 1.f;
	m_Lgt.Specular.b = 1.f;
	m_Lgt.Specular.a = 1.f;
	
	m_Lgt.Diffuse.r = 1.f;
	m_Lgt.Diffuse.g = 1.f;
	m_Lgt.Diffuse.b = 1.f;
	m_Lgt.Diffuse.a = 1.f;
	
	m_Lgt.Range = 1000;
	m_Lgt.Falloff = 0;
	m_Lgt.Attenuation0 = 1;
	m_Lgt.Attenuation1 = 0;
	m_Lgt.Attenuation2 = 0;
	
	m_Mtl.Diffuse.r = 1.f;
	m_Mtl.Diffuse.g = .5f;
	m_Mtl.Diffuse.b = 0.f;
	m_Mtl.Diffuse.a = 0.f;
	m_Mtl.Specular.r = 1.f;
	m_Mtl.Specular.g = 0.5f;
	m_Mtl.Specular.b = 0.f;
	m_Mtl.Specular.a = 0.f;

	m_Mtl.Power = 20;

	
	m_pDev->SetLight( 0, &m_Lgt );
	m_pDev->LightEnable( 0, TRUE );
	m_pDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_LIGHTING, FALSE);
	
	
	m_pDev->SetMaterial( &m_Mtl );

	m_pDev->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );


//	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	m_pDev->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE);
	m_pDev->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE);
	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  TRUE);
	m_pDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
	m_pDev->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_ZERO);

	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE);
	

	m_pDev->SetFVF(CLcSplt::VtxSpl::FVF);


	hr = m_pDev->DrawIndexedPrimitiveUP(
						D3DPT_TRIANGLELIST
						, 0
						, m_nVtx
						, m_nFce
						, m_pFce
						, D3DFMT_INDEX16
						, m_pVtx
						, sizeof(CLcSplt::VtxSpl)
						);

	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}



////////////////////////////////////////////////////////////////////////////////
void CLcSplt::CalculateMap()
{
	INT i;

	for(i=0; i<m_nTex; ++i)
	{
		CalculateMapTile(i);
	}
}


void CLcSplt::CalculateMapTile(int nTx)
{
	INT		x, z, m, n;
	INT		nXBgn, nXEnd;
	INT		nZBgn, nZEnd;

	FLOAT	fAlpha;
	FLOAT	fN;
	FLOAT	fEpsilon= 10;

	for (z=0; z<m_iTile; ++z)
	{
		for (x=0; x<m_iTile; ++x)
		{
			fAlpha = 0.0f;

			if(z==0)
			{
				if(x==0)
				{
					nXBgn	= 0;	nXEnd	= 1;
					nZBgn	= 0;	nZEnd	= 1;
				}

				else if(x==(m_iTile-1))
				{
					nXBgn	= -1;	nXEnd	= 0;
					nZBgn	= 0;	nZEnd	= 1;
				}

				else
				{
					nXBgn	= -1;	nXEnd	= 1;
					nZBgn	= 0;	nZEnd	= 1;
				}
			}

			else if(z==(m_iTile-1))
			{
				if(x==0)
				{
					nXBgn	= 0;	nXEnd	= 1;
					nZBgn	= -1;	nZEnd	= 0;
				}

				else if(x==(m_iTile-1))
				{
					nXBgn	= -1;	nXEnd	= 0;
					nZBgn	= -1;	nZEnd	= 0;
				}

				else
				{
					nXBgn	= -1;	nXEnd	= 1;
					nZBgn	= -1;	nZEnd	= 0;
				}
			}

			else
			{
				if(x==0)
				{
					nXBgn	= 0;	nXEnd	= 1;
					nZBgn	= -1;	nZEnd	= 1;
				}

				else if(x==(m_iTile-1))
				{
					nXBgn	= -1;	nXEnd	= 0;
					nZBgn	= -1;	nZEnd	= 1;
				}

				else
				{
					nXBgn	= -1;	nXEnd	= 1;
					nZBgn	= -1;	nZEnd	= 1;
				}
			}

			for(m=nZBgn; m<=nZEnd; ++m)
			{
				for(n=nXBgn; n<=nXEnd; ++n)
				{
					if(m_IdxLA[z+m][x+n] !=nTx)
						continue;

					if(0==m && 0==n)
						fAlpha +=fEpsilon;
					else
						fAlpha +=1.f;
				}
			}

			fN = 2 * (abs(nXEnd-nXBgn) + abs(nZEnd-nZBgn)) -1 + fEpsilon;

			fAlpha /= fN;


			VEC6 _color;
			
			memcpy(&_color, &m_pVtx[m_iTile*z + x].ts[0], sizeof(VEC6));

			_color[nTx] = fAlpha;

			memcpy(&m_pVtx[m_iTile*z + x].ts[0], &_color, sizeof(VEC6));
		}// for
	}// for
}



void CLcSplt::SetupNormal()
{
	int x;
	int z;
	int	k;
	int n;
	INT	nVtxT	= m_iTile;
	INT	nTile	= m_iTile -1;

	VEC3	Normal(0,0,0);
	VEC3	*v0, *v1, *v2;
	VEC3	Nor;


	for(z=1; z<nTile; ++z)
	{
		for(x=1; x<nTile; ++x)
		{
			n = z * nVtxT + x;

			INT nIdx[6][3]=
			{
				{ n, n-1       , n-nVtxT   },
				{ n, n-nVtxT   , n-nVtxT+1 },
				{ n, n-nVtxT+1 , n+1       },
				{ n, n+1       , n+nVtxT   },
				{ n, n+nVtxT   , n+nVtxT-1 },
				{ n, n+nVtxT-1 , n-1       },
			};

			Normal = VEC3(0,0,0);

			for(k=0; k<6; ++k)
			{
				v0 = &m_pVtx[  nIdx[k][0] ].p;
				v1 = &m_pVtx[  nIdx[k][1] ].p;
				v2 = &m_pVtx[  nIdx[k][2] ].p;

				CalculateNormal(&Nor, v0, v1, v2);
				Normal +=Nor;
			}

			Normal /=6.f;
			D3DXVec3Normalize(&Normal,&Normal);

			m_pVtx[n].n = Normal;
		}
	}



	//왼쪽
	for(z=1; z<nTile; ++z)
	{
		x = 0;
		n = z * nVtxT + x;

		INT nIdx[3][3]=
		{
			{ n, n-nVtxT   , n-nVtxT+1 },
			{ n, n-nVtxT+1 , n+1       },
			{ n, n+1       , n+nVtxT   },
		};

		Normal = VEC3(0,0,0);

		for(k=0; k<3; ++k)
		{
			v0 = &m_pVtx[  nIdx[k][0] ].p;
			v1 = &m_pVtx[  nIdx[k][1] ].p;
			v2 = &m_pVtx[  nIdx[k][2] ].p;

			CalculateNormal(&Nor, v0, v1, v2);
			Normal +=Nor;
		}

		Normal /=3.f;
		D3DXVec3Normalize(&Normal,&Normal);

		m_pVtx[n].n = Normal;
	}


	//오른쪽
	for(z=1; z<nTile; ++z)
	{
		x = nTile;
		n = z * nVtxT + x;

		INT nIdx[3][3]=
		{
			{ n, n-1       , n-nVtxT   },
			{ n, n+nVtxT   , n+nVtxT-1 },
			{ n, n+nVtxT-1 , n-1       },
		};

		Normal = VEC3(0,0,0);

		for(k=0; k<3; ++k)
		{
			v0 = &m_pVtx[  nIdx[k][0] ].p;
			v1 = &m_pVtx[  nIdx[k][1] ].p;
			v2 = &m_pVtx[  nIdx[k][2] ].p;

			CalculateNormal(&Nor, v0, v1, v2);
			Normal +=Nor;
		}

		Normal /=3.f;
		D3DXVec3Normalize(&Normal,&Normal);

		m_pVtx[n].n = Normal;
	}



	//아래
	for(x=1; x<nTile; ++x)
	{
		z = 0;
		n = z * nVtxT + x;

		INT nIdx[3][3]=
		{
			{ n, n+1       , n+nVtxT   },
			{ n, n+nVtxT   , n+nVtxT-1 },
			{ n, n+nVtxT-1 , n-1       },
		};

		Normal = VEC3(0,0,0);

		for(k=0; k<3; ++k)
		{
			v0 = &m_pVtx[  nIdx[k][0] ].p;
			v1 = &m_pVtx[  nIdx[k][1] ].p;
			v2 = &m_pVtx[  nIdx[k][2] ].p;

			CalculateNormal(&Nor, v0, v1, v2);
			Normal +=Nor;
		}

		Normal /=3.f;
		D3DXVec3Normalize(&Normal,&Normal);

		m_pVtx[n].n = Normal;
	}


	//위
	for(x=1; x<nTile; ++x)
	{
		z = nTile;
		n = z * nVtxT + x;

		INT nIdx[6][3]=
		{
			{ n, n-1       , n-nVtxT   },
			{ n, n-nVtxT   , n-nVtxT+1 },
			{ n, n-nVtxT+1 , n+1       },
		};

		Normal = VEC3(0,0,0);

		for(k=0; k<3; ++k)
		{
			v0 = &m_pVtx[  nIdx[k][0] ].p;
			v1 = &m_pVtx[  nIdx[k][1] ].p;
			v2 = &m_pVtx[  nIdx[k][2] ].p;

			CalculateNormal(&Nor, v0, v1, v2);
			Normal +=Nor;
		}

		Normal /=3.f;
		D3DXVec3Normalize(&Normal,&Normal);

		m_pVtx[n].n = Normal;
	}


	//(0,0)
	{
		x = 0;
		z = 0;
		n = z * nVtxT + x;

		v0 = &m_pVtx[  n      ].p;
		v1 = &m_pVtx[  n+1    ].p;
		v2 = &m_pVtx[  n+nVtxT].p;

		CalculateNormal(&Nor, v0, v1, v2);
		m_pVtx[n].n = Nor;
	}


	//(m_TileN,0)
	{
		x = nTile;
		z = 0;
		n = z * nVtxT + x;

		INT nIdx[2][3]=
		{
			{ n, n+nVtxT   , n+nVtxT-1 },
			{ n, n+nVtxT-1 , n-1       },
		};

		Normal = VEC3(0,0,0);

		for(k=0; k<2; ++k)
		{
			v0 = &m_pVtx[  nIdx[k][0] ].p;
			v1 = &m_pVtx[  nIdx[k][1] ].p;
			v2 = &m_pVtx[  nIdx[k][2] ].p;

			CalculateNormal(&Nor, v0, v1, v2);
			Normal +=Nor;
		}

		Normal /=2.f;
		D3DXVec3Normalize(&Normal,&Normal);

		m_pVtx[n].n = Normal;
	}


	//(0, m_TileN)
	{
		x = 0;
		z = nTile;
		n = z * nVtxT + x;


		INT nIdx[2][3]=
		{
			{ n, n-nVtxT   , n-nVtxT+1 },
			{ n, n-nVtxT+1 , n+1       },
		};

		Normal = VEC3(0,0,0);

		for(k=0; k<2; ++k)
		{
			v0 = &m_pVtx[  nIdx[k][0] ].p;
			v1 = &m_pVtx[  nIdx[k][1] ].p;
			v2 = &m_pVtx[  nIdx[k][2] ].p;

			CalculateNormal(&Nor, v0, v1, v2);
			Normal +=Nor;
		}

		Normal /=2.f;
		D3DXVec3Normalize(&Normal,&Normal);

		m_pVtx[n].n = Normal;
	}


	//(m_TileN, m_TileN)
	{
		x = nTile;
		z = nTile;
		n = z * nVtxT + x;

		v0 = &m_pVtx[  n      ].p;
		v1 = &m_pVtx[  n-1    ].p;
		v2 = &m_pVtx[  n-nVtxT].p;

		CalculateNormal(&Nor, v0, v1, v2);
		m_pVtx[n].n = Nor;
	}

}


void CLcSplt::CalculateNormal(VEC3* pOut
							  , const VEC3* v0
							  , const VEC3* v1
							  , const VEC3* v2)
{
	// Normalvector계산
	// 1. D3DXVec3Cross;
	// 2. D3DXVec3Normalize;

	VEC3 n;
	VEC3 A = *v2 - *v0;
	VEC3 B = *v1 - *v0;
	D3DXVec3Cross(&n, &A, &B);
	D3DXVec3Normalize(&n, &n);

	*pOut = n;
}

INT CLcSplt::TextureLoad(char* sFileName
						, PDTX& texture
						, DWORD _color
						, D3DXIMAGE_INFO*pSrcInfo
						, DWORD Filter
						, DWORD MipFilter
						, D3DFORMAT d3dFormat)
{
	if ( FAILED(D3DXCreateTextureFromFileEx(
		m_pDev
		, sFileName
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, d3dFormat
		, D3DPOOL_MANAGED
		, Filter
		, MipFilter
		, _color
		, pSrcInfo
		, NULL
		, &texture
		)) )
	{
		texture = NULL;
		return -1;
	}
	
	return 0;
}



INT CLcSplt::VertexDeclarator()
{
	PDVD pFVF = NULL;

	DWORD	dFVF = CLcSplt::VtxSpl::FVF;

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE];
	memset(vertex_decl, 0, sizeof(vertex_decl));
	D3DXDeclaratorFromFVF(dFVF, vertex_decl);

	if(FAILED(m_pDev->CreateVertexDeclaration( vertex_decl, &pFVF )))
		return -1;

	m_pFVF = pFVF;

	return 0;
}


INT CLcSplt::BuildHlsl()
{
	HRESULT	hr;
	DWORD dwFlags = 0;

#if defined( _DEBUG ) || defined( DEBUG )
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER	pErr = NULL;
	

	hr = D3DXCreateEffectFromFile(
					m_pDev
					, "Data/hlsl.fx"
					, NULL
					, NULL
					, dwFlags
					, NULL
					, &m_pEft
					, &pErr );
	
	if( FAILED(hr))
	{

		MessageBox( GetActiveWindow(), (char*)pErr->GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErr);
		return -1;
	}


	return 0;
}

