# coding=utf-8
import argparse
import errno
import os
import re
import shutil
import subprocess
import sys
from subprocess import PIPE, Popen, STDOUT


def ensure_dir(path):
    """
    os.path.makedirs without EEXIST.
    Taken from pip.utils
    """
    try:
        os.makedirs(path)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise


def touch(fname, times=None):
    with open(fname, 'a'):
        os.utime(fname, times)


def main():
    usage = 'Usage: %prog [options]'
    parser = argparse.ArgumentParser(usage=usage)
    parser.add_argument(
        '--audio-channel',
        choices=['both', 'left', 'right'],
        default='both',
        dest='audio_channel',
        help='Audio channel to use (both, left, right)',
    )
    parser.add_argument(
        '--dir',
        default='.',
        dest='dir',
        help='Path to folder with DV/AVI files',
        type=str,
    )
    parser.add_argument(
        '--save-temp',
        action='store_true',
        dest='save_temp_files',
        help='If chosen, temp files are not deleted (for debugging)',
    )
    args = parser.parse_args()
    working_dir = os.path.normpath(args.dir)

    script_path = os.path.dirname(os.path.abspath(__file__))
    ffmpeg_path = os.path.join(script_path, 'bin', 'ffmpeg.exe')
    x264_path = os.path.join(script_path, 'bin', 'x264.exe')

    temp_avs_path = os.path.join(working_dir, 'temp.avs')

    with open(temp_avs_path, 'w') as avs_file:
        avs_dll_path = os.path.join(
            script_path, 'bin', 'AviSynth_plugins', 'avss.dll')
        my_deinterlace_path = os.path.join(
            script_path, 'bin', 'deinterlacers', 'my_deinterlace.avs')
        avs_file.write(
            f'LoadPlugin("{avs_dll_path}")\n'
            f'import("{my_deinterlace_path}")\n'
            f'c = DSS2("temp")\n'
            f'return c.my_deinterlace()\n'
        )

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

            ensure_dir(mov_folder)
            ensure_dir(avi_folder)
            ensure_dir(x264_folder)
            ensure_dir(audio_folder)
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

            cmd = f'{ffmpeg_path} -i "{input_video}"'
            p = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT)
            output = p.stdout.read().decode()
            sar_match = re.search(r'SAR (?P<sar>\d+:\d+)', output)
            if sar_match:
                sar = sar_match.groupdict()['sar']
                sar = '--sar ' + sar
            else:
                sar = ''

            input_audio_stream_id = ''
            input_acodec = ''
            if args.audio_channel in ('left', 'right'):
                audio_match = re.search(
                    r'Stream #(?P<astream>\d+:\d+): Audio: (?P<acodec>\w+)',
                    output)
                if audio_match:
                    audio_match_dict = audio_match.groupdict()
                    input_audio_stream_id = audio_match_dict['astream']
                    input_acodec = audio_match_dict['acodec']
                    if not input_acodec.startswith('pcm'):
                        print('Audio codec is not pcm_*. Only pcm codecs are '
                              'supported when manipulating audio channels.')
                        sys.exit(1)
                else:
                    print('Failed to extract audio codec information; '
                          'it is necessary when manipulating audio channels.')
                    sys.exit(1)

            shutil.move(input_video, temp_video_path)
            touch(input_video + '.lock')
            cmd = f'{x264_path} --crf 15 --profile high --preset slow {sar} ' \
                  f'-o "{temp_video_without_audio}" "{temp_avs_path}"'
            subprocess.call(cmd)
            os.remove(input_video + '.lock')
            shutil.move(temp_video_path, input_video)

            if args.audio_channel in ('left', 'right'):
                if input_acodec and input_audio_stream_id:
                    if args.audio_channel == 'left':
                        achannel = 0
                    elif args.audio_channel == 'right':
                        achannel = 1
                    else:
                        raise RuntimeError('Failed to set achannel')
                    astream = input_audio_stream_id.replace(':', '.')
                    audio_options = f'-map_channel {astream}.{achannel} ' \
                                    f'-c:a {input_acodec}'
                else:
                    raise RuntimeError(
                        'input_acodec or input_audio_stream_id was not set')
            else:
                audio_options = '-c:a copy'

            cmd = f'{ffmpeg_path} -i "{input_video}" {audio_options} ' \
                  f'"{temp_audio_uncompressed}"'
            subprocess.call(cmd)

            cmd = f'{ffmpeg_path} ' \
                  f'-i "{temp_video_without_audio}" ' \
                  f'-i "{temp_audio_uncompressed}" ' \
                  f'-c:v copy -c:a copy "{output_video}"'
            subprocess.call(cmd)

            shutil.move(input_video, avi_folder)
            if not args.save_temp_files:
                os.remove(temp_video_without_audio)
                os.remove(temp_audio_uncompressed)
                shutil.rmtree(x264_folder)
                shutil.rmtree(audio_folder)

    os.remove(temp_avs_path)


if __name__ == '__main__':
    main()
