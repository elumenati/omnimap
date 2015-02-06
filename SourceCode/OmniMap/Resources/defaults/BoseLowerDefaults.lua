--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation

Projector = ClassProjector:newdefaults()
Projector.Point_ProjectorPostion = Vector(0, -0.5400,-0.1040)
--Projector.Point_ProjectorPostion = Vector(0, 0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,0,-1)
Projector.Vec_ProjectorUpVec     = Vector(0,1,0)

--func_OpticalOffsetAxis = LensCrop_PDome
func_OpticalOffsetAxis = LensCrop_OmniConfig

OC_offsetX = 0.0
OC_offsetY = -0.2423
OC_scaling = 0.0

AudiencePos			    = Vector(0.0 ,-0.20, 0.0) 

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,1,0)

BaseViewDirection = Vector(0,-1,0)
BaseHeadsUp       = Vector(0,0,-1)

createapanorama = true
numRings = 5
numSides = 5
ringRadius = 1.0
torusRadius = 1.0
--ringStartAngle = -30.0
--ringEndAngle = 210.0
ringStartAngle = 0.0
ringEndAngle = 180.0
sideStartAngle = -36.92
sideEndAngle = 89.99
needsrecompile = true

ProjectorFOV = 192.2
createapartialdome = false
numRings = 25
numSides = 50
DomeRadius_unitless = 1
DomeTesselation = 100

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")

leftChannel:SetCameraInfoAsymm(59, 45, 45,59,  .1,1000)
rightChannel:SetCameraInfoAsymm(45, 59,45, 59,.1,1000)
topChannel:SetCameraInfoAsymm(47, 47,45, 45,.1,1000)

