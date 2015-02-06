--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation

-- [X] since the dome is not perfectly round, i recomend setting the raidus to be 7.5 feet rather then 6 feet.  that way it should stay pinned to the top...?
GLOBAL_domeRadius = FeetToMeters(7.5)  -- use the feet to meters conversion routine so we can reset radius
SetDomeRadius(GLOBAL_domeRadius)

-- to do
-- [ ] then set the exact parameters to get the shape of the dome.
-- [x] also put the audience height in here. I dont know if they are standing or sitting....  57 is average human standing level...
local audienceheight_from_floor = 57 * inches -- or you could put in - inches from projector to eyeballs

Projector = ClassProjector:newdefaults()
Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,-1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,-1)

func_OpticalOffsetAxis = LensCrop_VDome

AudiencePos			    = Vector(0.0 , -1 + audienceheight_from_floor , 0.0)  -- or you could put in - inches from projector to eyeballs

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,-1,0)

BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)

createapanorama = true
numRings = 100
numSides = 100
ringRadius = 2.0
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
