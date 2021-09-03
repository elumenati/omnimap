----------------------------------------------------------------------------
----                        Dome Wiz AI
----------------------------------------------------------------------------
--
--
-- Dome wiz AI, sets 
-- OmniMap parameters
--    The dome radius in meters
--    Render Target (One of RT_FRAME_BUFFER_OBJECT, RT_PBUFFER, or RT_BACK_BUFFER)
--    Renderer (Either "OGL" or "D3D")
--    DisplayHUD (true to draw heads up display of unprojected channels, false to not display HUD)
GLOBAL_domeRadius = 6
GLOBAL_renderTarget = RT_FRAME_BUFFER_OBJECT
GLOBAL_renderer = "OGL"
GLOBAL_DisplayHUD = false

-- Dome wiz AI, sets 
-- OmniMap Channel Parameters
-- ConsolidateCameraIntoProjectionMatrix - By default, OmniMap puts its viewing matrix offset
--    and its projection matrix offset into the projection matrix.  If false, the viewing matrix
--    is multiplied into the modelview matrix.
-- UseOmniMapMatrix - To ignore the OmniMapMatrix, set this option to false.  It is reccommend that
--    applications always use the OmniMap matrix, because it changes based on the dome configuration
--    being used.
-- multiSampleQuality - This is supported in DirectX only.  It is the multisample quality of the
--    offscreen render buffer.  See the DirectX documentation for the method CreateRenderTarget
--    for more details on multisampling, and the meaning of this value.
GLOBAL_ConsolidateCameraIntoProjectionMatrix = true
GLOBAL_UseOmniMapMatrix = true
GLOBAL_multiSampleQuality = 0.0
GLOBAL_channelResX = 1024
GLOBAL_channelResY = 1024
GLOBAL_clipNear = .1
GLOBAL_clipFar = 1000.0

dofile(AddSupportDirectory("omnimap_configure_domes.lua"))

