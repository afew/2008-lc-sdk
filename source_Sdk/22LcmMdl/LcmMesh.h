// Interface for the CLcmMesh class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmMesh_H_
#define _LcmMesh_H_


namespace Lcm
{

class CLcmMesh : public ILcmMesh											// For Rendering
{
protected:
	CLcmMesh*	m_pLcOrg;
	TmshInf*	m_pGeo	;

	PDEV		m_pDev	;
	TbndAB		m_BndInf;

	void*		m_pVtx	;													//	VtxD, VtxUV, VtxNUV, VtxNDUV

public:
	CLcmMesh();
	virtual ~CLcmMesh();

	virtual	INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual	void	Destroy();
	virtual	INT		FrameMove();
	virtual	void	Render();

	virtual INT		Query(char* sCmd, void* pData);

	virtual INT		GetType()		;											// Mesh Type, Billboard ,solid, etc.
	virtual void*	GetGeometry()	;
	virtual	void*	GetBufVtx()		;											// Get Vertex Pointer
	virtual	void*	GetBndInf()		;

protected:
	INT		Load(char*);
	void	Clone(void*);	
};

}// namespace Lcm

#endif

