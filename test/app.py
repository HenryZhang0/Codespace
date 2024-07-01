from chord_extractor.extractors import Chordino
from chord_extractor import clear_conversion_cache

def process_audio(file_path):
    chordino = Chordino()
    clear_conversion_cache()
    
    # Extract chords from the file
    results = chordino.extract(file_path)
    
    # Process the results
    extracted_chords = []
    for s in results:
        extracted_chords.append({'chord': s.chord, 'timestamp': s.timestamp})
    
    return extracted_chords

if __name__ == "__main__":
    file_path = "./piano.wav"
    chords = process_audio(file_path)
    print(chords)
