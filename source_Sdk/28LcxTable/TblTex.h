// Interface for the CTblTex class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TblTex_H_
#define _TblTex_H_


#pragma warning(disable : 4786)
#include <vector>


class CTblTex : public ILcxTbl
{
public:
	struct TbS
	{
		char				sF[128];											// Tx File
		D3DXIMAGE_INFO		pI;
		DWORD				dF;													// Filter
		DWORD				dK;													// Color Key

		LPDIRECT3DTEXTURE9	pT;													// Texture Pointer

		TbS();
		~TbS();
	};


	struct TbM
	{
		char				sP[260];											// Tx Path
		std::vector<TbS* >	vS;
		
		TbM();
		~TbM();
	};


public:
	char				m_sF[260];
	char				m_sV[32];												// version
	LPDIRECT3DDEVICE9	m_pDev;
	std::vector<TbM* >	m_vM;
	
public:
	CTblTex();
	virtual ~CTblTex();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();

	virtual	INT		Select(void** pOut, INT nM, INT nS);
	virtual	INT		Select(void** pOut, INT* nM, INT* nS, char* sFile);
	virtual	INT		SelectInfo(void** pOut, INT nM, INT nS);

protected:
	INT		Load     (char *sFile);
	INT		CreateSub(INT nM, INT nS, void* pEtc=NULL);
	INT		Delete	 (INT nM, INT nS);

	void	Confirm();
};

#endif
