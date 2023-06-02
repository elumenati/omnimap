
-- Screen Aspect Settings -- 
-- 0 = 4x3
-- 1 = 16x9
-- 2 = 16x10
-- 3 = 1x1




------------------------------------------------
--------------------- LENS ---------------------
-- Lens Offset Settings
AspectRatio = 2

------------------------------------------------
------------------ PROJECTOR -------------------
Projector = ClassProjector:newdefaults()
Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)

if(AspectRatio == 0) then
	Projector.Point_ProjectorPostion = Vector(0.,-.800,-.500)
end
if(AspectRatio == 1) then
	Projector.Point_ProjectorPostion = Vector(0,-.800,-.218)
end
if(AspectRatio == 2) then
	Projector.Point_ProjectorPostion = Vector(0,-.800,-.250)
end
if(AspectRatio == 3) then
	Projector.Point_ProjectorPostion = Vector(0,-.800,-.25)
end

-- Lens Offset Settings
function LensCrop_Slammed()
  if(AspectRatio == 0) then
    SetglViewportsettings(0, 1.0/8.0, 0)
  end
  if(AspectRatio == 1) then
    SetglViewportsettings(0, 7.0/32.0, 0)
  end
  if(AspectRatio == 2) then
    SetglViewportsettings(0, 3.0/16.0, 0)
  end
  if(AspectRatio == 3) then
    SetglViewportsettings(0, 0, 1)
  end
end

func_OpticalOffsetAxis = LensCrop_Slammed

------------------------------------------------
--------------------- DOME ---------------------
DomeOrientation   = Vector(0,1,0)
DomeCenter 	= Vector(0,-.3,0) -- DOME CENTER DOES NOT WORK FOR PANO!!!!  You must leave dome center at 0,0,0 and make other numbers relative to dome center


createapanorama = true
needsrecompile = true
numRings = 100
numSides = 100
ringRadius = 1.0
torusRadius = 1.0

sideEndAngle = 90.0 -- apex
sideStartAngle = -30.0 -- bottom ring
ringStartAngle = 0 -- horizontal sweep starting west traveling forward
ringEndAngle = 180 -- horizontal sweep Ending east


------------------------------------------------
--------------------- VIEW ---------------------
AudiencePos	 = Vector( 0.00 , 0.0 ,0.00)  -- Use Dome center as 0,0,0 and offset audience position
BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)
BaseHeadsRight       = Vector(1,0,0)

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")

leftChannel:SetCameraInfoAsymm(45, 46, 60, 45, .1,1000)
rightChannel:SetCameraInfoAsymm(46, 45, 60, 45,.1,1000)
topChannel:SetCameraInfo (90,1,.1,1000)
--------------------------------------------------------











