

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