
 ##############################
 ##############################
 ##                          ##
 ##  mfToon v0.32 by mf ^^;  ##
 ##                          ##
 ## I would claim copyright, ##
 ## but as I do not give my  ##
 ## real identity, I can't.  ##
 ## So as netizen to net-    ##
 ## izen, I ask you, please  ##
 ## don't do lame stuff      ##
 ## with my script, like     ##
 ## claiming you made it.    ##
 ## Thank you.               ##
 ##                          ##
 ##############################
 ##############################
               ##
               ##
################################
################################
##                            ##
## Usage:                     ##
##                            ##
## Import("mfToon-v0.32.avs") ##
## mfToon()                   ##
##                            ##
################################
################################


function mfToon(clip orig, int "ssw", int "ssh", int "xstren", int "xthresh", bool "cwarp", \
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
sharp = Subtract(orig, mask).ConvertToYUY2()
origyuy = orig.ConvertToYUY2()
sharp2 = origyuy.MergeLuma(Layer(origyuy, sharp, "mul", 255) \
.Levels(0, uboost, urange, 0, 255)).ConvertToRGB32() 
origrgb = origyuy.ConvertToRGB32()
greymask = origrgb.Greyscale().Levels(0, 1, 255, 255, 0)

detailmask = orig.EdgeMask(0, "sobel").Tweak(0.0, 1.0, drange, 1.0) \
.Levels(60, dboost, 255, 0, 255).Levels(0, dboost, dlimit, 255, 0).GreyScale().ConvertToRGB32()
maskeddetail = Mask(detailmask, greymask)
white = BlankClip(greymask, color=$FFFFFF)
linemask = Layer(white, maskeddetail, "add", dstren).Levels(0, 1, 255, 255, 0)

maskedsharp = Mask(sharp2, linemask)
dark = Layer(origrgb, maskedsharp, "add", strength)
darkyv12 = dark.ConvertToYV12()
finaldark = orig.MergeLuma(darkyv12)

semifinal = darkyv12.BicubicResize(ssw2, ssh2, 0, 0.75).XSharpen(xstren, xthresh) \
.BicubicResize(orig.width, orig.height, 0, 0.75)
final=orig.MergeLuma(semifinal).aWarpSharp(cm=cwarp2, depth=wdepth, blurlevel=wblur, thresh=wthresh)

return sharpen ? final : finaldark

}