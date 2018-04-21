DV-to-MOV-converter
=======================

Python script for high-quality AVI (DV) to MOV recoding based on ffmpeg, x264, 
avisynth and avs-deinterlacer.

Usage (assuming you have your `*.dv` or `*.avi` files in `D:\Video` and you 
downloaded the tool to `D:\recoder`):

* call recode_video.py from D:\Video:
```bash
D:\Video> D:\recoder\recode_video.py
```

OR

* call the script with `--dir` option:
```bash
D:\> D:\recoder\recode_video.py --dir "D:\video"
```

`--audio-channel` option (`both`, `left` or `right`; `both` is used as a default) 
enables using chosen audio channels in the output.
If `left` or `right` is chosen, the output audio is mono.
