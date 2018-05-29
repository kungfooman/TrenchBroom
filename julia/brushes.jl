type Brush
	id::Int
end

selectedBrushesCount() = ccall( :selectedBrushesCount, Int, ())

ffi_free(ptr) = ccall(:ffi_free, Void, (Ptr{Void},), ptr)


# rotate( selectedBrushes()[1], 0.3, 0, 0)
rotate(brush::Brush, roll, pitch, yaw) = ccall( :ffi_brush_rotate, Bool, (Int32, Float32, Float32, Float32), brush.id, roll, pitch, yaw)


function selectedBrushes()::Array{Brush}
	ptrlist = ccall( :selectedBrushes, Ptr{Int}, ())
	n = selectedBrushesCount()
	brushes = Array{Brush}(n)
	for i in 1:n
		brushes[i] = Brush( unsafe_load(ptrlist, i) )
	end
	ffi_free(ptrlist)
	return brushes
end

