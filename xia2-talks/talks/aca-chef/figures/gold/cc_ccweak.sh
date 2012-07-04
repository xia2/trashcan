gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'cc_ccweak.tex'
set xrange [0:40]
set yrange [0:25]
set xlabel 'CC/\%'
set ylabel 'CCweak/\%'
plot '900_cc_ccweak.dat' using 2:3 with points title '900 frames', \
'1800_cc_ccweak.dat' using 2:3 with points title '1800 frames'
eof

pdflatex cc_ccweak.tex

