ConsolePrintString("OmniMap Geometry Correction Libs")
ConsolePrintString("(c) Elumenati")

mathFile = string.format("%s%s", LuaSupportDir, "omnimap_math.lua")
vectorFile = string.format("%s%s", LuaSupportDir, "omnimap_vector.lua")
matrix3x3File = string.format("%s%s", LuaSupportDir, "omnimap_matrix3x3.lua")
matrix4x4File = string.format("%s%s", LuaSupportDir, "omnimap_matrix4x4.lua")
helpFuncsFile = string.format("%s%s", LuaSupportDir, "omnimap_helperfunctions.lua")
lensFile = string.format("%s%s", LuaSupportDir, "omnimap_lens_interface.lua")
channelFile = string.format("%s%s", LuaSupportDir, "omnimap_channel_interface.lua")
screenFile = string.format("%s%s", LuaSupportDir, "omnimap_screen.lua")
hudFile = string.format("%s%s", LuaSupportDir, "omnimap_hud.lua")


dofile(mathFile)
dofile(vectorFile) 
dofile(matrix3x3File) 
dofile(matrix4x4File) 
dofile(helpFuncsFile) 
dofile(lensFile) 
dofile(channelFile) 
dofile(screenFile) 
dofile(hudFile)  

dofile("OmniMapConfig/omnimap_user_edit.lua")    -- user editable config
SetOmnimapParameter("RendererType", Renderer)



------------------------------------------------------------------------------------------------------------------
if(createacylinder) then
	CreateCylinder("MyCylinder",cylinder_radius, cylinder_height, DomeCenter,vec_up,vec_forward,theta_min,theta_max,DomeTesselation,ScreenRed,ScreenGreen,ScreenBlue)
	createacylinder = false
end

if(createadome) then
	CreateDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation)
	createadome = false
end

if(usestencilmask) then

	StencilMask_filename_set(StencilMask_filename)
	usestencilmask = false
end

SetProjector(Point_ProjectorPostion,Vec_ProjectorLookVec,Vec_ProjectorUpVec)
SetAudiencePosition(AudiencePos)


------------------------------------------------------------------------------------------------------------------




--- add any channel meta-data you want in this function
function CreateChannelMetaData(channelname,channelNumber)   
	CreateChannelData_STRING(channelname,		"name")
	SetChannelData_STRING(channelname,	"name",channelname)    
	CreateChannelData_INT(channelname,	"ChannelNumber") 
	SetChannelData_INT(channelname,	"ChannelNumber",channelNumber)


	CreateChannelData_INT(channelname,	"ExampleMetaData") 
	SetChannelData_INT(channelname,	"ExampleMetaData",99+channelNumber)

end



------------------------------------------------------------
--  OMNIMAP's RENDERING STATE OPTIONS
----
-- main options:
--  * Clear color
--  * 
------------------------------------------------------------
SetOmnimapParameter("clearcolor_r",0)
SetOmnimapParameter("clearcolor_g",0)
SetOmnimapParameter("clearcolor_b",0)  
SetOmnimapParameter("clearcolor_a",1)  
SetOmnimapParameter("GL_STATE_CLEAR_AT_STARTFRAME"     ,"true")		-- glstate options... posible performace tweaks 
SetOmnimapParameter("GL_STATE_glDisable_GL_DEPTH_TEST" ,"true")  -- glstate options... posible performace tweaks
SetOmnimapParameter("GL_STATE_glDepthMask_FALSE"       ,"true")		-- glstate options... posible performace tweaks
-------------------------------------------------------------


-- ----------------------------
-- --  Lens
-- ----------------------------
func_OpticalOffsetAxis()
-- ----------------------------
-- --  Audience
-- ----------------------------




-------------------------------------------
---------- these are callbacks ------------
-- each function call will also call a function pointer in c++   ...????? i forgot what i meant here
-------------------------------------------
-- When the screen gets resized this gets called....
function onResize()
	func_OpticalOffsetAxis()
	Clear()  -- clear the screen.
end

-----------
--(TODO) --
-----------
function onModeChange()

end
-- to do...
function onExit()

end





--- hud paraeters

centerx = 0   	-- HUD parameters
centery = -.50	-- HUD parameters
scale = .3	-- HUD parameters
offset = .00	-- HUD parameters



local LeftViewVec = RotateVec(BaseViewDirection,BaseHeadsUp,45)
local LeftHeadsUp = VectorCopy(BaseHeadsUp)

local RightViewVec = RotateVec(BaseViewDirection,BaseHeadsUp,-45)
local RightHeadsUp = VectorCopy(BaseHeadsUp)

local TopViewVec = VectorCopy(BaseHeadsUp)
local TopHeadsUp = RotateVec(Vector_Reverse(BaseViewDirection),BaseHeadsUp,45)

