// Interface for the CTblSmd class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TblSmd_H_
#define _TblSmd_H_


#pragma warning(disable : 4786)
#include <vector>


class CTblSmd : public ILcxTbl
{
public:
	struct TbS
	{
		char	sF[128];														// File Name
		INT		nT;																// Type
		void*	pT;																// Data Pointer

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
	CTblSmd();
	virtual ~CTblSmd();
	
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
