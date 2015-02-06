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


DomeTesselation = 100


-- Define the globals here
-- these will get set in the dome ai...
--------------------------------------
-- -- Render Channels
--------------------------------------
channelResX = 1024
channelResY = 1024
usefbo = true
--renderTarget = RT_FRAME_BUFFER_OBJECT
nearclip = .1
farclip = 1000
ConsolidateCameraIntoProjectionMatrix = true
UseOmniMapMatrix = true
multiSampleQuality = 0.0
Renderer = "D3D"

R = 1
ZNear = nearclip
ZFar  = farclip 

UseTopChannel	   = true
UseBottomChannel   = true
UseLeftChannel     = true
UseRightChannel    = true
UseFrontChannel    = true
UseBackChannel     = true

	LeftFovUp = 45
	LeftFovDown = 45
	LeftFovLeft = 45
	LeftFovRight = 45
	
	RightFovUp = 45
	RightFovDown = 45
	RightFovLeft = 45
	RightFovRight = 45
	
	BottomFovUp = 45
	BottomFovDown = 45
	BottomFovLeft = 45
	BottomFovRight = 45
	
	TopFovUp = 45
	TopFovDown = 45
	TopFovLeft = 45
	TopFovRight = 45
	
	FrontFovUp = 45
	FrontFovDown = 45
	FrontFovLeft = 45
	FrontFovRight = 45
	
	BackFovUp = 45
	BackFovDown = 45
	BackFovLeft = 45
	BackFovRight = 45
	

Vec_ProjectorLookVec   =  Vector(0,1,0  ) 
Vec_ProjectorUpVec     =  Vector(0,0,1  )
BaseViewDirection      =  Vector(0,0,-1 )
BaseHeadsUp            =  Vector(0,1,0  )  


-- no need to keep this global...???
DomeCenter 	= Vector(0,0,0)
DomeOrientation = Vector(0,1,0)









--  switch final tilt to full dome ai
--  AI needs to do the following
--  DomeOrientation
--  Base View Direction
--  Base headsup Direction
-- projector view
-- projector up
-- chan top
-- chan bot

createadome     = false
createacylinder = false
usestencilmask  = false

if(ScreenshapeDefaults == "HorizontalDome")  then
	Point_ProjectorPostion = Vector(0,0,0)
	func_OpticalOffsetAxis = LensCrop_HDome
	AudiencePos			    = Vector( 0.00 , 0.00 ,0.00) 

	Vec_ProjectorLookVec   = Vector(0,1,0)
	Vec_ProjectorUpVec     = Vector(0,0,1)
	DomeOrientation = Vector(0,1,0)
	BaseViewDirection = Vector(0,1,0)
	BaseHeadsUp       = Vector(0,0,1)

	
	LeftFovUp = 45
	LeftFovDown = 45
	LeftFovLeft = 45
	LeftFovRight = 45
	
	RightFovUp = 45
	RightFovDown = 45
	RightFovLeft = 45
	RightFovRight = 45
	
	BottomFovUp = 45
	BottomFovDown = 45
	BottomFovLeft = 45
	BottomFovRight = 45
	
	UseTopChannel	   = false
	UseBottomChannel   = true
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = false
	UseBackChannel    = false
		
	createadome     = true
	
elseif(ScreenshapeDefaults == "VerticalDome")  then
	
	Point_ProjectorPostion = Vector(0,0,0)
	func_OpticalOffsetAxis = LensCrop_VDome
	AudiencePos			    = Vector( 0.00 , 0.00 ,0.00) 
	Vec_ProjectorLookVec   = Vector(0,0,-1)
	Vec_ProjectorUpVec     = Vector(0,1,0)
	DomeOrientation = Vector(0,0,-1)

	local tilt = 0
	BaseViewDirection = RotateVec(Vector(0,0,-1),Vector(1,0,0),tilt)
	BaseHeadsUp       = RotateVec(Vector(0,1,0),Vector(1,0,0),tilt)
	
	LeftFovUp = 45
	LeftFovDown = 45
	LeftFovLeft = 45
	LeftFovRight = 45
	
	RightFovUp = 45
	RightFovDown = 45
	RightFovLeft = 45
	RightFovRight = 45
	
	TopFovUp = 45
	TopFovDown = 45
	TopFovLeft = 45
	TopFovRight = 45
	
	
	UseTopChannel	   = true
	UseBottomChannel   = false
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = false
	UseBackChannel    = false	
	
	createadome     = true
	
