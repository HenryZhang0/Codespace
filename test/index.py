from flask import Flask, request, jsonify, render_template
import os
from werkzeug.utils import secure_filename
from app import process_audio

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = './uploads'
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # Limit upload size to 16MB

# Ensure the upload folder exists
os.makedirs(app.config['UPLOAD_FOLDER'], exist_ok=True)

@app.route('/')
def upload_form():
    return '''
    <!doctype html>
    <title>Upload an Audio File</title>
    <h1>Upload an Audio File for Chord Extraction</h1>
    <form method=post enctype=multipart/form-data action="/upload">
      <input type=file name=file>
      <input type=submit value=Upload>
    </form>
    '''

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return "No file part", 400
    file = request.files['file']
    if file.filename == '':
        return "No selected file", 400
    if file:
        filename = secure_filename(file.filename)
        file_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(file_path)
        
        # Process the audio file
        chords = process_audio(str(file_path))

        # Return the results as JSON
        return jsonify(chords)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=3000, debug=True)