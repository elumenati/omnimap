

ClearScreenShapes()
Clear_Channels()
Clear()
createadome     = false
createacylinder = false
createapanorama = false
usestencilmask  = false

SetOmnimapParameter("RendererType", GLOBAL_renderer)
SetOmnimapParameter("displayHUD",GLOBAL_DisplayHUD)
SetDomeRadius(GLOBAL_domeRadius)
----------------------------------------------------------------------------
ConsolePrintString(string.format("Screenshape file : defaults/%sDefaults.lua",Screenshape))
dofile(AddSupportDirectory(string.format("defaults/%sDefaults.lua",Screenshape)))

----------------------------------------------------------------------------

Projector:Update()

----------------------------------------------------------------------------

