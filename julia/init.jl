juliaPrint(text) = ccall( :juliaPrint, Void, (Cstring,), text)

juliaPrint("haiiii!")

local ans

function Core.eval(code::AbstractString)
	return eval(parse(code))
end

function bla(code::AbstractString)
	try
		ans = eval(code)
		juliaPrint(string(ans))
	catch ex
		juliaPrint(string(ex))
	end
end