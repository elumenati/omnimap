--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation
Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,-.500,-.500)
Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)

func_OpticalOffsetAxis = LensCrop_HDome

AudiencePos   = Vector( 0.00 , 0.0 ,0.0) 

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,0,-1)

BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)

createapartialdome       = true
DomeTesselation = 100
DomeNumStacks = 75
DomeNumRings = 99

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")

topChannel:SetCameraInfo (90,1,.1,1000)
leftChannel:SetCameraInfo(90,1,  .1,1000)
rightChannel:SetCameraInfo(90,1,  .1,1000)
