---  Object oriented lua file for Audience
---  usage 
---
---  local vec = Audience:new(Point_AudiencePostion,Vec_AudienceLookVec,Vec_AudienceUpVec)   -- Create a vector
---  vec:Update()
---

ClassAudience = { } -- Create a table to hold the class methods

function ClassAudience:newdefaults() -- The constructor function
  local object = 
  {
	Point_AudiencePostion = Vector(0,0,0),
	Vec_AudienceLookVec   = Vector(0,1,0),
	Vec_AudienceUpVec     = Vector(0,0,1)
  }
  setmetatable(object, {
    -- Overload the index event so that fields not present within the object are
    -- looked up in the prototype Vector table
    __index = ClassAudience
  })
  return object 
end

function ClassAudience:new(Point_AudiencePostion , Vec_AudienceLookVec ,Vec_AudienceUpVec) -- The constructor function
  local object = 
  {
    Point_AudiencePostion = VectorCopy(Point_AudiencePostion),
    Vec_AudienceLookVec   = VectorCopy(Vec_AudienceLookVec),
    Vec_AudienceUpVec     = VectorCopy(Vec_AudienceUpVec)
  }
  setmetatable(object, {
    -- Overload the index event so that fields not present within the object are
    -- looked up in the prototype Vector table
    __index = ClassAudience
  })
  return object 
end

function ClassAudience:Update()
-- to do:
-- this should enforce orthognoality between look and up
	local LookPos = Vector_Add(self.Point_AudiencePostion,self.Vec_AudienceLookVec)

	SetOmnimapParameter("AudiencePosition.pos.x",self.Point_AudiencePostion["x"])
	SetOmnimapParameter("AudiencePosition.pos.y",self.Point_AudiencePostion["y"])
	SetOmnimapParameter("AudiencePosition.pos.z",self.Point_AudiencePostion["z"])
	SetOmnimapParameter("AudiencePosition.lookAtpos.x",LookPos["x"])
	SetOmnimapParameter("AudiencePosition.lookAtpos.y",LookPos["y"])
	SetOmnimapParameter("AudiencePosition.lookAtpos.z",LookPos["z"])
	SetOmnimapParameter("AudiencePosition.headsUp.x",self.Vec_AudienceUpVec["x"])
	SetOmnimapParameter("AudiencePosition.headsUp.y",self.Vec_AudienceUpVec["y"])
	SetOmnimapParameter("AudiencePosition.headsUp.z",self.Vec_AudienceUpVec["z"])

end



