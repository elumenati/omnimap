---%%% Check if we are using col major consistently...
---%%% Check if we are using col major consistently...



--------------------------------
--(%$$$%) Matrix Type
--------------------------------

function isMatrix3x3(M)
return (
	(m["type"] =="Matrix") and
	(m["DimX"] == 3) and
	(m["DimY"] == 3)
	)

end
--------------------------------
--(%$$$%) Matrix Creation
--------------------------------

--(%%) Matrix3x3 Defination
--(%%) create a matrix from elements
function Matrix3x3(e11,e12,e13,e21,e22,e23,e31,e32,e33)
	local m = {}
	m["type"] ="Matrix"
	m["DimX"] = 3
	m["DimY"] = 3
	m["e11"] = e11
	m["e12"] = e12
	m["e13"] = e13
	m["e21"] = e21
	m["e22"] = e22
	m["e23"] = e23
	m["e31"] = e31
	m["e32"] = e32
	m["e33"] = e33
	return m
end


--(%%) copy a matrix 
--(%%) matB =  matA   -- pointer copy
--(%%) matB = VectorCopy(matA)   -- data copy
function Matrix3x3_CopyMatrix(M)
	return Matrix3x3(
	m["e11"],
	m["e12"],
	m["e13"],
	
	m["e21"],
	m["e22"],
	m["e23"],
	
	m["e31"],
	m["e32"],
	m["e33"]
	)
end


--(%%) create A matrix from Row Vectors
function Matrix3x3_fromRowVecs(R1,R2,R3)
	return Matrix3x3(
	R1["x"],R1["y"],R1["z"],
	R2["x"],R2["y"],R2["z"],
	R3["x"],R3["y"],R3["z"]
	)
end


--(%%) Create Matrix from ColVecs
function Matrix3x3_fromColVecs(C1,C2,C3)
	return Matrix3x3_transpose(Matrix3x3_fromRowVecs(C1,C2,C3))
end
--(%%) Create Zero Matrix
function Matrix3x3_Zero()
	return Matrix3x3(0,0,0, 0,0,0, 0,0,0)
end
--(%%) Create Identity
function Matrix3x3_Identity()
	return Matrix3x3(1,0,0, 0,1,0, 0,0,1)
end

--(%%) Create A Rotation Matrix
--(%%) Not thouroughly tested
--(%%) watch out to make sure that it rotates in the right direction....
function RotateMatrix3x3( Axis, angleDegrees)	
	local angleRadians =  angleDegrees  /57.2957795
	-- Create Orthonormal Basis from Vector Axis
	local W = Vector_Normalize(Axis)
	local t = VectorCopy(W)
	-- set smallest component of t to 1 so that t is not colinnear to w
	if(math.abs (t["x"]) < math.abs (t["y"])) then
		if(math.abs (t["x"]) < math.abs (t["z"])) then
			t["x"] = 1
		else
			t["z"] = 1
		end
	else
		if(math.abs (t["y"] )< math.abs (t["z"])) then
			t["y"] = 1
		else
			t["z"] = 1
		end
	end
	
	local U  = Vector_Normalize(Vector_Cross(t,W))
	local V  = Vector_Cross(W,U)
	local M  = Matrix3x3_fromColVecs(U,V,W)
	local Mt = Matrix3x3_fromRowVecs(U,V,W)
	local Mr = Matrix3x3_Identity()
	
	Mr["e11"] =  math.cos( angleRadians )
	Mr["e12"] = -math.sin( angleRadians )
	Mr["e21"] =  math.sin( angleRadians )
	Mr["e22"] =  math.cos( angleRadians )	
	return Matrix3x3_MxM(M, Matrix3x3_MxM(Mr,Mt))
end


--------------------------------
--(%$$$%) Matrix Operators
--------------------------------


--(%%) transpose
function Matrix3x3_transpose(m)
	return Matrix3x3(
	m["e11"],
	m["e21"],
	m["e31"],
	
	m["e12"],
	m["e22"],
	m["e32"],
	
	m["e13"],
	m["e23"],
	m["e33"]
	)
end

--(%%) Vector Multiply
function Matrix3x3_MxV(m,V)
	return Vector(
	m["e11"]*V["x"]+
	m["e12"]*V["y"]+
	m["e13"]*V["z"],
	
	m["e21"]*V["x"]+
	m["e22"]*V["y"]+
	m["e23"]*V["z"],
	
	m["e31"]*V["x"]+
	m["e32"]*V["y"]+
	m["e33"]*V["z"]
	)
end


--(%%) Matrix Multiply
function Matrix3x3_MxM(m,m2)
	return Matrix3x3(
	m["e11"]*m2["e11"]+  --11
	m["e12"]*m2["e21"]+
	m["e13"]*m2["e31"],
	
	m["e11"]*m2["e12"]+  --12
	m["e12"]*m2["e22"]+
	m["e13"]*m2["e32"],
	
	m["e11"]*m2["e13"]+  --13
	m["e12"]*m2["e23"]+
	m["e13"]*m2["e33"],
	
	m["e21"]*m2["e11"]+  --21
	m["e22"]*m2["e21"]+
	m["e23"]*m2["e31"],	
	
	m["e21"]*m2["e12"]+  --22
	m["e22"]*m2["e22"]+
	m["e23"]*m2["e32"],	
	
	m["e21"]*m2["e13"]+  --23
	m["e22"]*m2["e23"]+ 
	m["e23"]*m2["e33"],		
	
	m["e31"]*m2["e11"]+  --31
	m["e32"]*m2["e21"]+
	m["e33"]*m2["e31"],		

	m["e31"]*m2["e12"]+  --32
	m["e32"]*m2["e22"]+
	m["e33"]*m2["e32"],		
	
	m["e31"]*m2["e13"]+  --33
	m["e32"]*m2["e23"]+
	m["e33"]*m2["e33"])
end


