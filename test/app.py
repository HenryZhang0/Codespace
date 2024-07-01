from chord_extractor.extractors import Chordino

from chord_extractor import clear_conversion_cache, LabelledChordSequence

files_to_extract_from = [
  "./piano.wav"
]

def save_to_db_cb(results):
    print(results.sequence)
    
    for s in results.sequence:
        # print(s)
        print(s.chord, s.timestamp)
    # Every time one of the files has had chords extracted, receive the chords here 
    # along with the name of the original file and then run some logic here, e.g. to 
    # save the latest data to DB

chordino = Chordino()

# Optionally clear cache of file conversions (e.g. wav files that have been converted from midi)
clear_conversion_cache()

# Run bulk extraction
res = chordino.extract_many(files_to_extract_from, callback=save_to_db_cb, num_extractors=2,
                            num_preprocessors=2, max_files_in_cache=10, stop_on_error=False)