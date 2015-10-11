#!/bin/bash

echo "Sampling..."

# execute python script to sample and save data
# number of samples is given by first argument of this script
echo $1 | ./read.py 
# instruct gnuplot to plot sampled data
gnuplot -persist -e "set xlabel 'Sample number'; set yrange [-100:100]; set ylabel 'ACC reading'; set grid; plot 'data.dat' using 1:2 with lines, 'data2.dat' using 1:2 with lines"


echo "Done!"