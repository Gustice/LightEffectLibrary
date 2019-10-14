# Plot script to show all generated graphs

## Plot-frame settings
#set style line 2 lt 2 lc rgb "blue" lw 1
#set style line 3 lt 2 lc rgb "green" lw 1
#set style line 4 lt 2 lc rgb "red" lw 1

set multiplot layout 2, 1 title "Pulses and slopes" font ",12"
set yrange [-5:270]
set ylabel 'intensity / 1'
#set tmargin 2
set title "Pulses"
plot    "Output/gau8_offsetPulse_Plot.txt" using 1:2 with lines title 'offset pulse'  ,\
        "Output/gau8_fullPulse_Plot.txt" using 1:2 with lines title 'full pulse'  
#
set title "Slopes"
plot    "Output/gau8_offsetSlope_Plot.txt" using 1:2 with lines title 'offset slope' ,\
        "Output/gau8_fullSlope_Plot.txt" using 1:2 with lines title 'full slope' ,\
        "Output/gau8_initSlope_Plot.txt" using 1:2 with lines title 'initial slope' 
unset multiplot
 
