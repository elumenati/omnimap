-- This is where you should set up the dome and projector configuration

--  * func_CurrentLens
--  * Point_ProjectorPostion 
--  * AudiencePos			    

-- ----------------------------
-- --  Lens  Cropping
-- ----------------------------
-- set to , LensCrop_HDome, LensCrop_VDome,  or create a custom function! 
--	func_CurrentLens = LensCrop_CustomDome
func_CurrentLens = LensCrop_HDome

----------------------------------  
----  Dome
---------------------------------
-- Decrease this value to decrease final tesselation of sphere.
DomeTesselation = 100
-- orientation is managed by omnimap_dome_wiz_ai

---------------------------------
----  Projector
---------------------------------
Point_ProjectorPostion = Vector(0,0,0)
-- orientation is managed by omnimap_dome_wiz_ai

-- ----------------------------
-- --  Audience
-- ----------------------------
AudiencePos			    = Vector( 0.00 , 0.00 ,0.00) 
-- orientation is managed by omnimap_dome_wiz_ai

-- ----------------------------
-- -- HUD 
-- ----------------------------
-- Set to false to turn off Heads Up Display
SetOmnimapParameter("displayHUD","false")





