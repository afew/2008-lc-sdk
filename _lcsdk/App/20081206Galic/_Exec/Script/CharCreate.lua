--
-- Character Phase Script
-- Phase Number: 4

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
		return 5
	end

	return 0

end

function Lua_Render()
	Lfont.Draw(g_nFnt, "캐릭터 만들기", 100, 50, "0xFFFFFFFF")

	return 0
end
