#!/bin/bash

# Input text file
input_file="test_7.txt"

# Output .dot file
output_file="tree_graph.dot"

# Initialize the output .dot file
echo "digraph Tree {" > "$output_file"

# Read the input text file line by line
while IFS= read -r line
do
  # Check if the line starts with "Position in node:"
  if [[ $line == "Position in node:"* ]]; then
    position=$(echo "$line" | awk -F":" '{print $2}' | awk '{$1=$1};1')
    echo "  $position [label=\"Position: $position\"];" >> "$output_file"
  fi

  # Check if the line starts with "Number of Records:"
  if [[ $line == "Number of Records:"* ]]; then
    records=$(echo "$line" | awk -F":" '{print $2}' | awk '{$1=$1};1')
    echo "  $position -> { " >> "$output_file"
  fi

  # Check if the line starts with "Keys:"
  if [[ $line == "Keys:"* ]]; then
    keys=$(echo "$line" | awk -F":" '{print $2}' | awk '{$1=$1};1')
    keys_arr=($keys)
    for key in "${keys_arr[@]}"; do
      echo "    $key;" >> "$output_file"
    done
  fi

  # Check if the line starts with "Links:"
  if [[ $line == "Links:"* ]]; then
    links=$(echo "$line" | awk -F":" '{print $2}' | awk '{$1=$1};1')
    links_arr=($links)
    for link in "${links_arr[@]}"; do
      echo "  } -> $link;" >> "$output_file"
    done
  fi
done < "$input_file"

# Close the .dot file
echo "}" >> "$output_file"
