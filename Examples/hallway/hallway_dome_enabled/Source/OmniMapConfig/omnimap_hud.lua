function PositionRenderChannel_In_HudDisplay(channel, vecbl,vectl,vectr,vecbr)
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.bl.x",vecbl["x"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.bl.y",vecbl["y"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.bl.z",vecbl["z"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.br.x",vecbr["x"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.br.y",vecbr["y"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.br.z",vecbr["z"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.tl.x",vectl["x"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.tl.y",vectl["y"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.tl.z",vectl["z"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.tr.x",vectr["x"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.tr.y",vectr["y"])
			SetChannelParameter(channel,"HeadsUpDisplay_Quad.tr.z",vectr["z"])
end

-- creates an axis alligned rectangle
-- good for positioning the heads up display elements
--(TODO) i should make this create aspect ratio aware boxes...
function CreateXY_Rectangle(xmin,ymin,xscale,yscale)
	return Vector(xmin,ymin,0),
			Vector(xmin,ymin+yscale,0),
			Vector(xmin+xscale,ymin+yscale,0),
			Vector(xmin+xscale,ymin+0,0)
end


