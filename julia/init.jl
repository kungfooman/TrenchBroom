importall Base

juliaPrint(text) = ccall( :ffi_julia_print, Void, (Cstring,), text)

imgui_log = juliaPrint # just compatibility to my console.jl

include("console.jl")
include("brushes.jl")
include("texturemanager.jl")

log(console, "Loading init.jl")

function Core.eval(code::AbstractString)
	#return eval(parse(code)) # shitty substitute for linux, because jl_parse_eval_all isnt available for some reason
	bstr = String(code)
	ret = nothing
	try
		ret = ccall(:jl_parse_eval_all, Any, (Cstring, Cstring, Csize_t), "hurr.jl", bstr, sizeof(bstr))
	catch ex
		#print("Got error: " * string(ex.error.msg) * "\n")
		return ex
	end
	return ret	
end

local ans

function eval_and_prettyprint(code::AbstractString)
	try
		ans = eval(code)
		log(console, ans)
	catch ex
		log(console, ex)
	end
end