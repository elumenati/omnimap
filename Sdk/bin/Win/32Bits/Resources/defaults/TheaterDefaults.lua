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
function LensCrop_Theater()
  if(AspectRatio == 0) then
	SetglViewportsettings(0, 1.0/8.0, 0)
  end
  if(AspectRatio == 1) then
	SetglViewportsettings(0, 7.0/32.0, 0)
  end
  if(AspectRatio == 2) then
	SetglViewportsettings(0, 3.0/16.0, 0)
  end
end

func_OpticalOffsetAxis = LensCrop_Theater


Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,0,-0.15)
Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)


-- GeoDome Theater Audience
AudiencePos	 = Vector( 0.00 , 0.15 ,0.15) 

-- GeoDome Evolver (Planetarium) Audience
--AudiencePos	 = Vector( 0.00 , -0.1 ,0.0) 



DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,1,0)

BaseViewDirection = Vector(0,1,0)
BaseHeadsUp       = Vector(0,0,1)

createadome       = true
DomeTesselation = 100

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
bottomChannel = ClassRenderChannel:new("Bottom")

bottomChannel:SetCameraInfo (100, 1, .1, 2000)
leftChannel:SetCameraInfoAsymm(60, 46, 50, 60, 0.1, 2000)
rightChannel:SetCameraInfoAsymm(46, 60, 50, 60, 0.1, 2000)

