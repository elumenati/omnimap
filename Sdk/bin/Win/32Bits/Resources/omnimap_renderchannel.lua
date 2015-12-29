---  Object oriented lua file for RenderChannel
---  usage 
---
---  local vec = RenderChannel:new(Point_RenderChannelPostion,Vec_RenderChannelLookVec,Vec_RenderChannelUpVec)   -- Create a vector
---  vec:Update()
---

ClassRenderChannel = { } -- Create a table to hold the class methods

function ClassRenderChannel:new(_name) -- The constructor function
  local object = 
  {
	channelname = _name,
	renderTarget = GLOBAL_renderTarget,
	channelResX = GLOBAL_channelResX,
	channelResY = GLOBAL_channelResY,
	ClipNear = GLOBAL_clipNear,
	multiSampleQuality = GLOBAL_multiSampleQuality,
	ClipFar  = GLOBAL_clipFar,
	ClipBottom =-1,
	ClipTop = 1,
	ClipLeft = -1,
	ClipRight =1,
	consolidatematrix = GLOBAL_ConsolidateCameraIntoProjectionMatrix,
	UseOmniMapMatrix = GLOBAL_UseOmniMapMatrix,
	Mtr = Matrix4x4_Identity()
  }
  setmetatable(object, {
    -- Overload the index event so that fields not present within the object are
    -- looked up in the prototype Vector table
    __index = ClassRenderChannel
  })
  return object 
end


function ClassRenderChannel:Update()
	
	SetChannelParameter(self.channelname,"cam_info.ClipTop",self.ClipTop)
	SetChannelParameter(self.channelname,"cam_info.ClipBottom",self.ClipBottom)
	SetChannelParameter(self.channelname,"cam_info.ClipLeft",self.ClipLeft)
	SetChannelParameter(self.channelname,"cam_info.ClipRight",self.ClipRight)
	SetChannelParameter(self.channelname,"cam_info.ClipNear",self.ClipNear)
	SetChannelParameter(self.channelname,"cam_info.ClipFar",self.ClipFar)
	SetChannelParameter(self.channelname,"ConsolidateCameraIntoProjectionMatrix",self.consolidatematrix);

	SetChannelParameter(self.channelname,"Matrix_WorldView.0.0" ,self.Mtr["e11"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.0.1" ,self.Mtr["e12"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.0.2" ,self.Mtr["e13"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.0.3" ,self.Mtr["e14"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.1.0" ,self.Mtr["e21"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.1.1" ,self.Mtr["e22"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.1.2" ,self.Mtr["e23"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.1.3" ,self.Mtr["e24"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.2.0" ,self.Mtr["e31"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.2.1" ,self.Mtr["e32"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.2.2" ,self.Mtr["e33"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.2.3" ,self.Mtr["e34"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.3.0" ,self.Mtr["e41"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.3.1" ,self.Mtr["e42"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.3.2" ,self.Mtr["e43"])
	SetChannelParameter(self.channelname,"Matrix_WorldView.3.3" ,self.Mtr["e44"])
	if("D3D" == GLOBAL_renderer)
	then
		SetChannelParameter(self.channelname, "MultiSampleQuality" ,self.multiSampleQuality)
	end
	if("D3D10" == GLOBAL_renderer)
	then
		SetChannelParameter(self.channelname, "MultiSampleQuality" ,self.multiSampleQuality)
	end
	if("D3D11" == GLOBAL_renderer)
	then
		SetChannelParameter(self.channelname, "MultiSampleQuality" ,self.multiSampleQuality)
	end
end


function ClassRenderChannel:CreateChannel()
	Create_Channel(self.channelname)
end

function ClassRenderChannel:InitChannel()
	if("D3D" == GLOBAL_renderer)
	then
		SetChannelParameter(self.channelname, "MultiSampleQuality" ,self.multiSampleQuality)
	end
	if("D3D10" == GLOBAL_renderer)
	then
		SetChannelParameter(self.channelname, "MultiSampleQuality" ,self.multiSampleQuality)
	end
	if("D3D11" == GLOBAL_renderer)
	then
		SetChannelParameter(self.channelname, "MultiSampleQuality" ,self.multiSampleQuality)
	end
	SetChannelParameter(self.channelname, "RenderTarget", self.renderTarget)
	SetChannelParameter(self.channelname, "XResolution", self.channelResX )
	SetChannelParameter(self.channelname, "YResolution", self.channelResY )
	InitializeChannel(self.channelname)
end

function ClassRenderChannel:SetCameraInfoAsymm(fovLeft, fovRight, fovDown, fovUp, near, far)
	self.ClipTop = math.tan( fovUp * 3.14159/360.0 * 2.0) * near;
	self.ClipBottom = -math.tan( fovDown * 3.14159/360.0 * 2.0) * near;
	self.ClipLeft = -math.tan(fovLeft * 3.14159/360.0 * 2.0) * near;
	self.ClipRight = math.tan(fovRight * 3.14159/360.0 * 2.0) * near;
	self.ClipNear = near;
	self.ClipFar = far;		
end

function ClassRenderChannel:SetCameraInfo(fov,apectratio,near,far)
	self.ClipTop    =  math.tan( fov * 3.14159/360.0) * near -- convert to radians
	self.ClipBottom = -self.ClipTop;
	self.ClipLeft   = apectratio*self.ClipBottom;
	self.ClipRight  = apectratio*self.ClipTop;	
	self.ClipNear = near;
	self.ClipFar = far;
end


--(%%) LuaGlue interface for setting the View matrix of the render channel.

function ClassRenderChannel:SetViewRotateMatrix4x4(_Mtr)
	self.Mtr = Matrix4x4_CopyMatrix(_Mtr)
end



