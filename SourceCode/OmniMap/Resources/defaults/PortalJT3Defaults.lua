Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0.78, 0, 0)
Projector.Vec_ProjectorLookVec   = Vector(0, 0, -1)
Projector.Vec_ProjectorUpVec = Vector(0, 1, 0)

AudiencePos = Vector(0, 0.2, 0.1)
BaseViewDirection = Vector(0, 0, -1)
BaseHeadsUp = Vector(0, 1, 0)

OC_offsetX = 0
OC_offsetY = -0.125
OC_scaling = 0
func_OpticalOffsetAxis = LensCrop_OmniConfig

ProjectorFOV = 180
-- Screen Variables--
Dome_ApexDirection.x = 0.000000
Dome_ApexDirection.y = 0.000000
Dome_ApexDirection.z = -1.000000
Dome_Center.x = 0.000000
Dome_Center.y = 0.000000
Dome_Center.z = 0.000000
needsrecompile = false
Dome_Radius = 1.000000
Dome_tesselation = 50
leftChannel = ClassRenderChannel:new("Left")
leftChannel:SetCameraInfoAsymm(45, 45, 60, 45, 0.1, 1000)

rightChannel = ClassRenderChannel:new("Right")
rightChannel:SetCameraInfoAsymm(45, 45, 60, 45, 0.1, 1000)

topChannel = ClassRenderChannel:new("Top")
topChannel:SetCameraInfoAsymm(45, 45, 45, 45, 0.1, 1000)

