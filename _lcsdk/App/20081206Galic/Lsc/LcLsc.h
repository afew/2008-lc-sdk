// Interface for the LcLsc class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcLsc_H_
#define _LcLsc_H_


namespace	LcLua
{

class CLcLsc : public ILcLsc
{
protected:
	char		m_sF[160];
	void*		m_pL		;

	ILcDev*		m_pDevSys	;

public:
	CLcLsc();
	virtual ~CLcLsc();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		Query(char* sCmd, void* pData);
	

public:
	// Lua Glue
	virtual INT		Lua_Init();
	virtual INT		Lua_Destroy();
	virtual INT		Lua_FrameMove();
	virtual INT		Lua_Render();
	virtual INT		Lua_Create();
};




class CLcLscApp : public CLcLsc
{
public:
	CLcLscApp();
	virtual ~CLcLscApp();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		Query(char* sCmd, void* pData);

protected:
	void	ContainerCreate();
	void	ContainerDestroy();

	void	StaticContainerCreate();
	void	StaticContainerDestroy();
	void	OpenLib();
};


}// namespace Lua

#endif


