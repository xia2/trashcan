gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'patterson_height.tex'
set xlabel 'Extent of data set / $^{\circ}$'
set ylabel 'Peak height / $\sigma$'
set xrange [0:1440] 
set yrange [0:20]
plot 'patterson_height.dat' using 1:2 with lines lw 4 notitle
eof

pdflatex patterson_height.tex



