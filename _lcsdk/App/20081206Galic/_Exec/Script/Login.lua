--
-- Begin Phase Script
-- Phase Number: 2

Lsys.DoFile("Script/input.lua")


function Lua_Init()
	Lsys.ShowState( 1 )
	Ld3d.SetClearColor("0xFF006699")

	g_nFnt	= Lfont.New("HY¿±¼­L", 32, 1, 0, "0XFFFF0000", "0XFFFFFF00", 1, 1)

	Lnet.Create("127.0.0.1", "20000")

	return 0
end


function Lua_Destroy()
	return 0
end


nCount = 0

function Lua_FrameMove()
	UpdateInput()

	nCount = nCount + 1

	local	sUid	= "Heesung"
	local	sPwd	= string.format("Count-%d", nCount)

	-- ascii 37 is Left, 39 is Right
	if 1 == g_Keyboard[37] then
		return 1
	elseif 1 == g_Keyboard[13] then
		Lapp.SetCommand("Setup Login UID", sUid)
		Lapp.SetCommand("Setup Login PWD", sPwd)
		Lapp.SetCommand("Login")
		return 3
	end

--	sCmd, sMsg = Lsys.GetCommand()
--	print(sMsg)

	return 0

end

function Lua_Render()
	Lfont.Draw(g_nFnt, "·Î±×ÀÎ", 100, 50, "0xFFFFFFFF")

	return 0
end
