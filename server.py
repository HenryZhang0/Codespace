import autochord
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
    autochord.recognize('other.wav', lab_fn='chords.lab')


def lyrics(songTitle):
    #syncedlyrics "rap god"
    command = ["syncedlyrics", songTitle, "-o"]
    runCommand(command)

    file_path = 'rap god.lrc'
    with open(file_path, 'r') as file:
        file_content = file.read()
    return file_content


from flask import Flask, request, redirect, url_for, flash, send_from_directory
import os


app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = 'uploads/'
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # 16 MB max upload size

# Ensure the upload directory exists
if not os.path.exists(app.config['UPLOAD_FOLDER']):
    os.makedirs(app.config['UPLOAD_FOLDER'])

@app.route('/test')
def test():
    print("test")
    split()
    return

@app.route('/')
def upload_form():
    return '''
    <!doctype html>
    <title>Upload an MP3 File</title>
    <h1>Upload an MP3 File</h1>
    <form method="post" enctype="multipart/form-data" action="/upload">
        <input type="file" name="file" accept=".mp3">
        <input type="submit" value="Upload">
    </form>
    '''

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        flash('No file part')
        return redirect(request.url)
    
    file = request.files['file']
    
    if file.filename == '':
        flash('No selected file')
        return redirect(request.url)
    
    if file and file.filename.endswith('.mp3'):
        filename = file.filename
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        
        split(filename)
        return (lyrics('rap god'))
    else:
        return 'Invalid file type. Please upload an MP3 file.'

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

if __name__ == '__main__':
    app.run(debug=True)