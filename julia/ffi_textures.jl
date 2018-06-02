type Texture
	id::Int
end

function find_texture(name)
	id = ccall( :ffi_find_texture, Int, (Cstring,), name)
	if id == -1
		# todo: when texture doesnt exist... then generate a random texture or return a default texture
		return Texture(0) # id 0 gotta be default texture for now xD
	end
	return Texture(id)
end

if false
	find_texture("concrete/white")
end