gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'comp.tex'
set xlabel 'Image number'
set ylabel 'Completeness'
set xrange [1:1440]
set yrange [0:1.05]
plot 'comp.dat' using 1:2 with lines notitle
eof

pdflatex comp.tex

