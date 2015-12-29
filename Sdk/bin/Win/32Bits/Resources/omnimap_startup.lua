ConsolePrintString("OmniMap Geometry Correction Libs")
ConsolePrintString("(c) Elumenati")

function AddSupportDirectory(filename)
	return string.format("%s%s", LuaSupportDir, filename)
end

mathFile = AddSupportDirectory( "omnimap_math.lua")
vectorFile =  AddSupportDirectory( "omnimap_vector.lua")
matrix3x3File =  AddSupportDirectory( "omnimap_matrix3x3.lua")
matrix4x4File =  AddSupportDirectory( "omnimap_matrix4x4.lua")
helpFuncsFile =  AddSupportDirectory( "omnimap_helperfunctions.lua")
lensFile =  	AddSupportDirectory( "omnimap_lens_interface.lua")
channelFile =  	AddSupportDirectory( "omnimap_channel_interface.lua")
screenFile =  	AddSupportDirectory( "omnimap_screen.lua")
hudFile =  	AddSupportDirectory( "omnimap_hud.lua")
projectorFile =  AddSupportDirectory( "omnimap_projector.lua")
renderchannelFile =  AddSupportDirectory( "omnimap_renderchannel.lua")


dofile(mathFile)
dofile(vectorFile) 
dofile(matrix3x3File) 
dofile(matrix4x4File) 
dofile(helpFuncsFile) 
dofile(lensFile) 
dofile(channelFile) 
dofile(screenFile) 
dofile(hudFile)  
dofile(projectorFile)  
dofile(renderchannelFile)

dofile(AddSupportDirectory("../omnimap_user_edit.lua"))    -- user editable config
dofile(AddSupportDirectory("omnimap_dome_wiz_ai.lua"))  -- dome wizard

------------------------------------------------------------------------------------------------------------------
if(createacylinder) then
	CreateCylinder("MyCylinder",cylinder_radius, cylinder_height, DomeCenter,
	vec_up,vec_forward,
	theta_min,theta_max,DomeTesselation,
	ScreenRed,ScreenGreen,ScreenBlue)
	createacylinder = false
end

if(createadome) then
	CreateDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation)
	createadome = false
end

if(createapartialdome) then
	CreatePartialDome("MyDome", DomeCenter,DomeOrientation,DomeRadius_unitless,DomeTesselation,
		DomeNumStacks, DomeNumRings)
	createadome = false
end

if (createapanorama) then
	CreatePanorama("MyPanorama", numRings,
		numSides,
		DomeCenter,
		DomeOrientation,
		ringRadius,
		torusRadius,
		ringStartAngle,
		ringEndAngle,
		sideStartAngle,
		sideEndAngle)
end

if (createaplane) then
	CreatePlane("MyPlane", PlaneWidth, PlaneHeight, PlaneCenter, PlaneOrientation) 
end

if(usestencilmask) then
	StencilMask_filename_set(StencilMask_filename)
	usestencilmask = false
end


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


if(leftChannel)
then
	leftChannel:CreateChannel() -- last chance to set fbo
	leftChannel:InitChannel()   -- last chance to set res
	PositionRenderChannel_In_HudDisplay("Left",	CreateXY_Rectangle(centerx-scale*.5-offset,centery,scale,scale) )
	if(leftChannel.UseOmniMapMatrix)
	then 
	leftChannel:SetViewRotateMatrix4x4(CreateViewMatrixFromBasis(AudiencePos, RotateVec(BaseViewDirection,BaseHeadsUp,45)  , VectorCopy(BaseHeadsUp) ))

	end
	leftChannel:Update()
	end
if(rightChannel)
then
	rightChannel:CreateChannel() -- last chance to set fbo
	rightChannel:InitChannel()   -- last chance to set res
	PositionRenderChannel_In_HudDisplay("Right",  CreateXY_Rectangle(centerx+scale*.5+offset,centery,scale,scale))
	if(rightChannel.UseOmniMapMatrix) 
	then 
		rightChannel:SetViewRotateMatrix4x4(CreateViewMatrixFromBasis(AudiencePos, RotateVec(BaseViewDirection,BaseHeadsUp,-45)  , VectorCopy(BaseHeadsUp) ))
	end
	rightChannel:Update()
	
	end
