# -*- coding: utf-8 -*-
from __future__ import print_function

import locale
import os
import re
import shutil
import subprocess
import sys
from optparse import OptionParser
from subprocess import Popen, PIPE, STDOUT


def touch(fname, times=None):
    with file(fname, 'a'):
        os.utime(fname, times)


def main():
    usage = 'Usage: %prog [options]'
    parser = OptionParser(usage=usage)
    parser.add_option(
        '--dir',
        default='.',
        action='store',
        type='string',
        dest='dir',
        help='Path to folder with DV/AVI files',
    )

    options, args = parser.parse_args()

    working_dir = u'' + os.path.normpath(unicode(options.dir.decode(sys.getfilesystemencoding())))

    PATH = os.path.dirname(os.path.abspath(__file__))

    temp_avs_path = os.path.join(working_dir, 'temp.avs')

    # shutil.copyfile(os.path.join(PATH,'template_temp_file_dss2_tempgaussmc.avs'),'temp.avs')
    avs_file = open(temp_avs_path, 'w')
    avs_file.write(r'LoadPlugin("' + PATH + r'\bin\AviSynth_plugins\avss.dll")' + '\n')
    avs_file.write(r'import("' + PATH + r'\bin\deinterlacers\my_deinterlace.avs")' + '\n')
    avs_file.write(r'c = DSS2("temp")' + '\n')
    avs_file.write(r'return c.my_deinterlace()')
    avs_file.close()

    mov_folder = os.path.join(working_dir, 'mov')
    avi_folder = os.path.join(working_dir, 'avi')
    x264_folder = os.path.join(working_dir, 'x264')
    audio_folder = os.path.join(working_dir, 'audio')

    # Used temp paths
    temp_video_without_audio = os.path.join(x264_folder, 'video.mp4')
    temp_audio_uncompressed = os.path.join(audio_folder, 'audio.wav')

    for source_filename in os.listdir(working_dir):
        if source_filename.endswith('.avi') or source_filename.endswith('.dv'):
            print(source_filename)

            if not os.path.exists(mov_folder):
                os.mkdir(mov_folder)
            if not os.path.exists(avi_folder):
                os.mkdir(avi_folder)
            if not os.path.exists(x264_folder):
                os.mkdir(x264_folder)
            if not os.path.exists(audio_folder):
                os.mkdir(audio_folder)
            try:
                os.remove(temp_video_without_audio)
            except OSError:
                pass
            try:
                os.remove(temp_audio_uncompressed)
            except OSError:
                pass

            filename, extension = os.path.splitext(source_filename)
            input_video = os.path.join(working_dir, source_filename)
            output_video = os.path.join(working_dir, 'mov', filename + '.mov')
            temp_video_path = os.path.join(working_dir, 'temp')

            cmd = u'' + os.path.join(PATH, 'bin', 'ffmpeg.exe') + ' -i "' + input_video + '"'
            p = Popen(cmd.encode(locale.getpreferredencoding()), shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
            output = p.stdout.read()
            pattern = re.compile(r'SAR \d+:\d+')
            match = pattern.findall(output)
            sar = r''
            if len(match) == 1:
                sar = match[0].replace(u'SAR ', u'').decode('utf8')
                sar = r'--sar ' + sar

            shutil.move(input_video, temp_video_path)
            touch(input_video + '.lock')
            cmd = r'' + os.path.join(PATH, 'bin', 'x264.exe') + \
                  r' --crf 15 --profile high --preset slow ' + sar + \
                  r' -o "' + temp_video_without_audio + r'" "' + temp_avs_path + r'"'
            subprocess.call(cmd.encode(locale.getpreferredencoding()))
            os.remove(input_video+'.lock')
            shutil.move(temp_video_path, input_video)

            cmd = r'' + os.path.join(PATH, 'bin', 'ffmpeg.exe') + \
                  r' -i "' + input_video + r'" -c:a copy "' + temp_audio_uncompressed + r'"'
            subprocess.call(cmd.encode(locale.getpreferredencoding()))

            cmd = u'' + os.path.join(PATH, 'bin', 'ffmpeg.exe') + \
                  u' -i "' + temp_video_without_audio + u'" -i "' + temp_audio_uncompressed + \
                  u'" -c:v copy -c:a copy "' + output_video + u'"'
            subprocess.call(cmd.encode(locale.getpreferredencoding()))

            os.remove(temp_video_without_audio)
            os.remove(temp_audio_uncompressed)
            shutil.move(input_video, avi_folder)

            shutil.rmtree(x264_folder)
            shutil.rmtree(audio_folder)

    os.remove(temp_avs_path)


if __name__ == '__main__':
    main()
