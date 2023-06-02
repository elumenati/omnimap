

function CreateCone(name, bottomRadius, topRadius, coneHeight, 
		sweepAngleStart, sweepAngleEnd, numStacks)
	ConsolePrintString("CreateCone")
	Create_ScreenShape(name, "cone")
	SetScreenParameter(name,"BottomRadius", bottomRadius)
	SetScreenParameter(name,"TopRadius", topRadius)
	SetScreenParameter(name,"Height", coneHeight)
	SetScreenParameter(name,"SweepAngleStart", sweepAngleStart)
	SetScreenParameter(name,"SweepAngleEnd", sweepAngleEnd)
	SetScreenParameter(name,"NumStacks", numStacks)		
end

function CreateDome(name, vec_center,  vec_orientation,Radius,Tessel)
	ConsolePrintString("CreateDome")
	Create_ScreenShapeDome(name)
	SetScreenParameter(name,"Dome_Radius",Radius)
	SetScreenParameter(name,"Dome_tesselation",Tessel)
	SetScreenParameter(name,"Dome_Center.x",vec_center["x"])
	SetScreenParameter(name,"Dome_Center.y",vec_center["y"])
	SetScreenParameter(name,"Dome_Center.z",vec_center["z"])
	SetScreenParameter(name,"Dome_ApexDirection.x",vec_orientation["x"])
	SetScreenParameter(name,"Dome_ApexDirection.y",vec_orientation["y"])
	SetScreenParameter(name,"Dome_ApexDirection.z",vec_orientation["z"])
	SetScreenParameter(name,"needsrecompile","true")
end

function CreatePartialDome(name, vec_center,  vec_orientation,Radius,Tessel,NumStacks, NumRings)
	ConsolePrintString("CreatePartialDome")
	Create_ScreenShape(name, "partial_dome")
	SetScreenParameter(name,"Dome_Radius",Radius)
	SetScreenParameter(name,"Dome_tesselation",Tessel)
	SetScreenParameter(name,"Dome_Center.x",vec_center["x"])
	SetScreenParameter(name,"Dome_Center.y",vec_center["y"])
	SetScreenParameter(name,"Dome_Center.z",vec_center["z"])
	SetScreenParameter(name,"Dome_ApexDirection.x",vec_orientation["x"])
	SetScreenParameter(name,"Dome_ApexDirection.y",vec_orientation["y"])
	SetScreenParameter(name,"Dome_ApexDirection.z",vec_orientation["z"])
	SetScreenParameter(name,"Dome_NumStacks", NumStacks);
	SetScreenParameter(name,"Dome_NumRings", NumRings);
	SetScreenParameter(name,"needsrecompile","true")
end

function CreatePanorama(name, numRings,
	numSides,
	vec_center,  vec_orientation,
	ringRadius, torusRadius,
	ringStartAngle, ringEndAngle,
	sideStartAngle, sideEndAngle)
	Create_ScreenShape(name,"toroid")
	ConsolePrintString("CreatePanoramic")
	
	SetScreenParameter(name, "NumRings", numRings)
	SetScreenParameter(name, "NumSides", numSides)
	SetScreenParameter(name, "RingRadius", ringRadius)
	SetScreenParameter(name, "TorusRadius", torusRadius)
	SetScreenParameter(name, "RingStartAngle", ringStartAngle)
	SetScreenParameter(name, "RingEndAngle", ringEndAngle)
	SetScreenParameter(name, "SideStartAngle", sideStartAngle)
	SetScreenParameter(name, "SideEndAngle", sideEndAngle)
	SetScreenParameter(name,"Dome_Center.x",vec_center["x"])
	SetScreenParameter(name,"Dome_Center.y",vec_center["y"])
	SetScreenParameter(name,"Dome_Center.z",vec_center["z"])
	SetScreenParameter(name,"Dome_ApexDirection.x",vec_orientation["x"])
	SetScreenParameter(name,"Dome_ApexDirection.y",vec_orientation["y"])
	SetScreenParameter(name,"Dome_ApexDirection.z",vec_orientation["z"])
	SetScreenParameter(name, "needsrecompile","true")

end



function CreateCylinder(
name,
radius, height, 
Vec_BottomOrigin,
Vec_Up,Vec_Foward,
theta_min,theta_max,
tess,red,green,blue)
	ConsolePrintString("CreateCylinder")
	Create_ScreenShape(name,"cylinder")
	SetScreenParameter(name,"Radius",radius)
	SetScreenParameter(name,"Height",height)  
	SetScreenParameter(name,"iTessel",tess)
	SetScreenParameter(name,"ThetaMin",theta_min)
	SetScreenParameter(name,"ThetaMax",theta_max)
	SetScreenParameter(name,"OriginBottom.x",Vec_BottomOrigin["x"])
	SetScreenParameter(name,"OriginBottom.y",Vec_BottomOrigin["y"])
	SetScreenParameter(name,"OriginBottom.z",Vec_BottomOrigin["z"])
	SetScreenParameter(name,"Up.x",Vec_Up["x"])
	SetScreenParameter(name,"Up.y",Vec_Up["y"])
	SetScreenParameter(name,"Up.z",Vec_Up["z"])
	SetScreenParameter(name,"Foward.x",Vec_Foward["x"])
	SetScreenParameter(name,"Foward.y",Vec_Foward["y"])
	SetScreenParameter(name,"Foward.z",Vec_Foward["z"])
	SetScreenParameter(name,"ColorRGB.r",red);
	SetScreenParameter(name,"ColorRGB.g",green);
	SetScreenParameter(name,"ColorRGB.b",blue);
	SetScreenParameter(name,"needsrecompile","true")

end

function CreatePlane(name, width, height, center, orientation)

	ConsolePrintString("CreatePlane")
	Create_ScreenShape(name, "plane");
	SetScreenParameter(name,"PlaneWidth",width)
	SetScreenParameter(name,"PlaneHeight",height)
	SetScreenParameter(name,"Center.x",center["x"])
	SetScreenParameter(name,"Center.y",center["y"])
	SetScreenParameter(name,"Center.z",center["z"])
	SetScreenParameter(name,"Foward.x",orientation["x"])
	SetScreenParameter(name,"Foward.y",orientation["y"])
	SetScreenParameter(name,"Foward.z",orientation["z"])
	SetScreenParameter(name,"needsrecompile","true")
end