if(bottomChannel)
then
	bottomChannel:CreateChannel() -- last chance to set fbo
	bottomChannel:InitChannel()   -- last chance to set res
	PositionRenderChannel_In_HudDisplay("Bottom",	CreateXY_Rectangle(centerx-scale*.5-offset,centery+scale+offset,scale,scale))
	if(bottomChannel.UseOmniMapMatrix) 
	then 
		bottomChannel:SetViewRotateMatrix4x4(CreateViewMatrixFromBasis(AudiencePos,Vector_Reverse(BaseHeadsUp) ,RotateVec(BaseViewDirection,BaseHeadsUp,45) ))
	end
	bottomChannel:Update()
        end
if(topChannel)
then
	topChannel:CreateChannel() -- last chance to set fbo
	topChannel:InitChannel()   -- last chance to set res
	PositionRenderChannel_In_HudDisplay("Top",	CreateXY_Rectangle(centerx-scale*.5-offset,centery-scale-offset,scale,scale))
	if(topChannel.UseOmniMapMatrix)
	then
		topChannel:SetViewRotateMatrix4x4(CreateViewMatrixFromBasis(AudiencePos,VectorCopy(BaseHeadsUp) , RotateVec(Vector_Reverse(BaseViewDirection),BaseHeadsUp,45)))
        end
	topChannel:Update()
        end
if(frontChannel)
then
	frontChannel:CreateChannel() -- last chance to set fbo
	frontChannel:InitChannel()   -- last chance to set res
	PositionRenderChannel_In_HudDisplay("Front",	CreateXY_Rectangle(centerx-scale*1.5-offset-offset,centery,scale,scale))
	if(frontChannel.UseOmniMapMatrix) 
	then
		frontChannel:SetViewRotateMatrix4x4(CreateViewMatrixFromBasis(AudiencePos,RotateVec(BaseViewDirection,BaseHeadsUp,45+180) ,VectorCopy(BaseHeadsUp) ))
        end
	frontChannel:Update()
        end
if(backChannel)
then
	backChannel:CreateChannel() -- last chance to set fbo
	backChannel:InitChannel()   -- last chance to set res
	PositionRenderChannel_In_HudDisplay("Back",	CreateXY_Rectangle(centerx+scale*1.5+offset+offset,centery,scale,scale)) 
	if(backChannel.UseOmniMapMatrix)
	then
		backChannel:SetViewRotateMatrix4x4(CreateViewMatrixFromBasis(AudiencePos, RotateVec(BaseViewDirection,BaseHeadsUp,-45+180), VectorCopy(BaseHeadsUp)))
        end
	backChannel:Update()
end

-- create and set meta data
local NumberOfChannels = 0 --(TODO) switch to TopChannelNumber etc  have create_channel return this number and clear channel reset it to zero 
if(topChannel)then 		CreateChannelMetaData("Top",NumberOfChannels)    NumberOfChannels = NumberOfChannels + 1 end
if(bottomChannel)then 	CreateChannelMetaData("Bottom",NumberOfChannels) NumberOfChannels = NumberOfChannels + 1 end
if(leftChannel)then 		CreateChannelMetaData("Left",NumberOfChannels)  NumberOfChannels = NumberOfChannels + 1 end
if(rightChannel)then 	CreateChannelMetaData("Right",NumberOfChannels) NumberOfChannels = NumberOfChannels + 1 end
if(frontChannel)then 	CreateChannelMetaData("Front",NumberOfChannels) NumberOfChannels = NumberOfChannels + 1 end
if(backChannel)then 		CreateChannelMetaData("Back",NumberOfChannels)  NumberOfChannels = NumberOfChannels + 1 end

--- Final Step is to recompute the PlaneEquations shader parameters
--RecomputePlaneEquations()
