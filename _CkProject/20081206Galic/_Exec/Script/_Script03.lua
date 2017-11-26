--
-- Begin Phase Script
--

Lsys.DoFile("Script/input.lua")


D3DMX_PI = 3.141592654

mtWorld =
{
	  1.0, 0.0, 0.0, 0.0
	, 0.0, 1.0, 0.0, 0.0
	, 0.0, 0.0, 1.0, 0.0
	, 0.0, 0.0, 0.0, 1.0
}


function Lua_Init()
	Ld3d.SetClearColor("0xFF006699")
	g_nFnt	= Lfont.New("HY¾èÀº»ù¹°M", 40, 2, 0, "0XFFFFFFFF", "0XFF111111", 1, 1)

	LxCam.SetEye  (10, 800., 800.,-800.)
	LxCam.SetLook (10,   0.,   0.,   0.)
	LxCam.SetUp   (10,   0.,   1.,   0.)
	LxCam.SetBasis(10, 800., 800., 800.)
	LxCam.SetType (10, 1)	-- 1ÀÎÄª Ä«¸Þ¶ó·Î ÀüÈ¯(2ÀÌ¸é 3ÀÎÄª)


	g_nXmsh	= Lxmsx.New("SampleXfile/tiny.x", "SampleXfileTexture/", "SampleXFile/Hlsl.fx")

	if g_nXmsh < 0 then
		return -1
	end

	return 0
end


function Lua_Destroy()
	return 0
end


function Lua_FrameMove()
	UpdateInput()

	-- ascii 37 is Left, 39 is Right
	if 1 == g_Keyboard[37] then
		return 2
	elseif 1 == g_Keyboard[39] then
		return 4
	end

	-- 'W'
	if 3 == g_Keyboard[64+23] then 
		LxCam.MoveForward(10, 4);
	end

	-- 'S'
	if 3 == g_Keyboard[64+19] then 
		LxCam.MoveForward(10, -4);
	end

	-- 'D'
	if 3 == g_Keyboard[64+4] then 
		LxCam.MoveSideward(10, 4);
	end

	-- 'A'
	if 3 == g_Keyboard[64+1] then 
		LxCam.MoveSideward(10, -4);
	end


	if 3 == MouseEvntR then 
		local ex, ey, ez = Lin.MouseDelta()

		sMsg = string.format("Input: %f %f %f", ex, ey, ez)
		print(sMsg)
		LxCam.RotateYaw(10, ex, 0.3);
		LxCam.RotatePitch(10, ey, 0.3);
	end

	LxCam.FrameMove()

	return 0

end

function Lua_Render()

	LxCam.SetTransformView(10)
	LxCam.SetTransformProj(10)

	Ld3d.SetRenderState( "LIGHTING", "FALSE" )
	LxUtil.Command("Render Grid")

--	Ld3d.SetRenderState( "LIGHTING", "TRUE" )
	Lxmsx.Render(g_nXmsh)

	Lfont.Draw(g_nFnt, "XFile Skinning", 100, 50, "0xFFFFFFFF")

	return 0
end


