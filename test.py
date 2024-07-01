# import autochord
import subprocess

def runCommand(command):
    #command = ['ls', '-l']  # Example command to list directory contents in long format
    try:
        # Run the command
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        # Output the result
        print("Output:")
        print(result.stdout)
        return result.stdout
    except subprocess.CalledProcessError as e:
        print("Error:")
        print(e.stderr)

def split(filename):
    print("split")
    #spleeter separate -o audio_output -p spleeter:5stems audio_example.mp3
    command = ['spleeter', 'separate', '-o', 'audio_output', '-p', 'spleeter:5stems', filename]
    runCommand(command)

def chords():
    autochord.recognize('lauv.mp3', lab_fn='lauv_chords.lab')
    
def lyrics(songTitle):
    #syncedlyrics "rap god"
    command = ["syncedlyrics", songTitle, "-o", songTitle+".lrc"]
    runCommand(command)

# lyrics("rap god")
chords()
# split('audio_example.mp3')