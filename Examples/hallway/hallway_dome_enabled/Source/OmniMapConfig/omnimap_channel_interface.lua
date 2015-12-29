-- a render channel needs a few things set
-- camera perspective info
-- camera view rotation and translation  info
---this file sets up the render channel positions and view matrix ...
---
---
--- Check if we are using col major consistently...
RT_NONE = 0
RT_FRAME_BUFFER_OBJECT = 1
RT_PBUFFER = 2
RT_BACK_BUFFER = 3
function SetChannelResolution(channel, res)
	SetChannelParameter(channel, "resolution", res)
	InitializeChannel(channel);
end

function SetChannelResolutionXY(channel, xres, yres)
	SetChannelParameter(channel, "XResolution", xres)
	SetChannelParameter(channel, "YResolution", yres)
	InitializeChannel(channel);
end


function SetCameraInfo(channel, fov,apectratio,near,far,consolidatematrix)
--	SetChannelParameter(channel,"cam_info.fovY",fov)  ---- removed
--	SetChannelParameter(channel,"cam_info.AspectRatio",apectratio)---- removed
	SetChannelParameter(channel,"ConsolidateCameraIntoProjectionMatrix",consolidatematrix)
	


	local ClipTop    =  math.tan( fov * 3.14159/360.0) * near -- convert to radians
	local ClipBottom = -ClipTop;
	local ClipLeft   = apectratio*ClipBottom;
	local ClipRight  = apectratio*ClipTop;	
	
	SetChannelParameter(channel,"cam_info.ClipTop",ClipTop)
	SetChannelParameter(channel,"cam_info.ClipBottom",ClipBottom)
	SetChannelParameter(channel,"cam_info.ClipLeft",ClipLeft)
	SetChannelParameter(channel,"cam_info.ClipRight",ClipRight)
	SetChannelParameter(channel,"cam_info.ClipNear",near)
	SetChannelParameter(channel,"cam_info.ClipFar",far)
end


--(%%) LuaGlue interface for setting the View matrix of the render channel.
function SetViewRotateMatrix4x4(channel,Mtr)
--  to do put error checking here to make sure mtr is 4x4 matrix.
--- Check if we are using col major consistently...
--- Check if we are using col major consistently...

	SetChannelParameter(channel,"Matrix_WorldView.0.0" ,Mtr["e11"])
	SetChannelParameter(channel,"Matrix_WorldView.0.1" ,Mtr["e12"])
	SetChannelParameter(channel,"Matrix_WorldView.0.2" ,Mtr["e13"])
	SetChannelParameter(channel,"Matrix_WorldView.0.3" ,Mtr["e14"])

	SetChannelParameter(channel,"Matrix_WorldView.1.0" ,Mtr["e21"])
	SetChannelParameter(channel,"Matrix_WorldView.1.1" ,Mtr["e22"])
	SetChannelParameter(channel,"Matrix_WorldView.1.2" ,Mtr["e23"])
	SetChannelParameter(channel,"Matrix_WorldView.1.3" ,Mtr["e24"])
	
	SetChannelParameter(channel,"Matrix_WorldView.2.0" ,Mtr["e31"])
	SetChannelParameter(channel,"Matrix_WorldView.2.1" ,Mtr["e32"])
	SetChannelParameter(channel,"Matrix_WorldView.2.2" ,Mtr["e33"])
	SetChannelParameter(channel,"Matrix_WorldView.2.3" ,Mtr["e34"])
	
	SetChannelParameter(channel,"Matrix_WorldView.3.0" ,Mtr["e41"])
	SetChannelParameter(channel,"Matrix_WorldView.3.1" ,Mtr["e42"])
	SetChannelParameter(channel,"Matrix_WorldView.3.2" ,Mtr["e43"])
	SetChannelParameter(channel,"Matrix_WorldView.3.3" ,Mtr["e44"])
end


