---- IMPORTANT NOTE
-- When copying vectors and matricies, be sure to use the 
-- Matrix3x3_CopyMatrix( ) function or VectorCopy() function
-- Otherwise, operations done to the new vector happen to the old vector also...



math.randomseed(os.date("%d%H%M%S")) -- initalize the random seed here...


function DegreesToRadians(angleDegrees)
	return angleDegrees  /57.2957795
end



function FeetToMeters(n) return n*0.3048 end
function InchesToFeet(n) return n*(1.0/12.0) end
function InchesToMeters(n) return FeetToMeters(InchesToFeet(n)) end
function FeetToInches(n) return n*12.0 end
function MetersToFeet(n) return n/0.3048 end
function MetersToInches(n) return FeetToInches(MetersToFeet(n)) end


---------------------------------------------------
--- Setting Up the base Units....
--- can be skiped if you dont care about units
-- 1 baseUnit = 1 DomeRadius
function SetupUnitConversionsByDomeRadiusInInches(DomeRad_in_inches)
	domeradius_in_inches = DomeRad_in_inches
	inches  = 1.0/ domeradius_in_inches
	feet	= FeetToInches(1.0)/ domeradius_in_inches
	meters  = MetersToInches(1.0)/ domeradius_in_inches
	DomeRadis =1.0
	DomeRadius_unitless = 1.0

	inch = inches  
	foot = feet	
	meter = meters

end
---------------------------------------------------

