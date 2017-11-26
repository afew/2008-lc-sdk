
--
-- 초기 설정
-- Date: 2006-10-19

function Lua_Create()
	Lsys.ScriptFile( 1, "Script/PlayField.lua")
	Lsys.ScriptFile( 2, "Script/Login.lua")
	Lsys.ScriptFile( 3, "Script/Server.lua")
	Lsys.ScriptFile( 4, "Script/CharSelect.lua")
	Lsys.ScriptFile( 5, "Script/CharCreate.lua")
	Lsys.ScriptFile( 6, "Script/PlayTown.lua")
	Lsys.ScriptFile( 7, "Script/PlayField.lua")
	Lsys.ScriptFile( 8, "Script/End.lua")


	Lsys.ConsoleSet(1)									-- Using Console window
	Lsys.SetClearColor("0xFF006699")
	Lsys.ShowCursor(1)									--윈도 마우스포인터를 가린다

	-- Window create. position x, position y, screen width, screen height, title, fullmode?...
	return Lsys.CreateWindow(-1, -1, 1024, 768, "Galic Soft(®) Game Application(v0.8)", 0)
end

