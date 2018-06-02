importall Base
using Glob

include("juliaPrint.jl")
include("console.jl")

log(console, "Loading init.jl")

include("filename.jl")
include("brushes.jl")
include("texturemanager.jl")
include("TextureCollection.jl")
include("TextureBrowserView.jl")
include("allTextures.jl")
include("eval.jl")
include("eval_and_prettyprint.jl")
include("removeExtension.jl")
include("libwebgame.jl")
include("php.jl")
