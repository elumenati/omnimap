Projector = ClassProjector:newdefaults()
Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,-1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,-1)
func_OpticalOffsetAxis = LensCrop_VDome

AudiencePos			    = Vector( 0.00 , 0,0.00) 

DomeCenter 	= Vector(0,0,0)
DomeOrientation   = Vector(0,-1,0)
BaseViewDirection = Vector(0,0,-1)
BaseHeadsUp       = Vector(0,1,0)

createadome      	= true
DomeTesselation = 100
usestencilmask 		= false
-- StencilMask_filename 	= "OmniMapConfig/maskpanodome.bmp"


leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
--topChannel = ClassRenderChannel:new("Top")
bottomChannel = ClassRenderChannel:new("Bottom")
--frontChannel = ClassRenderChannel:new("Front")
backChannel = ClassRenderChannel:new("Back")

--frontChannel:SetCameraInfo (90,1,.1,1000)
backChannel:SetCameraInfo (90,1,.1,1000)
bottomChannel:SetCameraInfo (120,1,.1,1000)
--- symetric
--leftChannel:SetCameraInfo (90,1,.1,1000)
--rightChannel:SetCameraInfo (90,1,.1,1000)
--- asymetric
leftChannel:SetCameraInfoAsymm(45, 45, 45,80,  .1,1000)
rightChannel:SetCameraInfoAsymm(45, 60,45, 55,.1,1000)



