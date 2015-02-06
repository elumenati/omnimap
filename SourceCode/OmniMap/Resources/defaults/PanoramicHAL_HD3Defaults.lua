--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation

local audienceheight_from_floor = 72 * inches -- or you could put in - inches from projector to eyeballs

Projector = ClassProjector:newdefaults()
--Projector.Point_ProjectorPostion = Vector(0, -8.0 *inches , 0)  
Projector.Point_ProjectorPostion = Vector(0, 0.0, 0)  
Projector.Vec_ProjectorLookVec   = Vector(0,-1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,-1)

func_OpticalOffsetAxis = LensCrop_PDome

--AudiencePos			    = Vector(0.0 , -1 + audienceheight_from_floor , 0.0)  -- or you could put in - inches from projector to eyeballs
AudiencePos			    = Vector(0.0 , 0.0 , 0.0)

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,-1,0)

BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)

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
bottomChannel = ClassRenderChannel:new("Bottom")

leftChannel:SetCameraInfoAsymm(60, 45, 45,45,  .1,1000)
rightChannel:SetCameraInfoAsymm(45, 60,45, 45,.1,1000)
bottomChannel:SetCameraInfoAsymm(63, 45,63, 45,.1,1000)
