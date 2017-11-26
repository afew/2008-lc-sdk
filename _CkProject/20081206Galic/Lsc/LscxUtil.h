// Interface for the LscxUtil class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscxUtil_H_
#define _LscxUtil_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{

struct LscxUtil
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);

	static	void*		m_pGrid		;											// Grid

	static INT	Lscx_CamSetType(lua_State*);
	static INT	Lscx_CamFrameMove(lua_State*);
	static INT	Lscx_CamViewParam(lua_State*);
	static INT	Lscx_CamSetEye(lua_State*);
	static INT	Lscx_CamGetEye(lua_State*);
	static INT	Lscx_CamSetLook(lua_State*);
	static INT	Lscx_CamGetLook(lua_State*);
	static INT	Lscx_CamSetUp(lua_State*);

	static INT	Lscx_CamSetBasis(lua_State*);
	static INT	Lscx_CamSetEpsilonY(lua_State*);

	static INT	Lscx_CamProjParam(lua_State*);
	static INT	Lscx_CamSetFov(lua_State*);
	static INT	Lscx_CamSetScreen(lua_State*);
	static INT	Lscx_CamSetNear(lua_State*);
	static INT	Lscx_CamSetFar(lua_State*);

	static INT	Lscx_CamSetSpeed(lua_State*);
	static INT	Lscx_CamSetControl(lua_State*);

	static INT	Lscx_CamMoveForward(lua_State*);
	static INT	Lscx_CamMoveSideward(lua_State*);
	static INT	Lscx_CamRotateYaw(lua_State*);
	static INT	Lscx_CamRotatePitch(lua_State*);
	static INT	Lscx_CamZoom(lua_State*);

	static INT	Lscx_CamTransformView(lua_State*);
	static INT	Lscx_CamTransformProj(lua_State*);
	static INT	Lscx_CamTransformVP(lua_State*);


	static INT	Lscx_Command(lua_State*);

	static INT	OpenLib(lua_State*);

	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif

