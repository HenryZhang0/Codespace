#!/bin/bash

# Define the command and its arguments
COMMAND="spleeter separate -o audio_output -p spleeter:5stems audio_example.mp3"

# Run the command
$COMMAND

# Check if the command was successful
if [ $? -eq 0 ]; then
  echo "Spleeter separation completed successfully."
else
  echo "An error occurred during spleeter separation." >&2
fi
