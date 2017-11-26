// Implementation of the CLcxtObj classes.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxtObj_H_
#define _LcxtObj_H_


class CLcxtObj : public ILcxtObj
{
protected:
	char					m_sType[32];
	LPDIRECT3DDEVICE9		m_pDev;

public:
	CLcxtObj();
	virtual ~CLcxtObj();

	virtual INT		Query(char* sCmd, void* pData);
};


#endif

