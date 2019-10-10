# Plots colored Graphs 
# 
# Run this file from console: $ gnuplot "filename" # add '-p' to keep graphs opened
# Run this file from opened gnuplot console > load "filename"
# 

set yrange [-0.1:1.1]
set xlabel 'Cycle / 1'
set ylabel 'Color channel / 1'
plot \
    "PlotData.txt" using 1:2 with lines, \
    "PlotData.txt" using 1:3 with lines