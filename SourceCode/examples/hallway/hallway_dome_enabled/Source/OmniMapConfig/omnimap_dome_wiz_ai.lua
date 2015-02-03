----------------------------------------------------------------------------
----                        Dome Wiz AI
----------------------------------------------------------------------------
--
--
--  Dome wiz AI, sets 
--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation

ClearScreenShapes()
Clear_Channels()
Clear()


-- Define the globals here
-- these will get set in the dome ai...
--------------------------------------
-- -- Render Channels
--------------------------------------
channelRes = 1024
usefbo = true
nearclip = .1
farclip = 1000
ConsolidateCameraIntoProjectionMatrix = true

R = 1
ZNear = nearclip
ZFar  = farclip 

UseTopChannel	   = true
UseBottomChannel   = true
UseLeftChannel     = true
UseRightChannel    = true
UseFrontChannel    = true
UseBackChannel     = true


	LeftFovY=90
	RightFovY=90
	TopFovY=90 
	BotFovY=90 
	FrontFovY=90
	BackFovY=90
	

Vec_ProjectorLookVec   =  Vector(0,1,0  ) 
Vec_ProjectorUpVec     =  Vector(0,0,1  )
BaseViewDirection      =  Vector(0,0,-1 )
BaseHeadsUp            =  Vector(0,1,0  )  


-- no need to keep this global...???
DomeCenter 	= Vector(0,0,0)
DomeOrientation = Vector(0,1,0)


---------------------------------------------------------------
-----------------  UNIT CONVERSION SETUP  ---------------------
DomeRadiusInInches = MetersToInches(99.9)
DomeRadius_unitless = 1.0
SetupUnitConversionsByDomeRadiusInInches(DomeRadiusInInches)
---------------------------------------------------------------



SetAudiencePosition(AudiencePos)




--  switch final tilt to full dome ai
--  AI needs to do the following
--  DomeOrientation
--  Base View Direction
--  Base headsup Direction
-- projector view
-- projector up
-- chan top
-- chan bot










if(func_CurrentLens == LensCrop_HDome) then
	Vec_ProjectorLookVec   = Vector(0,1,0)
	Vec_ProjectorUpVec     = Vector(0,0,1)
	DomeOrientation = Vector(0,1,0)
	BaseViewDirection = Vector(0,1,0)
	BaseHeadsUp       = Vector(0,0,1)

	LeftFovY=90
	RightFovY=90
	TopFovY=80-- not used
	BotFovY=90
	FrontFovY=90-- not used
	BackFovY=90-- not used
	

	UseTopChannel	   = false
	UseBottomChannel   = true
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = false
	UseBackChannel    = false	
	CreateDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation)
	
elseif(func_CurrentLens == LensCrop_VDome) then
	Vec_ProjectorLookVec   = Vector(0,0,-1)
	Vec_ProjectorUpVec     = Vector(0,1,0)
	DomeOrientation = Vector(0,0,-1)
	BaseViewDirection = Vector(0,0,-1)
	BaseHeadsUp       = Vector(0,1,0)

	LeftFovY=90
	RightFovY=90
	TopFovY=90
	BotFovY=90 -- not used
	FrontFovY=90-- not used
	BackFovY=90-- not used
	

	UseTopChannel	   = true
	UseBottomChannel   = false
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = false
	UseBackChannel    = false	
	CreateDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation)

elseif(func_CurrentLens == LensCrop_FullDome) then
	Vec_ProjectorLookVec   = Vector(0,1,0)
	Vec_ProjectorUpVec     = Vector(0,0,1)
	DomeOrientation = Vector(0,1,0)
	BaseViewDirection = Vector(0,0,-1)
	BaseHeadsUp       = Vector(0,1,0)

	LeftFovY=90
	RightFovY=90
	TopFovY=90 -- not used
	BotFovY=90 -- not used
	FrontFovY=90
	BackFovY=90
	

	UseTopChannel	   = true
	UseBottomChannel   = false
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = true
	UseBackChannel    = true
	CreateDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation)
else


	Vec_ProjectorLookVec   = Vector(0,0,-1)
	Vec_ProjectorUpVec     = Vector(0,1,0)
	DomeOrientation = Vector(0,0,-1)
	BaseViewDirection = Vector(0,0,-1)
	BaseHeadsUp       = Vector(0,1,0)

	LeftFovY=90
	RightFovY=90
	TopFovY=90 
	BotFovY=90 
	FrontFovY=90
	BackFovY=90
	

	UseTopChannel	   = true
	UseBottomChannel   = true
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = true
	UseBackChannel    = true	
	
	--(TODO) try createing walls or a cylinder for this instead...
	CreateDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation)
end


----------------------------------------------------------------------------

SaveShaderConfig("OmniMapConfig/shaderconfig.txt",BtoI(UseTopChannel)+BtoI(UseBottomChannel)+BtoI(UseLeftChannel)+BtoI(UseRightChannel)+BtoI(UseBackChannel)+BtoI(UseFrontChannel))

--(TODO) Load shader here....


SetProjector(Point_ProjectorPostion,Vec_ProjectorLookVec,Vec_ProjectorUpVec)