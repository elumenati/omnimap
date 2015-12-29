---%%% Check if we are using col major consistently...
---%%% Check if we are using col major consistently...


--------------------------------
--(%$$$%) Matrix Type
--------------------------------

function isMatrix4x4(M)
return (
	(m["type"] =="Matrix") and
	(m["DimX"] == 4) and
	(m["DimY"] == 4)
	)

end
--------------------------------
--(%$$$%) Matrix Creation
--------------------------------

--(%%) Matrix4x4 Defination
--(%%) create a matrix from elements
function Matrix4x4(e11,e12,e13,e14,e21,e22,e23,e24,e31,e32,e33,e34,e41,e42,e43,e44)
	local m = {}
	m["type"] ="Matrix"
	m["DimX"] = 4
	m["DimY"] = 4
	m["e11"] = e11
	m["e12"] = e12
	m["e13"] = e13
	m["e14"] = e14
	
	m["e21"] = e21
	m["e22"] = e22
	m["e23"] = e23
	m["e24"] = e24
	m["e31"] = e31
	m["e32"] = e32
	m["e33"] = e33
	m["e34"] = e34

	m["e41"] = e41
	m["e42"] = e42
	m["e43"] = e43
	m["e44"] = e44
	return m
end

function Matrix3x3ToMatrix4x4(M3x3)
local m = Matrix4x4_CopyMatrix(M3x3)
	m["DimX"] = 4
	m["DimY"] = 4

	m["e14"] = 0
	m["e24"] = 0
	m["e34"] = 0
	
	m["e41"] = 0
	m["e42"] = 0
	m["e43"] = 0
	m["e44"] = 1
	return m
end

--(%%) copy a matrix 
--(%%) matB =  matA   -- pointer copy
--(%%) matB = VectorCopy(matA)   -- data copy
function Matrix4x4_CopyMatrix(m)
	return Matrix4x4(
	m["e11"],
	m["e12"],
	m["e13"],
	m["e14"],
	
	m["e21"],
	m["e22"],
	m["e23"],
	m["e24"],
	
	m["e31"],
	m["e32"],
	m["e33"],
	m["e34"],
	
	m["e41"],
	m["e42"],
	m["e43"],
	m["e44"]
	)
end


--(%%) Create Zero Matrix
function Matrix4x4_Zero()
	return Matrix4x4(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0)
end

--(%%) Create Identity
function Matrix4x4_Identity()
	return Matrix4x4(1,0,0,0, 0,1,0,0, 0,0,1,0,  0,0,0,1  )
end


--(%%) Create A Rotation Matrix
--(%%) Not thouroughly tested
--(%%) watch out to make sure that it rotates in the right direction....
function RotateMatrix4x4( Axis, angleDegrees)	
	return Matrix3x3ToMatrix4x4(RotateMatrix3x3(Axis,angleDegrees ) )
end


--------------------------------
--(%$$$%) Matrix Operators
--------------------------------


--(%%) Matrix Multiply
function Matrix4x4_MxM(m,m2)
	return Matrix4x4(
	m["e11"]*m2["e11"]+  --11
	m["e12"]*m2["e21"]+
	m["e13"]*m2["e31"]+
	m["e14"]*m2["e41"]
	,
	
	m["e11"]*m2["e12"]+  --12
	m["e12"]*m2["e22"]+
	m["e13"]*m2["e32"]+
	m["e14"]*m2["e42"]
	
	,
	
	m["e11"]*m2["e13"]+  --13
	m["e12"]*m2["e23"]+
	m["e13"]*m2["e33"]+
	m["e14"]*m2["e43"]
	,
	
	m["e11"]*m2["e14"]+  --14
	m["e12"]*m2["e24"]+
	m["e13"]*m2["e34"]+
	m["e14"]*m2["e44"]
	,


	m["e21"]*m2["e11"]+  --21
	m["e22"]*m2["e21"]+
	m["e23"]*m2["e31"]+
	m["e24"]*m2["e41"]
	,
	
	m["e21"]*m2["e12"]+  --22
	m["e22"]*m2["e22"]+
	m["e23"]*m2["e32"]+
	m["e24"]*m2["e42"]
	
	,
	
	m["e21"]*m2["e13"]+  --23
	m["e22"]*m2["e23"]+
	m["e23"]*m2["e33"]+
	m["e24"]*m2["e43"]
	,
	
	m["e21"]*m2["e14"]+  --24
	m["e22"]*m2["e24"]+
	m["e23"]*m2["e34"]+
	m["e24"]*m2["e44"]
	,

	m["e31"]*m2["e11"]+  --31
	m["e32"]*m2["e21"]+
	m["e33"]*m2["e31"]+
	m["e34"]*m2["e41"]
	,
	
	m["e31"]*m2["e12"]+  --32
	m["e32"]*m2["e22"]+
	m["e33"]*m2["e32"]+
	m["e34"]*m2["e42"]
	
	,
	
	m["e31"]*m2["e13"]+  --33
	m["e32"]*m2["e23"]+
	m["e33"]*m2["e33"]+
	m["e34"]*m2["e43"]
	,
	
	m["e31"]*m2["e14"]+  --34
	m["e32"]*m2["e24"]+
	m["e33"]*m2["e34"]+
	m["e34"]*m2["e44"]
	,

	m["e41"]*m2["e11"]+  --41
	m["e42"]*m2["e21"]+
	m["e43"]*m2["e31"]+
	m["e44"]*m2["e41"]
	,
	
	m["e41"]*m2["e12"]+  --42
	m["e42"]*m2["e22"]+
	m["e43"]*m2["e32"]+
	m["e44"]*m2["e42"]
	
	,
	
	m["e41"]*m2["e13"]+  --43
	m["e42"]*m2["e23"]+
	m["e43"]*m2["e33"]+
	m["e44"]*m2["e43"]
	,
	
	m["e41"]*m2["e14"]+  --44
	m["e42"]*m2["e24"]+
	m["e43"]*m2["e34"]+
	m["e44"]*m2["e44"]	
	)
