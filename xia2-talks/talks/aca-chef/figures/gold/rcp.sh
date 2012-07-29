gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output '1800_rcp2.tex'
set xlabel 'Extent of data set / $^{\circ}$'
set ylabel 'Rcp'
set y2label 'Completeness'
set yrange [0:0.12]
set y2range [0:0.7]
set xrange [0:360]
set ytics nomirror
set y2tics nomirror
set key right bottom
plot '1800_rcp.dat' using (0.2 * \$1):10 with lines lw 4 title 'Rcp', \
'1800_completeness.dat' using (0.2 * \$1):2 with lines lw 4 title 'Completeness' axis x1y2, \
'1800_completeness.dat' using (0.2 * \$1):5 with lines lw 4 title 'Anomalous' axis x1y2
eof

cat 1800_rcp2.tex | python ../fix.py > 1800_rcp2.tmp
mv 1800_rcp2.tmp 1800_rcp2.tex

pdflatex 1800_rcp2.tex