local BotViewVec = Vector_Reverse(BaseHeadsUp)
local BotHeadsUp = RotateVec(BaseViewDirection,BaseHeadsUp,45)

local FrontViewVec = RotateVec(BaseViewDirection,BaseHeadsUp,45+180)
local FrontHeadsUp = VectorCopy(BaseHeadsUp)

local BackViewVec = RotateVec(BaseViewDirection,BaseHeadsUp,-45+180)
local BackHeadsUp = VectorCopy(BaseHeadsUp)
 





---------- Create Channels
--if(UseLeftChannel)then   Create_Channel("Left",channelRes,usefbo) end
--if(UseRightChannel)then  Create_Channel("Right",channelRes,usefbo) end
--if(UseTopChannel)then    Create_Channel("Top",channelRes,usefbo) end
--if(UseBottomChannel)then Create_Channel("Bottom",channelRes,usefbo) end
--if(UseFrontChannel)then  Create_Channel("Front",channelRes,usefbo) end
--if(UseBackChannel)then   Create_Channel("Back",channelRes,usefbo) end

if(UseLeftChannel)then   
	Create_Channel("Left", usefbo) 
	SetChannelParameter("Left", "RenderTarget", renderTarget)
	SetChannelResolutionXY("Left", channelResX, channelResY)
end
if(UseRightChannel)then  
	Create_Channel("Right", usefbo)
	SetChannelParameter("Right", "RenderTarget", renderTarget)
	SetChannelResolutionXY("Right", channelResX, channelResY)
end
if(UseTopChannel)then    
	Create_Channel("Top", usefbo)
	SetChannelParameter("Top", "RenderTarget", renderTarget)
	SetChannelResolutionXY("Top", channelResX, channelResY)
end
if(UseBottomChannel)then 
	Create_Channel("Bottom", usefbo)
	SetChannelParameter("Bottom", "RenderTarget", renderTarget)
	SetChannelResolutionXY("Bottom", channelResX, channelResY)
end
if(UseFrontChannel)then  
	Create_Channel("Front", usefbo)
	SetChannelParameter("Front", "RenderTarget", renderTarget)
	SetChannelResolutionXY("Front", channelResX, channelResY)
end
if(UseBackChannel)then   
	Create_Channel("Back", usefbo)
	SetChannelParameter("Back", "RenderTarget", renderTarget)
	SetChannelResolutionXY("Back", channelResX, channelResY)
	
end



----------- Set up HUD
if(UseLeftChannel)	 then PositionRenderChannel_In_HudDisplay("Left",	CreateXY_Rectangle(centerx-scale*.5-offset,centery,scale,scale)) end
if(UseRightChannel)	 then PositionRenderChannel_In_HudDisplay("Right",  CreateXY_Rectangle(centerx+scale*.5+offset,centery,scale,scale)) end
if(UseTopChannel)	 then PositionRenderChannel_In_HudDisplay("Top",	CreateXY_Rectangle(centerx-scale*.5-offset,centery+scale+offset,scale,scale)) end
if(UseBottomChannel) then PositionRenderChannel_In_HudDisplay("Bottom",	CreateXY_Rectangle(centerx-scale*.5-offset,centery-scale-offset,scale,scale)) end
if(UseBackChannel)	 then PositionRenderChannel_In_HudDisplay("Back",	CreateXY_Rectangle(centerx-scale*1.5-offset-offset,centery,scale,scale)) end
if(UseFrontChannel)	 then PositionRenderChannel_In_HudDisplay("Front",	CreateXY_Rectangle(centerx+scale*1.5+offset+offset,centery,scale,scale)) end

