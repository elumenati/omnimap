--    render channels to use
--    fov for each render channel
--    dome orientiation
--    audience orientation
--    projector orientation
Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,0,-1)
Projector.Vec_ProjectorUpVec     = Vector(0,1,0)

func_OpticalOffsetAxis = LensCrop_VDome

AudiencePos			    = Vector( 0.00 , 0.20 ,0.10) 

DomeCenter 	= Vector(0,0,0)

DomeOrientation   = Vector(0,0,-1)
BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)

createadome       = true
DomeTesselation = 50

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")


topChannel:SetCameraInfo (90,1,.1,1000)
leftChannel:SetCameraInfoAsymm(45, 45, 60, 45, .1,1000)
rightChannel:SetCameraInfoAsymm(45, 45, 60, 45,.1,1000)
