--
-- Character Select Phase Script


Lsys.DoFile("Script/input.lua")


function Lua_Init()
	Lsys.ShowState( 1 )
	Ld3d.SetClearColor("0xFF006699")

	g_nFnt	= Lfont.New("HY엽서L", 32, 1, 0, "0XFFFF0000", "0XFFFFFF00", 1, 1)


	Lapp.SetCommand("Add Man Character", "ManChar1")
	Lapp.SetCommand("Add Man Character", "ManChar2")
	Lapp.SetCommand("Add Man Character", "ManChar3")
	Lapp.SetCommand("Add Man Character", "ManChar4")
	Lapp.SetCommand("Add Man Character", "ManChar5")
	Lapp.SetCommand("Add Man Character", "ManChar6")
	Lapp.SetCommand("Add Man Character", "ManChar7")

	Lapp.SetCommand("Add Lady Character", "LadyChar1")
	Lapp.SetCommand("Add Lady Character", "LadyChar2")
	Lapp.SetCommand("Add Lady Character", "LadyChar3")
	Lapp.SetCommand("Add Lady Character", "LadyChar4")
	Lapp.SetCommand("Add Lady Character", "LadyChar5")
	Lapp.SetCommand("Add Lady Character", "LadyChar6")
	Lapp.SetCommand("Add Lady Character", "LadyChar7")



	LxCam.SetEye  (10,   0., 50., -300.)
	LxCam.SetLook (10,   0., 50.,   0.)
	LxCam.SetUp   (10,   0.,  1.,   0.)
	LxCam.SetBasis(10,   0., 50., -300.)
	LxCam.SetType (10, 1)	-- 1인칭 카메라로 전환(2이면 3인칭)


	acmNum = 3
	g_nLcm	= {}

	g_nLcm[1] = Lxlcm.New("Lcm/Character/testscale_5.acm", "Lcm/Character/", "Lcm/Hlsl.fx")
	g_nLcm[2] = Lxlcm.New("Lcm/Character/testscale_10.acm", "Lcm/Character/", "Lcm/Hlsl.fx")
	g_nLcm[3] = Lxlcm.New("Lcm/Character/testscale_20.acm", "Lcm/Character/", "Lcm/Hlsl.fx")

	local i
	for i=1, acmNum, 1 do
		local r  =	Lutil.Rand(acmNum)
		Lsys.Sleep(3)
		Lxlcm.SetStartFrame (g_nLcm[i], r)						-- Set Start Frame

		Lxlcm.SetRotation(g_nLcm[i], 0, 1, 0,	0)				-- Axis(x,y,z), angle(Degree)
		Lxlcm.SetPosition(g_nLcm[i], (i-1-(acmNum-1)/2) * 70, 0, 0)		-- Postion X, Y, Z
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
		return 3
	elseif 1 == g_Keyboard[13] then
		Lapp.SetCommand("Setup Man Character Index", 2)
		Lapp.SetCommand("Setup Lady Character Index", 3)

		Lapp.SetCommand("Setup Man Character Name", "Osiris")
		Lapp.SetCommand("Setup Lady Character Name", "Isis")

		Lapp.SetCommand("Select Character")
		return 7
	end



	local ex, ey, ez = Lin.MouseDelta()

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
		sMsg = string.format("Input: %f %f %f", ex, ey, ez)
		print(sMsg)
		LxCam.RotateYaw(10, ex, 0.3);
		LxCam.RotatePitch(10, ey, 0.3);
	end

	LxCam.MoveForward(10, -ez);


	LxCam.FrameMove()

	local i
	for i=1, acmNum, 1 do
		Lxlcm.FrameMove(g_nLcm[i])
	end

	return 0

end

function Lua_Render()

	LxCam.SetTransformView(10)
	LxCam.SetTransformProj(10)

	Ld3d.SetRenderState( "LIGHTING", "FALSE" )
	LxUtil.Command("Render Grid")

	local i
	for i=1, acmNum, 1 do
		Lxlcm.Render(g_nLcm[i])
	end

	Lfont.Draw(g_nFnt, "캐릭터 선택", 100, 50, "0xFFFFFFFF")

	return 0
end
