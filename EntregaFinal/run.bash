#!/bin/bash
# Set the initial value
# Set the size of the array
array_size=10

# Generate an array of random values
predefined_values=()
for ((i=0; i<array_size; i++)); do
    predefined_values+=("$((RANDOM % 1000))")
done



for value in "${predefined_values[@]}"; do
    # Set the predefined value
    predefined_value="$value"

  make all t=3
(echo "$predefined_value" | t=3 ./run -b ) >> "test_$predefined_value.txt"
done