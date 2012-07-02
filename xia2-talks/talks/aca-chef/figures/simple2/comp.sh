gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'comp.tex'
set xlabel 'Image number'
set ylabel 'Completeness'
plot 'comp.dat' using 1:2 with lines notitle
eof

pdflatex comp.tex

