---  to do...
---  add a pin interface to pinthe renderings to the top or bottom
---  this will ensure that the image looks right even on widescreen laptops or in a window.
function LensCrop_HD()
	
end

function LensCrop_OmniConfig()
	SetglViewportsettings(OC_offsetX, OC_offsetY, OC_scaling)
end

function LensCrop_HDome()
	OC_offsetX = 0.0
	OC_offsetY = .125
	OC_scaling = 0.0
	SetglViewportsettings(0,1.0/8.0,0)
end

function LensCrop_VDome()
	OC_offsetX = 0.0
	OC_offsetY = -.125
	OC_scaling = 0.0
	SetglViewportsettings(0,-1.0/8.0,0)
end

function LensCrop_PDome()
	OC_offsetX = 0.0
	OC_offsetY = -7.0/32.0
	OC_scaling = 0.0
	SetglViewportsettings(0,-7.0/32.0 ,0)
end
 

function LensCrop_FullDome()
	OC_offsetX = 0.0
	OC_offsetY = 0.0
	OC_scaling = 1.0
	SetglViewportsettings(0,0,1)
end

function LensCrop_CustomDome()
	OC_offsetX = 0.0
	OC_offsetY = -.125
	OC_scaling = 0.0
	SetglViewportsettings(0,-1.0/8.0,0)
end



function SetglViewportsettings(centeroffsetX_inRadius, centeroffsetY_inRadius, HorizontalOrVertScaling) -- scale = one or aratiob

	local w =GetOmnimapNumberParameter("resWidth")
	local h =GetOmnimapNumberParameter("resHeight")
  
	local aratio = h/w
	local aratiob = (HorizontalOrVertScaling)*aratio + (1-HorizontalOrVertScaling)*1

	local r = aratiob * h/aratio

	local centeroffsetY = r*centeroffsetY_inRadius
	local centeroffsetX = r*centeroffsetX_inRadius


	local glViewportsettings2 =w *aratiob
	local glViewportsettings3 =aratiob * h/aratio  
	local glViewportsettings0 =centeroffsetX+(w-glViewportsettings2)/2.0
	local glViewportsettings1 =centeroffsetY+(h-glViewportsettings3)/2.0

	SetOmnimapParameter("glViewportsettings0",glViewportsettings0)
	SetOmnimapParameter("glViewportsettings1",glViewportsettings1)
	SetOmnimapParameter("glViewportsettings2",glViewportsettings2)
	SetOmnimapParameter("glViewportsettings3",glViewportsettings3)

end





function SetProjector(Position,Direction,Up)
-- to do:
-- this should enforce orthognoality between look and up

	local LookPos = Vector_Add(Position,Direction)

	SetOmnimapParameter("ProjectorPosition.pos.x",Position["x"])
	SetOmnimapParameter("ProjectorPosition.pos.y",Position["y"])
	SetOmnimapParameter("ProjectorPosition.pos.z",Position["z"])
	SetOmnimapParameter("ProjectorPosition.lookAtpos.x",LookPos["x"])
	SetOmnimapParameter("ProjectorPosition.lookAtpos.y",LookPos["y"])
	SetOmnimapParameter("ProjectorPosition.lookAtpos.z",LookPos["z"])
	SetOmnimapParameter("ProjectorPosition.headsUp.x",Up["x"])
	SetOmnimapParameter("ProjectorPosition.headsUp.y",Up["y"])
	SetOmnimapParameter("ProjectorPosition.headsUp.z",Up["z"])
end

function SetAudiencePosition(Pos)
	SetOmnimapParameter("AudiencePosition_x",Pos["x"])
	SetOmnimapParameter("AudiencePosition_y",Pos["y"])
	SetOmnimapParameter("AudiencePosition_z",Pos["z"])
end

function SetAudienceView(viewDir, viewHeadsUp)
	SetOmnimapParameter("ViewDirection.x",viewDir["x"])
	SetOmnimapParameter("ViewDirection.y",viewDir["y"])
	SetOmnimapParameter("ViewDirection.z",viewDir["z"])
	
	SetOmnimapParameter("ViewHeadsUp.x",viewHeadsUp["x"])
	SetOmnimapParameter("ViewHeadsUp.y",viewHeadsUp["y"])
	SetOmnimapParameter("ViewHeadsUp.z",viewHeadsUp["z"])
end