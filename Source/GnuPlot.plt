# Plots colored Graphs 
# 
# Run this file from console: $ gnuplot "filename" # add '-p' to keep graphs opened
# Run this file from opened gnuplot console > load "filename"
# 

## Function declaration
RGB(R,G,B) =  int(255*R) * 2**16 + int(255*G) * 2**8  + int(255*B)
Abs(r,g,b) = (r**2 + g**2 + b**2)**(0.5)
## 

## Plot-frame settings
set yrange [-0.1:1.1]
set xlabel 'Cycle / 1'
set ylabel 'Color channel / 1'
set rmargin 5; set lmargin 5; set bmargin 2

set title "LED-Color-Effect"

set style line 2 lt 2 lc rgb "blue" lw 1
set style line 3 lt 2 lc rgb "green" lw 1
set style line 4 lt 2 lc rgb "red" lw 1

## Data is set in order: 1:num 2:B 3:G 4:R (5:W)
plot \
    "PlotData.txt" using 1:(Abs($4,$3,$2)):(RGB($4,$3,$2)) with points pt 7 ps 2 lc rgb variable title 'Effect x', \
    "PlotData.txt" using 1:4 with line ls 4 notitle, \
    "PlotData.txt" using 1:3 with line ls 3 notitle, \
    "PlotData.txt" using 1:2 with line ls 2 notitle

