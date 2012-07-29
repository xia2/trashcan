gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'comp.tex'
set xlabel 'Total number of frames'
set ylabel 'Completeness'
set xrange [0:900]
set yrange [0:1.05]
plot 'comp.dat' using 1:2 with lines lw 4 notitle
eof

pdflatex comp.tex

