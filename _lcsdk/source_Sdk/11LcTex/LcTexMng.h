// Interface for the CLcTexMng class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcTexMng_H_
#define _LcTexMng_H_

#pragma warning(disable : 4786)
#include <string>
#include <map>


typedef	std::map<std::string, ILcTex* >	mpTLcTex;
typedef	mpTLcTex::iterator				itTLcTex;

class CLcTexMng : public ILcTexMng
{
protected:
	INT			m_nId;					// Manager Id
	char		m_sName[MAX_PATH];		// Manager Name

	mpTLcTex	m_lsTex;
	PDEV		m_pDev;

public:
	CLcTexMng();
	virtual ~CLcTexMng();
	
	// Basic Interface
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();

	virtual INT		Query(char* sCmd, void* pData);


	// Management Interface
	virtual INT		Find(char* sName, ILcTex** pData);
	virtual INT		FindCreate(char* sCmd
								, ILcTex** pData
								, void* pVal1
								, void* pVal2 = NULL
								, DWORD dColorKey=0x00FFFFFF
								, DWORD dFilter= D3DMX_FILTER_NONE
								, DWORD Pool= D3DPOOL_SYSTEMMEM);
};


#endif
