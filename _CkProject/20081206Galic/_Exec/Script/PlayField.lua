--
-- Begin Phase Script
-- Phase Number: 5

Lsys.DoFile("Script/input.lua")


function Lua_Init()
	Lsys.ShowState( 1 )
	Ld3d.SetClearColor("0xFF006699")

	g_nFnt	= Lfont.New("HY엽서L", 32, 1, 0, "0XFFFF0000", "0XFFFFFF00", 1, 1)


	LxCam.SetEye  (0,    0, 500, -1000)
	LxCam.SetLook (0,   20,   0, 40)
	LxCam.SetUp   (0,    0,   1,  0)

	LxCam.SetBasis(0,   180, 0, 170)
	LxCam.SetEpsilonY(0, 20)
	LxCam.SetType (0, 2)	-- 1인칭 카메라로 전환(2이면 3인칭)


	Lapp.SegmentLoad()
	Lapp.FieldLoad("Map/grave04_.bod", "Map/Data/MapHlsl.fx")
	Lapp.FieldViewRange(g_nLcm, 3)
	Lapp.FieldCameraSet(0)	-- Field의 카메라를 0번으로 설정

	-- Major- Minor ==>Man: 0-2, Lady: 1-3
	Lapp.MateModel(0, 0, "Lcm/Character/boy_1204.acm", "Lcm/Character/", "Lcm/Hlsl.fx")
	Lapp.MateModel(0, 1, "Lcm/Character/mong_1.acm", "Lcm/Character/", "Lcm/Hlsl.fx")
	Lapp.MateModel(0, 2, "Lcm/Character/cat_1204.acm", "Lcm/Character/", "Lcm/Hlsl.fx")
	Lapp.MateModel(0, 3, "Lcm/Character/boy_1204.acm", "Lcm/Character/", "Lcm/Hlsl.fx")

	Lapp.MateActive(0, 0, 1)	-- Boy(Man)
	Lapp.MateActive(0, 1, 0)	-- Mong(Lady)
	Lapp.MateActive(0, 2, 1)	-- Cat(Lady)
	Lapp.MateActive(0, 3, 0)	-- .....?

	Lapp.MateRotation(0, 0, 1, 0,	180)	-- Axis(x,y,z), angle(Degree)

	Lapp.MatePosition(0, 0,	 180, 0, 170)		-- Postion X, Y, Z
	Lapp.MatePosition(0, 1,	 140, 0, 140)		-- Postion X, Y, Z
	Lapp.MatePosition(0, 2,	 160, 0, 140)		-- Postion X, Y, Z
	Lapp.MatePosition(0, 3,	 180, 0, 140)		-- Postion X, Y, Z

	g_nXFile = Lxmsx.New("SampleXfile/tiny.x", "SampleXfileTexture/", "SampleXfile/hlsl.fx")

	Lxmsx.Position(g_nXFile, 260, 25, 200)			-- Postion X, Y, Z
	Lxmsx.Scaling(g_nXFile, 0.1, 0.1, 0.1)			-- Scaling X, Y, Z
	Lxmsx.Rotation(g_nXFile, 0, 45, 0)	-- Rotation With X, Y, Z Axis

	return 0
end


function Lua_Destroy()
	return 0
end


function Lua_FrameMove()
	UpdateInput()

--	local	charName = Lapp.GetCharNameMan()
--	Lsys.WindowTitle(charName)

	-- ascii 37 is Left, 39 is Right
	if 1 == g_Keyboard[37] then
		return 5
	elseif 1 == g_Keyboard[39] then
		return 8
	end

	local ex, ey, ez = Lin.MouseDelta()

	-- 'W'
	if 3 == g_Keyboard[64+23] then
		LxCam.MoveForward(0, 4);
	end

	-- 'S'
	if 3 == g_Keyboard[64+19] then
		LxCam.MoveForward(0, -4);
	end

	-- 'D'
	if 3 == g_Keyboard[64+4] then
		LxCam.MoveSideward(0, 4);
	end

	-- 'A'
	if 3 == g_Keyboard[64+1] then
		LxCam.MoveSideward(0, -4);
	end


	if 3 == MouseEvntR then
--		sMsg = string.format("Input: %f %f %f", ex, ey, ez)
--		print(sMsg)
		LxCam.RotateYaw(0, ex, 0.3);
		LxCam.RotatePitch(0, ey, 0.3);
	end

	LxCam.Zoom(0, ez*0.1);

	LxCam.FrameMove()
	LxCam.SetTransformView(0)
	LxCam.SetTransformProj(0)


	Lapp.SegmentFrameMove()
	Lxmsx.FrameMove(g_nXFile)

	return 0

end

function Lua_Render()
	LxCam.SetTransformView(0)
	LxCam.SetTransformProj(0)

	LxUtil.Command("Render Grid")

--	LxUtil.Command("Render Grid")
--	Ld3d.SetRenderState( "FILLMODE", "WIREFRAME" )
	Lapp.SegmentRender()
--	Ld3d.SetRenderState( "FILLMODE", "SOLID" )

--	Lfont.Draw(g_nFnt, "게임 플레이- 대지", 100, 50, "0xFFFFFFFF")

	Lxmsx.Render(g_nXFile)

	return 0
end
