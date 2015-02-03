

function CreateDome(name, vec_center,  vec_orientation,Radius,Tessel)
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

