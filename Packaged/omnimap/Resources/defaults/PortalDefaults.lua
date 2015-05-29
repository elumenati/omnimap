--    render channels to use
--    fov for each render channel
--    dome orientiation
--    audience orientation
--    projector orientation

-- Screen Aspect Settings -- 
-- 0 = 4x3
-- 1 = 16x9
-- 2 = 16x10

AspectRatio = 0


-- Lens Offset Settings
function LensCrop_Portal()
	if (AspectRatio == 0) then 
		SetglViewportsettings(0.0, -1.0/8.0, 0.0)
	end
	if (AspectRatio == 1) then 
		SetglViewportsettings(0.0, -7.0/32.0, 0.0)
	end
	if (AspectRatio == 2) then 
		SetglViewportsettings(0.0, -3.0/16.0, 0.0)
	end
end

func_OpticalOffsetAxis = LensCrop_Portal


Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,0,-1)
Projector.Vec_ProjectorUpVec     = Vector(0,1,0)


AudiencePos = Vector( 0.00 , 0.20 ,0.10) 

DomeCenter = Vector(0,0,0)

DomeOrientation   = Vector(0,0,-1)
BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)

createadome = true

DomeTesselation = 100

leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel = ClassRenderChannel:new("Top")


topChannel:SetCameraInfo (90,1,.1,1000)
leftChannel:SetCameraInfoAsymm(45, 46, 60, 45, .1,1000)
rightChannel:SetCameraInfoAsymm(46, 45, 60, 45,.1,1000)
