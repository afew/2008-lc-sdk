// Interface for the LscTex class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscTex_H_
#define _LscTex_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{
	
struct LscTex
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT			Alloc(char* sFile, DWORD dc=0x00FFFFFF, DWORD dF=D3DX_FILTER_NONE);
		INT			ReAlloc(INT nKey, char* sFile, DWORD dc=0x00FFFFFF, DWORD dF=D3DX_FILTER_NONE);
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
		ILcLsc::LscH* Find(INT* nKey, char* sFile);		
	};

	static	ILcDev*			m_pDevSys	;
	static	void*			m_pDevD3D	;
	static	ILcDevSprite*	m_pDev2D	;										// 2D Sprite
	static	INT				Create(ILcDev* pDev);

	static	ILcCont*		m_pContAuto	;										// 지역 변수
	static	ILcCont*		m_pContStat	;										// 정적 변수
	

	static INT	Lsc_Alloc(lua_State *pL);										// 이미지 파일 로딩
	static INT	Lsc_ReAlloc(lua_State *pL);										// 이미지 파일 다시 로딩
	static INT	Lsc_Release(lua_State *pL);										// 이미지 파일 해제

	static INT	Lsc_TextureWidth(lua_State *pL);								// 이미지 파일 너비
	static INT	Lsc_TextureHeight(lua_State *pL);								// 이미지 파일 높이
	static INT	Lsc_TexturePointer(lua_State *pL);								// Get texture Point
	static INT	Lsc_TextureDraw(lua_State *pL);									// 이미지 그리기
	static INT	Lsc_TextureDrawMask(lua_State *pL);								// 배경 레이어와 같이 이미지 그리기

	static INT	Lsc_SetTexture(lua_State *pL);									// Device->SetTexture
	static INT	Lsc_SetTextureNull(lua_State *pL);

	static INT	OpenLib(lua_State *pL);

	
	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif
