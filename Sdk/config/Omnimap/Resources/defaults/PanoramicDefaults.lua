--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation

Projector = ClassProjector:newdefaults()
Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,-1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,-1)

func_OpticalOffsetAxis = LensCrop_PDome

AudiencePos			    = Vector(0.0 ,-0.20, 0.0) 

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,-1,0)

BaseViewDirection = Vector(0,-1,0)
BaseHeadsUp       = Vector(0,0,-1)

createapanorama = true
numRings = 100
numSides = 100
ringRadius = 1.25
torusRadius = 1.0
ringStartAngle = -30.0
ringEndAngle = 210.0
sideStartAngle = 0.0
sideEndAngle = 70.0
needsrecompile = true

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")

leftChannel:SetCameraInfoAsymm(59, 45, 45,59,  .1,1000)
rightChannel:SetCameraInfoAsymm(45, 59,45, 59,.1,1000)
topChannel:SetCameraInfoAsymm(47, 47,45, 45,.1,1000)
