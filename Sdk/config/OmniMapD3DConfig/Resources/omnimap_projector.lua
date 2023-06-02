---  Object oriented lua file for projector
---  usage 
---
---  local vec = Projector:new(Point_ProjectorPostion,Vec_ProjectorLookVec,Vec_ProjectorUpVec)   -- Create a vector
---  vec:Update()
---

ClassProjector = { } -- Create a table to hold the class methods

function ClassProjector:newdefaults() -- The constructor function
  local object = 
  {
	Point_ProjectorPostion = Vector(0,0,0),
	Vec_ProjectorLookVec   = Vector(0,1,0),
	Vec_ProjectorUpVec     = Vector(0,0,1)
  }
  setmetatable(object, {
    -- Overload the index event so that fields not present within the object are
    -- looked up in the prototype Vector table
    __index = ClassProjector
  })
  return object 
end

function ClassProjector:new(Point_ProjectorPostion , Vec_ProjectorLookVec ,Vec_ProjectorUpVec) -- The constructor function
  local object = 
  {
    Point_ProjectorPostion = VectorCopy(Point_ProjectorPostion),
    Vec_ProjectorLookVec   = VectorCopy(Vec_ProjectorLookVec),
    Vec_ProjectorUpVec     = VectorCopy(Vec_ProjectorUpVec)
  }
  setmetatable(object, {
    -- Overload the index event so that fields not present within the object are
    -- looked up in the prototype Vector table
    __index = ClassProjector
  })
  return object 
end

function ClassProjector:Update()
-- to do:
-- this should enforce orthognoality between look and up
	local LookPos = Vector_Add(self.Point_ProjectorPostion,self.Vec_ProjectorLookVec)

	SetOmnimapParameter("ProjectorPosition.pos.x",self.Point_ProjectorPostion["x"])
	SetOmnimapParameter("ProjectorPosition.pos.y",self.Point_ProjectorPostion["y"])
	SetOmnimapParameter("ProjectorPosition.pos.z",self.Point_ProjectorPostion["z"])
	SetOmnimapParameter("ProjectorPosition.lookAtpos.x",LookPos["x"])
	SetOmnimapParameter("ProjectorPosition.lookAtpos.y",LookPos["y"])
	SetOmnimapParameter("ProjectorPosition.lookAtpos.z",LookPos["z"])
	SetOmnimapParameter("ProjectorPosition.headsUp.x",self.Vec_ProjectorUpVec["x"])
	SetOmnimapParameter("ProjectorPosition.headsUp.y",self.Vec_ProjectorUpVec["y"])
	SetOmnimapParameter("ProjectorPosition.headsUp.z",self.Vec_ProjectorUpVec["z"])


--PrintVector("........testing........Up",			self.Vec_ProjectorUpVec)
--PrintVector("........testing........Point_ProjectorPostion",	self.Point_ProjectorPostion)
--PrintVector("........testing........LookPos",LookPos)


end



