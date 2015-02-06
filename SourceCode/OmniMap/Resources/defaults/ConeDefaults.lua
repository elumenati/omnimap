Projector = ClassProjector:newdefaults()

Projector.Point_ProjectorPostion = Vector(0,0,0)
Projector.Vec_ProjectorLookVec   = Vector(0,1,0)
Projector.Vec_ProjectorUpVec     = Vector(0,0,1)

func_OpticalOffsetAxis = LensCrop_PDome

AudiencePos			    = Vector(0.00, 0.0 ,0.00) 

BaseViewDirection = Vector(0,1,0)
BaseHeadsUp       = Vector(0,0,1)

createadome      	= false


TopRadius = .8844
BottomRadius = 1.0
ConeHeight = .6


SweepAngleStart = -1
SweepAngleEnd = 181
NumStacks = 60


createacone = true
ProjectorFOV = 186





leftChannel   = ClassRenderChannel:new("Left")
rightChannel  = ClassRenderChannel:new("Right")
topChannel  = ClassRenderChannel:new("Top")

-- if we want to maximize our channel res...
leftChannel:SetCameraInfoAsymm(45, 45, 55, 45, .1,1000)
rightChannel:SetCameraInfoAsymm(45, 45, 55, 45,.1,1000)
topChannel:SetCameraInfoAsymm(45, 45, 45, 45,.1,1000)






