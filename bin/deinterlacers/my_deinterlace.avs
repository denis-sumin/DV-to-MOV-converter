LoadPlugin("masktools-v2.0a48\mt_masktools-26.dll")
LoadPlugin("RemoveGrain-1.0\RemoveGrainSSE3.dll")
LoadPlugin("RemoveGrain-1.0\RepairSSE3.dll")
LoadPlugin("mvtools-v2.5.11.3\mvtools2.dll")
LoadPlugin("VerticalCleaner\VerticalCleanerSSE3.dll")
LoadPlugin("nnedi2\nnedi2.dll")
import ("TempGaussMC_beta2.avsi")

function my_deinterlace (clip c)
{
	# a = c.convertToYV12().nnedi3(field=2).subtitle("nnedi")
	b = c.AssumeBFF().convertToYV12().TempGaussMC_beta2(tr2=3,sharpness=1.0,Sbb=2,SLrad= 2,Sovs=2,EdiMode="NNEDI2") # .subtitle("TempGaussMC")
	# d = c.convertToYV12().TempGaussMC_beta2(tr0=1,tr1=1,tr2=0,rep0=0,rep1=0, rep2=0,sharpness=0,Smode=0,SLmode=0,Sbb=0,SVthin=0.0,EdiMode="NNEDI3",pelsearch=1).subtitle("TempGaussMC_lite")
	return b
	# return interleave(a,b)
}