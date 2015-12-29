--------------------------------
--(%$$$%) FILE IO FUNCTIONS
--------------------------------
-- not totally tested yet...

--(%%)
function ReadEntireFileToString(filename)
	local configfile = io.open (filename,"r")
	local configstring = configfile:read("*a")
	io.close(configfile)
	return configstring 
end

--(%%)
function SaveStringToFile(filename, strContent)
	local thefile = io.open (filename,"w+")
	thefile:write( strContent )
	io.close(thefile)
end

--(%%)
function IsFileExactlyThisString(filename,strString)
	local newstring = ReadEntireFileToString(filename)
	return (strString==newstring)
end

--(%%)
function SaveStringToFileOnlyOnChange(filename, strContent)
	if( not IsFileExactlyThisString(filename,strContent) )
		then 
			SaveStringToFile( filename,strContent)
		end
end


--------------------------------
--(%$$$%) Shader Config functions
--------------------------------

--(%%) Creates the shader config specification
--(%%)(n) This uses #defines to pass config data to the shader
--(%%)(n) it might be the place to set defines that specify !theta and f-theta
function CreateShaderConfigString(NChannels)
	return "#define MaxChannels " .. NChannels
end

--(%%)  This function saves a set of defines that are loaded in the shader
--(%%)  NChannels is the number of render channels
--(%%)  It will not overwrite the config file if it hasn't changed.
function SaveShaderConfig(shaderconfigname,NChannels)
	SaveStringToFileOnlyOnChange(shaderconfigname,CreateShaderConfigString(NChannels))
end


--(%%) bool to integer
function BtoI(bval)
	if(bval) then return 1 else return 0 end
end