end




--(%%) Create Matrix4x4 translate Matrix
function Matrix4x4_VecTranslate(vector)
	local trans = Matrix4x4_Identity()
	trans["e14"] = vector["x"]
	trans["e24"] = vector["y"]
	trans["e34"] = vector["z"]
	return trans
end

--(%%) Create Matrix4x4 translate back matrix
function Matrix4x4_VecTranslateBack(vector)
	local trans = Matrix4x4_Identity()
	trans["e14"] = -vector["x"]
	trans["e24"] = -vector["y"]
	trans["e34"] = -vector["z"]
	return trans
end

--(%%) Create Matrix4x4 xyz Scale Matrix
function Matrix4x4_ScaleVec(vector)
	local trans = Matrix4x4_Identity()
	trans["e11"] = vector["x"]
	trans["e22"] = vector["y"]
	trans["e33"] = vector["z"]
	return trans
end

--(%%) Create Matrix4x4 uniform Scale Matrix 
function Matrix4x4_ScaleScalar(scalar)
	local trans = Matrix4x4_Identity()
	trans["e11"] = scalar
	trans["e22"] = scalar
	trans["e33"] = scalar
	return trans
end

--(%%) Create Matrix4x4 uniform othonormal basis
function Matrix4x4_3xVec3OthonormalBasis(vecI, vecJ, vecK)
	return Matrix3x3ToMatrix4x4(Matrix3x3_fromColVecs(vecI, vecJ, vecK))
end

function PrintMatrix4x4(m)
ConsolePrintString(
	m["type"] .. " , " ..
	m["DimX"] .. " , " ..
	m["DimY"] )

ConsolePrintString(
	m["e11"] .. " , " ..
	m["e21"] .. " , " ..
	m["e31"] .. " , " ..
	m["e41"] )
	
ConsolePrintString(
	m["e12"] .. " , " ..
	m["e22"] .. " , " ..
	m["e32"] .. " , " ..
	m["e42"] )
	
ConsolePrintString(m["e13"] .. " , " ..
	m["e23"] .. " , " ..
	m["e33"] .. " , " ..
	m["e43"] )
	
ConsolePrintString(m["e14"] .. " , " ..
	m["e24"] .. " , " ..
	m["e34"] .. " , " ..
	m["e44"] )

end



function Matrix4x4_transpose(m)
	return Matrix4x4(
	m["e11"],
	m["e21"],
	m["e31"],
	m["e41"],
	
	
	m["e12"],
	m["e22"],
	m["e32"],
	m["e42"],
	
	m["e13"],
	m["e23"],
	m["e33"],
	m["e43"],
	m["e14"],
	m["e24"],
	m["e34"],
	m["e44"]	)
end



--(%%) matrix vector multiply
--(%%) this one assumes the input w cooridinate is 1 and sets discards the output w.
function Matrix4x4_Vec3p1(m,v)


	local vecfinal =  Vector(
	m["e11"]*v["x"]+
	m["e21"]*v["y"]+
	m["e31"]*v["z"]+
	m["e41"],
	
	
	m["e12"]*v["x"]+
	m["e22"]*v["y"]+
	m["e32"]*v["z"]+
	m["e42"],
	
	m["e13"]*v["x"]+
	m["e23"]*v["y"]+
	m["e33"]*v["z"]+
	m["e43"]	)  -- skip w...
	return vecfinal
	
end
