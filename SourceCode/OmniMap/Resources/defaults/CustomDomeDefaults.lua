--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation
Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)

func_OpticalOffsetAxis = LensCrop_FullDome

AudiencePos			    = Vector( 0.00 , 0.00 ,0.00) 

DomeCenter 	= Vector(0,0,0)

DomeOrientation   = Vector(0,1,0)
BaseViewDirection = Vector(0,1,0)
BaseHeadsUp       = Vector(0,0,1)

createadome       = true
DomeTesselation = 100

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
bottomChannel = ClassRenderChannel:new("Bottom")
topChannel = ClassRenderChannel:new("Top")

--- symetric
bottomChannel:SetCameraInfo (90,1,.1,1000)
topChannel:SetCameraInfo (90,1,.1,1000)
leftChannel:SetCameraInfo (90,1,.1,1000)
rightChannel:SetCameraInfo (90,1,.1,1000)