elseif(ScreenshapeDefaults == "PanoDome")  then

	Point_ProjectorPostion = Vector(0,0.0,0)
	func_OpticalOffsetAxis = LensCrop_VDome
	AudiencePos			    = Vector( 0.00 , 0.0 ,0.00) 

	func_OpticalOffsetAxis = LensCrop_VDome
	Point_ProjectorPostion = Vector(0, 1 ,0)
	AudiencePos			    = Vector( 0.00 , .5 ,0.00) 

	DomeCenter 	= Vector(0,1,0)
	DomeOrientation = Vector(0,-1,0)

	Vec_ProjectorLookVec   =Vector(0,-1,0)
	Vec_ProjectorUpVec     =Vector(0,0,-1)

	BaseViewDirection = Vector(0,0,-1)
	BaseHeadsUp       = Vector(0,1,0)

	local tilt = 45
	BaseViewDirection = RotateVec(BaseViewDirection,Vector(0,1,0),tilt)
	BaseHeadsUp       = RotateVec(BaseHeadsUp,Vector(0,1,0),tilt)
	tilt = -24
	BaseViewDirection = RotateVec(BaseViewDirection,Vector(1,0,0),tilt)
	BaseHeadsUp       = RotateVec(BaseHeadsUp,Vector(1,0,0),tilt)

	LeftFovUp = 55
	LeftFovDown = 55
	LeftFovLeft = 55
	LeftFovRight = 55
	
	RightFovUp = 55
	RightFovDown = 55
	RightFovLeft = 55
	RightFovRight = 55

	FrontFovUp = 55
	FrontFovDown = 55
	FrontFovLeft = 55
	FrontFovRight = 55
	
	UseTopChannel	   = false
	UseBottomChannel   = false
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = true
	UseBackChannel    = false
	
	createadome     = true

elseif(ScreenshapeDefaults == "Cylinder")  then

	Point_ProjectorPostion = Vector(0,1,0)
	func_OpticalOffsetAxis = LensCrop_HDome
	AudiencePos			    = Vector( 0.00 , 0.500 ,0.00) 


	cylinder_radius = DomeRadius_unitless
	cylinder_height = DomeRadius_unitless

	Vec_ProjectorLookVec   = Vector(0,0,-1)
	Vec_ProjectorUpVec     = Vector(0,1,0)
	
	DomeOrientation = Vector(0,0,-1)
	BaseViewDirection = Vector(0,0,-1)
	BaseHeadsUp       = Vector(0,1,0)

	vec_up = Vector(0,1,0)
	vec_forward = Vector(0,0,-1)

	LeftFovUp = 45
	LeftFovDown = 45
	LeftFovLeft = 45
	LeftFovRight = 45
	
	RightFovUp = 45
	RightFovDown = 45
	RightFovLeft = 45
	RightFovRight = 45
	
	TopFovUp = 45
	TopFovDown = 45
	TopFovLeft = 45
	TopFovRight = 45
	
	UseTopChannel	   = true
	UseBottomChannel   = false
	UseLeftChannel     = true
	UseRightChannel    = true
	UseFrontChannel   = false
	UseBackChannel    = false	

	theta_min = -3.14/2.0
	theta_max = 3.14/2.0

	createacylinder = true
	ScreenRed = 1
	ScreenGreen = 1
	ScreenBlue = 1
	
elseif(ScreenshapeDefaults == "Custom")  then

	Point_ProjectorPostion = Vector(0,0,0)
	func_OpticalOffsetAxis = LensCrop_FullDome
	AudiencePos			    = Vector( 0.00 , 0.00 ,0.00) 

	Vec_ProjectorLookVec   = Vector(0,1,0)
	Vec_ProjectorUpVec     = Vector(0,0,1)
	DomeOrientation = Vector(0,1,0)
	BaseViewDirection = Vector(0,1,0)
	BaseHeadsUp       = Vector(0,0,1)
	
	LeftFovUp = 45
	LeftFovDown = 45
	LeftFovLeft = 45
	LeftFovRight = 45
	
	RightFovUp = 45
	RightFovDown = 45
	RightFovLeft = 45
	RightFovRight = 45
	
	BottomFovUp = 45
	BottomFovDown = 45
	BottomFovLeft = 45
	BottomFovRight = 45
	
	TopFovUp = 45
	TopFovDown = 45
	TopFovLeft = 45
	TopFovRight = 45
	
	FrontFovUp = 45
	FrontFovDown = 45
	FrontFovLeft = 45
	FrontFovRight = 45
	
	BackFovUp = 45
	BackFovDown = 45
	BackFovLeft = 45
	BackFovRight = 45
	
	UseTopChannel	   = false
	UseBottomChannel   = false
	UseLeftChannel     = false
	UseRightChannel    = false
	UseFrontChannel   = true
	UseBackChannel    = true	
	
	createadome     = true
end



----------------------------------------------------------------------------
