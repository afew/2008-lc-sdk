--
-- Begin Phase Script
-- Phase Number: 6

Lsys.DoFile("Script/input.lua")


function Lua_Init()
	Lsys.ShowState( 1 )
	Ld3d.SetClearColor("0xFF006699")

	g_nFnt	= Lfont.New("HY엽서L", 32, 1, 0, "0XFFFF0000", "0XFFFFFF00", 1, 1)

	return 0
end


function Lua_Destroy()
	return 0
end


function Lua_FrameMove()
	UpdateInput()

	-- ascii 37 is Left, 39 is Right
	if 1 == g_Keyboard[37] then
		return 1
	elseif 1 == g_Keyboard[39] then
		return -1
	end

	return 0

end

function Lua_Render()
	Lfont.Draw(g_nFnt, "게임 종료", 100, 50, "0xFFFFFFFF")

	return 0
end
