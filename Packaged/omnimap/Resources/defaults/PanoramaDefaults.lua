--    render channels to use
--    fov for each render channel
--    dome oritiation
--    audience orientation
--    projector orientation

-- Screen Aspect Settings -- 
-- 0 = 4x3
-- 1 = 16x9
-- 2 = 16x10

AspectRatio = 2

-- Lens Offset Settings
function LensCrop_Panorama()
  if(AspectRatio == 0) then
	SetglViewportsettings(0, -1.0/8.0, 0)
  end
  if(AspectRatio == 1) then
	SetglViewportsettings(0, -7.0/32.0, 0)
  end
  if(AspectRatio == 2) then
	SetglViewportsettings(0, -3.0/16.0, 0)
  end
end

func_OpticalOffsetAxis = LensCrop_Panorama



BaseViewDirection = Vector(0,-1,0)
BaseHeadsUp       = Vector(0,0,-1)


-- Panorama V1 = 0
-- Panorama v2 = 1

PanoVersion = 1


if (PanoVersion == 0) then
	Projector = ClassProjector:newdefaults()
	Projector.Point_ProjectorPostion = Vector(0,0,0)
	Projector.Vec_ProjectorLookVec   = Vector(0,-1,0)
	Projector.Vec_ProjectorUpVec     = Vector(0,0,-1)

	DomeCenter 	= Vector(0,0,0)
	DomeOrientation   = Vector(0,-1,0)

	AudiencePos  = Vector(0.0, -0.20, 0.0) 

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
end

if (PanoVersion == 1) then
	Projector = ClassProjector:newdefaults()
	Projector.Point_ProjectorPostion = Vector(0,0.0,0.0)
	Projector.Vec_ProjectorLookVec   = Vector(0,-1,0)
	Projector.Vec_ProjectorUpVec     = Vector(0,0,-1)

	DomeCenter 	= Vector(0,-.64,0)
	DomeOrientation   = Vector(0,-1,0)

	AudiencePos  = Vector(0.0, -0.20, 0.0) 

	createapanorama = true
	numRings = 100
	numSides = 100
	ringRadius = 1.0
	torusRadius = 1.0
	ringStartAngle = -15.0
	ringEndAngle = 195.0
	sideStartAngle = 0.0
	sideEndAngle = 60.0
	needsrecompile = true
end

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")

leftChannel:SetCameraInfoAsymm(59, 46, 46,59,  .1,1000)
rightChannel:SetCameraInfoAsymm(46, 59,46, 59,.1,1000)
topChannel:SetCameraInfoAsymm(47, 47,46, 46,.1,1000)