--(%%) creates the view matrix from a Eye postion, View Direction(not focus postion like gluLookAt), and  head up direction
function CreateViewMatrixFromBasis(VecAud, View,Up)
-- I should force the heads up direction to be orthogonal
--  VERIFY THAT THIS IS COL MAJOR...
--  // 0) create basis vectors...
--	// 1) Rotate and twist head...
--	// 2) Move To Audience Position
--  // *is this the right order????
-- I might be able to construct this matrix directly using   http://pyopengl.sourceforge.net/documentation/manual/gluLookAt.3G.html
-- Should i be inverting the matrix before loading it??
-- Should i be using -VecAud instead of VecAud?

	local VecU =Vector_Normalize(Up);
	local VecF =Vector_Normalize(View);
	local VecNegF =Vector_Reverse(View);
	local VecS = Vector_Cross(VecF,VecU)
	local matTrans = Matrix4x4_VecTranslate(VecAud)
	local matRotate = Matrix3x3ToMatrix4x4(Matrix3x3_fromRowVecs(VecS,VecU ,VecNegF ))
	
	return Matrix4x4_MxM(matRotate,matTrans)--  <---- is this the right order????
end

--(%%) this function calculates the position of the render channels given the vector of its view and its field of view
--(%%) this function will need to be replaced when we use MATRIX projective texturing rather than my custom shader equations
--(%%)

function PositionRenderChannelInProjectiveTexturingWorldUsingVectors(channel,VecAudiencePos,ViewVec,HeadsUp,FovY,aspect_ratio)

	local distancefromaudience = 1
	local widthofviewportal = distancefromaudience*math.tan(DegreesToRadians( FovY/2));
	local heightofviewportal = widthofviewportal *aspect_ratio;

	local TVec  = Vector_VecXScalar( VectorCopy(HeadsUp) ,heightofviewportal)
	local SVec  = Vector_VecXScalar( Vector_Cross(ViewVec,HeadsUp) ,widthofviewportal)
	local nTVec = Vector_VecXScalar( Vector_Reverse(HeadsUp) , heightofviewportal)
	local nSVec = Vector_VecXScalar( Vector_Reverse(Vector_Cross(ViewVec,HeadsUp) )  , widthofviewportal)
	local vector_to_center = Vector_VecXScalar(ViewVec,distancefromaudience)
	local VectorCenter = Vector_Add(VecAudiencePos,vector_to_center)
	local vecbl = Vector_Add(VectorCenter,Vector_Add(nTVec,nSVec)) 
	local vecbr = Vector_Add(VectorCenter,Vector_Add(nTVec,SVec)) 
	local vectl = Vector_Add(VectorCenter,Vector_Add(TVec,nSVec)) 
	local vectr = Vector_Add(VectorCenter,Vector_Add(TVec,SVec)) 

	SetChannelParameter(channel,"ViewWindow_Quad.bl.x",vecbl["x"])
	SetChannelParameter(channel,"ViewWindow_Quad.bl.y",vecbl["y"])
	SetChannelParameter(channel,"ViewWindow_Quad.bl.z",vecbl["z"])		
	SetChannelParameter(channel,"ViewWindow_Quad.br.x",vecbr["x"])
	SetChannelParameter(channel,"ViewWindow_Quad.br.y",vecbr["y"])
	SetChannelParameter(channel,"ViewWindow_Quad.br.z",vecbr["z"])
	SetChannelParameter(channel,"ViewWindow_Quad.tl.x",vectl["x"])
	SetChannelParameter(channel,"ViewWindow_Quad.tl.y",vectl["y"])
	SetChannelParameter(channel,"ViewWindow_Quad.tl.z",vectl["z"])
	SetChannelParameter(channel,"ViewWindow_Quad.tr.x",vectr["x"])
	SetChannelParameter(channel,"ViewWindow_Quad.tr.y",vectr["y"])
	SetChannelParameter(channel,"ViewWindow_Quad.tr.z",vectr["z"])
end

