gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rcp2.tex'
set xlabel 'Extent of data set / $^{\circ}$'
set ylabel 'Rcp'
set y2label 'Completeness'
set yrange [0:0.08]
set y2range [0:1.05]
set xrange [0:1440]
set ytics nomirror
set y2tics nomirror
set key right bottom
plot 'rcp.dat' using 1:10 with lines title 'Rcp', \
'comp.dat' using 1:2 with lines title 'Completeness' axis x1y2, \
'comp.dat' using 1:5 with lines title 'Anomalous' axis x1y2
eof

cat rcp2.tex | python ../fix.py > rcp2.tmp
mv rcp2.tmp rcp2.tex

pdflatex rcp2.tex

