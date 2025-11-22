-- This is lua global variable
some_var = 7 * 6

-- init table
config = {
	title = "2D Game Engine",
	fullscreen = false,
	resolution = {
		width = 1280,
		height = 720
	}
}

-- function
function fac(n)
	if n == 1 then
		return 1
	end
	return n * fac(n - 1)
end
