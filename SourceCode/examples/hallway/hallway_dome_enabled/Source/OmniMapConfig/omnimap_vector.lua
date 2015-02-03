--------------------------------
--(%$$$%) Vector Type
--------------------------------


--(%%) Vector Defination
--(%%) Vector is a three dimensional Vector
function Vector(x,y,z)
	local v = {}
	v["type"] ="Vector"
	v["x"] =x
	v["y"] =y
	v["z"] =z
	return v
end


--(%%) Vector Copy
--(%%) vecB =  vecA   -- pointer copy
--(%%) vecB = VectorCopy(vecA)   -- data copy
function VectorCopy(V)
	return Vector( V["x"],V["y"],V["z"] )
end


--------------------------------
--(%$$$%) Vector Identities
--------------------------------
--(%%) Vector Identity Y
function VectorY()	return Vector(0,1,0) end
--(%%) Vector Identity X
function VectorX() 	return Vector(1,0,0) end
--(%%) Vector Identity Z
function VectorZ() 	return Vector(0,0,1) end


--------------------------------
--(%$$$%) Vector Math Opterators
--------------------------------

function Vector_Add(a,b) 
	return Vector(a["x"]+b["x"],a["y"]+b["y"],a["z"]+b["z"])
end

function Vector_Subtract(a,b) 
	return Vector(a["x"]-b["x"],a["y"]-b["y"],a["z"]-b["z"])
end

function Vector_MulPerElement(a,b) 
	return Vector(a["x"]*b["x"],a["y"]*b["y"],a["z"]*b["z"])
end

function Vector_VecXScalar(a,b) 
	return Vector(
	a["x"]*b,
	a["y"]*b,
	a["z"]*b)
end

function Vector_Dot(a,b) 
	return a["x"]*b["x"]+a["y"]*b["y"]+a["z"]*b["z"]
end

function Vector_Cross(a,b) 
	return Vector(a["y"]*b["z"] - a["z"]*b["y"], a["z"]*b["x"] - a["x"]*b["z"], a["x"]*b["y"] - a["y"]*b["x"])
end

function Vector_Length(a) 
	return math.sqrt(a["x"]*a["x"]+a["y"]*a["y"]+a["z"]*a["z"])
end

function Vector_Normalize(a) 
local len = Vector_Length(a)
	return Vector_VecXScalar(a,1.0/len)
end

function Vector_Reverse(a) 
   return Vector_VecXScalar(a,-1.0)
end

--(%%) rotate vector Vec by angleDegrees around Axis 
function RotateVec( Vec,Axis, angleDegrees)
	local Mtr = RotateMatrix3x3(Axis, angleDegrees)
	local rotatedvec = Matrix3x3_MxV(Mtr,Vec)
	return  rotatedvec 	    
end


--(%%) Print to OmnimapMessages.txt
function PrintVector( vec, name)
	ConsolePrintString( name .." ".. vec["x"] .." , ".. vec["y"] .." , ".. vec["z"])
end

