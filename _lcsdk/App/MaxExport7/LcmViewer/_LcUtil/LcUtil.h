// Interface for Utilities.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcUtil_H_
#define _LcUtil_H_


#define SAFE_NEWINIT(p, CLASSTYPE)												\
{																				\
	if(NULL == (p))																\
	{																			\
		p = new CLASSTYPE;														\
																				\
		if(!(p))																\
		{																		\
			return -1;															\
		}																		\
																				\
		if(FAILED((p)->Init()))													\
		{																		\
			delete p;															\
			p = NULL;															\
			return -1;															\
		}																		\
	}																			\
}


#define SAFE_NEWCREATE1(p, CLASSTYPE, p1 )										\
{																				\
	if(NULL == (p))																\
	{																			\
		p = new CLASSTYPE;														\
																				\
		if(!(p))																\
		{																		\
			return -1;															\
		}																		\
																				\
		if(FAILED((p)->Create( p1 )))											\
		{																		\
			delete p;															\
			p = NULL;															\
			return -1;															\
		}																		\
	}																			\
}


#define SAFE_NEWCREATE2(p, CLASSTYPE, p1, p2)									\
{																				\
	if(NULL == (p))																\
	{																			\
		p = new CLASSTYPE;														\
																				\
		if(!(p))																\
		{																		\
			return -1;															\
		}																		\
																				\
		if(FAILED((p)->Create( p1 , p2 )))										\
		{																		\
			delete p;															\
			p = NULL;															\
			return -1;															\
		}																		\
	}																			\
}


#define SAFE_NEWCREATE3(p, CLASSTYPE, p1, p2, p3 )								\
{																				\
	if(NULL == (p))																\
	{																			\
		p = new CLASSTYPE;														\
																				\
		if(!(p))																\
		{																		\
			return -1;															\
		}																		\
																				\
		if(FAILED((p)->Create( p1 , p2 , p3 )))									\
		{																		\
			delete p;															\
			p = NULL;															\
			return -1;															\
		}																		\
	}																			\
}

	
#define SAFE_RESTORE(p)															\
{																				\
	if(p)																		\
	{																			\
		if(FAILED((p)->Restore()))												\
			return -1;															\
	}																			\
}



#define SAFE_FRAMEMOVE(p)														\
{																				\
	if(p)																		\
	{																			\
		if(FAILED(	(p)->FrameMove()))											\
			return -1;															\
	}																			\
}

#define SAFE_DESTROY(p)		{	if(p)	(p)->Destroy();			}
#define SAFE_INVALIDATE(p)	{	if(p)	(p)->Invalidate();		}
#define SAFE_RENDER(p)		{	if(p)	(p)->Render();			}

#define	SAFE_FREE(p)		{ if(p) { free(p);		(p)=NULL; } }


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


// Error MessageBox
void LcUtil_ErrMsgBox(HWND hWnd, char *format,...);

//윈도우 타이틀
void LcUtil_SetWindowTitle(HWND hWnd, char *format,...);


INT LcUtil_TextureLoad(LPDIRECT3DDEVICE9 pDev
						, char * sFile
						, LPDIRECT3DTEXTURE9 & pTx
						, DWORD color=0xFF000000
						, D3DXIMAGE_INFO *pSrcInfo=NULL
						, DWORD Filter   = (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
						, DWORD MipFilter= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
						, D3DFORMAT d3dFormat = D3DFMT_UNKNOWN);


#endif

