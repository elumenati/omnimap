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
function LensCrop_Slammed()
  if(AspectRatio == 0) then
	SetglViewportsettings(0, 1.0/8.0, 0)
	Projector.Point_ProjectorPostion = Vector(0.,-.500,-.500)
  end
  if(AspectRatio == 1) then
	SetglViewportsettings(0, 7.0/32.0, 0)
	Projector.Point_ProjectorPostion = Vector(0,-.500,-.218)

  end
  if(AspectRatio == 2) then
	SetglViewportsettings(0, 3.0/16.0, 0)
	Projector.Point_ProjectorPostion = Vector(0,-.500,-.250)

  end
end

func_OpticalOffsetAxis = LensCrop_Slammed

Projector = ClassProjector:newdefaults()

if(AspectRatio == 0) then
	Projector.Point_ProjectorPostion = Vector(0.,-.500,-.500)
end

if(AspectRatio == 1) then
	Projector.Point_ProjectorPostion = Vector(0,-.500,-.218)
end

if(AspectRatio == 2) then
	Projector.Point_ProjectorPostion = Vector(0,-.500,-.250)
end

--Projector.Point_ProjectorPostion = Vector(0.0,-.500,-.500)

Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)


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