if (UseOmniMapMatrix) then
	-- Set Projection matrix
	if(UseLeftChannel) then	
		SetCameraInfoAsymm("Left", LeftFovLeft, LeftFovRight, LeftFovDown, LeftFovUp, nearclip, farclip,
			ConsolidateCameraIntoProjectionMatrix) 
	end
	if(UseRightChannel) then
		SetCameraInfoAsymm( "Right", RightFovLeft, RightFovRight, RightFovDown, RightFovUp, nearclip, farclip,
			ConsolidateCameraIntoProjectionMatrix) 
	end
	if(UseTopChannel) then
		SetCameraInfoAsymm( "Top",	 TopFovLeft, TopFovRight, TopFovDown, TopFovUp, nearclip, farclip,
			ConsolidateCameraIntoProjectionMatrix) 
	end
	if(UseBottomChannel) then
		SetCameraInfoAsymm( "Bottom", BottomFovLeft, BottomFovRight, BottomFovDown, BottomFovUp, nearclip,farclip,
			ConsolidateCameraIntoProjectionMatrix) 
	end
	if(UseFrontChannel) then
		SetCameraInfoAsymm( "Front", FrontFovLeft, FrontFovRight, FrontFovDown, FrontFovUp, nearclip, farclip,
			ConsolidateCameraIntoProjectionMatrix) 
	end
	if(UseBackChannel) then	
		SetCameraInfoAsymm( "Back", BackFovLeft, BackFovRight, BackFovDown, BackFovUp,nearclip,farclip,
			ConsolidateCameraIntoProjectionMatrix) 
	end
	
	
	--- Set View Matrix
	if(UseLeftChannel) then 
		SetViewRotateMatrix4x4("Left", 
			CreateViewMatrixFromBasis(AudiencePos, LeftViewVec  , 
			LeftHeadsUp )) 
	end
	if(UseRightChannel) then 
		SetViewRotateMatrix4x4("Right", 
			CreateViewMatrixFromBasis(AudiencePos, RightViewVec  , 
			RightHeadsUp )) 
	end
	if(UseTopChannel) then 
		SetViewRotateMatrix4x4("Top",   
			CreateViewMatrixFromBasis(AudiencePos, TopViewVec    , 
			TopHeadsUp   )) 
	end
	if(UseBottomChannel) then 
		SetViewRotateMatrix4x4("Bottom", 
			CreateViewMatrixFromBasis(AudiencePos, BotViewVec    , 
				BotHeadsUp   ) ) 
	end
	if(UseFrontChannel)   then 
		SetViewRotateMatrix4x4("Front",   
			CreateViewMatrixFromBasis(AudiencePos, FrontViewVec    , 
			FrontHeadsUp   )) 
	end
	if(UseBackChannel) then 
		SetViewRotateMatrix4x4("Back", 
			CreateViewMatrixFromBasis(AudiencePos, BackViewVec    , 
			BackHeadsUp   ) ) 
	end
else
	if(UseLeftChannel) then
		SetChannelParameter("Left", "UseOmniMapMatrix", false);
        end
        if(UseRightChannel) then
		SetChannelParameter("Right", "UseOmniMapMatrix", false);
        end
        if(UseTopChannel) then
		SetChannelParameter("Top", "UseOmniMapMatrix", false);
        end
        if(UseBottomChannel) then
		SetChannelParameter("Bottom", "UseOmniMapMatrix", false);
        end
        if(UseFrontChannel)   then
		SetChannelParameter("Front", "UseOmniMapMatrix", false);
        end
        if(UseBackChannel) then
		SetChannelParameter("Back", "UseOmniMapMatrix", false);
        end
end

-- set shader parameters for render channel
-- when we switch to 
--if(UseLeftChannel)		then PositionRenderChannelInProjectiveTexturingWorldUsingVectors( "Left",	AudiencePos, LeftViewVec  , LeftHeadsUp ,LeftFovY,R)end
--if(UseRightChannel)		then PositionRenderChannelInProjectiveTexturingWorldUsingVectors( "Right",	AudiencePos, RightViewVec , RightHeadsUp ,RightFovY,R)end
--if(UseTopChannel)		then PositionRenderChannelInProjectiveTexturingWorldUsingVectors( "Top",		AudiencePos, TopViewVec   , TopHeadsUp ,TopFovY,R)end
--if(UseBottomChannel)	then PositionRenderChannelInProjectiveTexturingWorldUsingVectors( "Bottom",AudiencePos, BotViewVec   , BotHeadsUp ,BotFovY,R)end
--if(UseFrontChannel)		then PositionRenderChannelInProjectiveTexturingWorldUsingVectors( "Front",		AudiencePos, FrontViewVec   , FrontHeadsUp ,FrontFovY,R)end
--if(UseBackChannel)		then PositionRenderChannelInProjectiveTexturingWorldUsingVectors( "Back",AudiencePos, BackViewVec   , BackHeadsUp ,BackFovY,R)end

-- create and set meta data
local NumberOfChannels = 0 --(TODO) switch to TopChannelNumber etc  have create_channel return this number and clear channel reset it to zero 
if(UseTopChannel)then 		CreateChannelMetaData("Top",NumberOfChannels)    NumberOfChannels = NumberOfChannels + 1 end
if(UseBottomChannel)then 	CreateChannelMetaData("Bottom",NumberOfChannels) NumberOfChannels = NumberOfChannels + 1 end
if(UseLeftChannel)then 		CreateChannelMetaData("Left",NumberOfChannels)  NumberOfChannels = NumberOfChannels + 1 end
if(UseRightChannel)then 	CreateChannelMetaData("Right",NumberOfChannels) NumberOfChannels = NumberOfChannels + 1 end
if(UseFrontChannel)then 	CreateChannelMetaData("Front",NumberOfChannels) NumberOfChannels = NumberOfChannels + 1 end
if(UseBackChannel)then 		CreateChannelMetaData("Back",NumberOfChannels)  NumberOfChannels = NumberOfChannels + 1 end

--- Final Step is to recompute the PlaneEquations shader parameters
--RecomputePlaneEquations()
