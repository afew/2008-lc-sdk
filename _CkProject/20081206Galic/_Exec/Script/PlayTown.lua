--
-- Begin Phase Script
-- Phase Number: 5

Lsys.DoFile("Script/input.lua")


function Lua_Init()
	Lsys.ShowState( 1 )
	Ld3d.SetClearColor("0xFF006699")

	g_nFnt	= Lfont.New("HY엽서L", 32, 1, 0, "0XFFFF0000", "0XFFFFFF00", 1, 1)

	g_nTown = Lxlcm.New("Lcm/Town/Town.acm", "Lcm/Town", "Lcm/Hlsl.fx")

	LxCam.SetEye  (11,   0., 100., 100.)
	LxCam.SetLook (11,   0.,   0.,   0.)
	LxCam.SetUp   (11,   0.,   1.,   0.)
	LxCam.SetBasis(11,   0., 100., 100.)
	LxCam.SetType (11, 1)	-- 1인칭 카메라로 전환(2이면 3인칭)

	return 0
end


function Lua_Destroy()
	return 0
end


function Lua_FrameMove()
	UpdateInput()

	local	charName = Lapp.GetCharNameMan()
	Lsys.WindowTitle(charName)

	-- ascii 37 is Left, 39 is Right
	if 1 == g_Keyboard[37] then
		return 3
	elseif 1 == g_Keyboard[39] then
		return 7
	end


	-- 'W'
	if 3 == g_Keyboard[64+23] then 
		LxCam.MoveForward(11, 4);
	end

	-- 'S'
	if 3 == g_Keyboard[64+19] then 
		LxCam.MoveForward(11, -4);
	end

	-- 'D'
	if 3 == g_Keyboard[64+4] then 
		LxCam.MoveSideward(11, 4);
	end

	-- 'A'
	if 3 == g_Keyboard[64+1] then 
		LxCam.MoveSideward(11, -4);
	end


	if 3 == MouseEvntR then 
		local ex, ey, ez = Lin.MouseDelta()

		sMsg = string.format("Input: %f %f %f", ex, ey, ez)
		print(sMsg)
		LxCam.RotateYaw(11, ex, 0.3);
		LxCam.RotatePitch(11, ey, 0.3);
	end

	LxCam.FrameMove()

	Lxlcm.FrameMove(g_nTown)

	return 0

end

function Lua_Render()
	LxCam.SetTransformView(11)
	LxCam.SetTransformProj(11)
	Lxlcm.Render(g_nTown)


	Lfont.Draw(g_nFnt, "게임 플레이- 마을", 100, 50, "0xFFFFFFFF")

	return 0
end
