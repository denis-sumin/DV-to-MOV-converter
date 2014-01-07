function KmfToon(clip orig, int "ssw", int "ssh", int "xstren", int "xthresh", bool "cwarp", \
bool "sharpen", int "strength", float "wdepth", int "wblur", float "wthresh", float "ublur", \
int "ustren", int "urange", float "uboost", int "dstren", int "drange", float "dboost", \
int "dlimit") {

# normal params

strength = Default(strength, 255) # line darkening strength, 0-255
sharpen  = Default(sharpen, true) # sharpening on/off
cwarp    = Default(cwarp, true)   # chroma warp on/off
cwarp2   = cwarp ? 1 : 0
wdepth   = Default(wdepth, 16.0)  # warping depth, ?-?
wblur    = Default(wblur, 2)      # warping blur level, ?-?
wthresh  = Default(wthresh, 0.5)  # warping threshold, 0.0-1.0
ssw      = Default(ssw, 4)        # supersample factor horizontally, 0-inf
ssh      = Default(ssh, 4)        # supersample factor vertically, 0-inf

# advanced params

xstren   = Default(xstren, 255)   # xsharpening strength, 0-255
xthresh  = Default(xthresh, 255)  # xsharpening threshold, 0-255
ublur    = Default(ublur, 1.58)   # unsharp mask blur level, 0.0-1.58
ustren   = Default(ustren, 255)   # unsharp mask strength, 0-255
urange   = Default(urange, 115)   # unsharp mask range, 0-255
uboost   = Default(uboost, 1.0)   # unsharp mask boost, 0.0-10.0
dstren   = Default(dstren, 255)   # detail strength, 0-255
drange   = Default(drange, 64)    # detail range, 0-255
dboost   = Default(dboost, 10.0)  # detail boost, 0.0-10.0
dlimit   = Default(dlimit, 30)    # detail limiter, 0-255

ssw2 = orig.width * ssw
ssh2 = orig.height * ssh
mask = orig.Blur(ublur).Blur(ublur).Blur(ublur).Blur(ublur).Blur(ublur).Blur(ublur)
sharp = orig.YV12Subtract(mask, Y=3, U=1, V=1)
# Without the level, mergeluma wouldn't have been needed

sharp1 = orig.YV12Layer(sharp, "mul", 255, chroma=false, Y=3, V=1, U=1).Levels(0, uboost, urange, 0, 255)
sharp2 = orig.MergeLuma(sharp1)

detailmask = orig.EdgeMask(3, 255, 255, 255, "sobel", Y=3, V=1, U=1) \
.Tweak(0.0, 1.0, drange, 1.0).Levels(60, dboost, 255, 0, 255) \
.Levels(0, dboost, dlimit, 255, 0)

white = orig.Binarize(Y=-255,U=-128,V=-128)

#First more correct, but slower (not much difference for a mask, 2%) 
#linemask1 = MaskedMerge(white, detailmask, orig.Invert().levels(0,1.0,255,0,dstren)).Invert()
linemask1 = MaskedMerge(detailmask, white, orig).invert()
#Total difference is generally 5% - up to you if the difference is visible
linemask = (dstren == 255) ? linemask1 : linemask1.levels(0,1.0,255,0,dstren)

dark = MaskedMerge(orig,sharp2,linemask,Y=3,U=2,V=2)

darkyv12 = (strength == 255) ? dark : dark.levels(1,1.0,255,1,strength)

semifinal = darkyv12.BicubicResize(ssw2, ssh2, 0, 0.75).XSharpen(xstren, xthresh) \
.BicubicResize(orig.width, orig.height, 0, 0.75)
final=orig.MergeLuma(semifinal).aWarpSharp(cm=cwarp2, depth=wdepth, blurlevel=wblur, thresh=wthresh)

return sharpen ? final : darkyv12

}