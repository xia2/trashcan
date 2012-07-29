gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rcp2.tex'
set xlabel 'Experiment time / s'
set ylabel 'Rcp'
set y2label 'Completeness'
set xrange [0:1350]
set yrange [0:0.08]
set y2range [0:1.05]
set ytics nomirror
set y2tics nomirror
set key right bottom
plot 'rcp.dat' using 1:10 with lines lw 4 title 'Rcp', \
'comp1d.dat' using 1:2 with lines lw 4 title 'Inflection' axis x1y2, \
'comp2d.dat' using 1:2 with lines lw 4 title 'Low remote' axis x1y2, \
'comp3d.dat' using 1:2 with lines lw 4 title 'Peak' axis x1y2
eof

cat rcp2.tex | python ../fix.py > rcp2.tmp
mv rcp2.tmp rcp2.tex

pdflatex rcp2.tex

