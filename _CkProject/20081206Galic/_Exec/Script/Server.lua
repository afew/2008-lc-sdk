--
-- Begin Phase Script
-- Phase Number: 3

Lsys.DoFile("Script/input.lua")


function Lua_Init()
	Lsys.ShowState( 1 )
	Ld3d.SetClearColor("0xFF006699")

	g_nFnt	= Lfont.New("HY����L", 32, 1, 0, "0XFFFF0000", "0XFFFFFF00", 1, 1)


	Lapp.SetCommand("Add Server List", "Server1")
	Lapp.SetCommand("Add Server List", "Server2")
	Lapp.SetCommand("Add Server List", "Server3")
	Lapp.SetCommand("Add Server List", "Server4")
	Lapp.SetCommand("Add Server List", "Server5")
	Lapp.SetCommand("Add Server List", "Server6")
	Lapp.SetCommand("Add Server List", "Server7")

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


	if 1 == g_Keyboard[13] then
		Lapp.SetCommand("Select Server", 3)
		return 7
	end

	return 0

end

function Lua_Render()
	Lfont.Draw(g_nFnt, "��������", 100, 50, "0xFFFFFFFF")

	return 0
end
