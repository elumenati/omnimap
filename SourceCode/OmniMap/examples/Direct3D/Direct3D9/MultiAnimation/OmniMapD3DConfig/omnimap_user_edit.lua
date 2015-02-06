---------------------------------------------------------------
-----------------  UNIT CONVERSION SETUP  ---------------------
SetupUnitConversionsByDomeRadiusInInches(MetersToInches(1))  -- set it up to be a 1 meter radius
---------------------------------------------------------------


---
--- set lens, screen, audience
---

-- This is where you should set up the dome and projector configuration

--  * func_OpticalOffsetAxis
--  * Point_ProjectorPostion 
--  * AudiencePos			    

-- ----------------------------
-- --  Lens  Cropping
-- ----------------------------
-- set to , LensCrop_HDome, LensCrop_VDome,  or create a custom function! 
--	func_OpticalOffsetAxis = LensCrop_CustomDome



----------------------------------  
----  Dome
---------------------------------
-- Decrease this value to decrease final tesselation of sphere.
-- orientation is managed by omnimap_dome_wiz_ai

---------------------------------
----  Projector
---------------------------------
--Point_ProjectorPostion = Vector(0,0,0)
-- orientation is managed by omnimap_dome_wiz_ai

-- ----------------------------
-- --  Audience
-- ----------------------------
-- orientation is managed by omnimap_dome_wiz_ai

-- ----------------------------
-- -- HUD 
-- ----------------------------
-- Set to false to turn off Heads Up Display
SetOmnimapParameter("displayHUD","false")


--HorizontalDome


----  suggest the defaults...
ScreenshapeDefaults = "Custom"  -- HorizontalDome,VerticalDome,PanoDome,Cylinder,Custom

---- have dome ai wiz set it up
-- run dome wiz
dofile("OmniMapD3DConfig/omnimap_dome_wiz_ai.lua")
--- tweak what needs tweaking



---