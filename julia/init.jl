importall Base

juliaPrint(text) = ccall( :juliaPrint, Void, (Cstring,), text)

imgui_log = juliaPrint # just compatibility to my console.jl

include("console.jl")


log(console, "haiiii!\n")
log(console, "another haiiii!\n")

function Core.eval(code::AbstractString)
	return eval(parse(code))
end

local ans
function bla(code::AbstractString)
	try
		ans = eval(code)
		juliaPrint(string(ans))
	catch ex
		juliaPrint(string(ex))
	end
end