--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation
Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,0,-0.15)
Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)

func_OpticalOffsetAxis = LensCrop_HDome

AudiencePos			    = Vector( 0.00 , 0.15 ,0.15) 

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,1,0)

BaseViewDirection = Vector(0,1,0)
BaseHeadsUp       = Vector(0,0,1)

createadome       = true
DomeTesselation = 100

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
bottomChannel = ClassRenderChannel:new("Bottom")

bottomChannel:SetCameraInfo (108,1,.1,1000)
leftChannel:SetCameraInfoAsymm(57, 45, 45,65,  .1,1000)
rightChannel:SetCameraInfoAsymm(45, 57,45, 60,.1,1000)
