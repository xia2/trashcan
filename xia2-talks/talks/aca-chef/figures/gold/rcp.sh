gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output '1800_rcp2.tex'
set xlabel 'Total number of frames'
set ylabel 'Rcp'
set y2label 'Completeness'
set yrange [0:0.12]
set y2range [0:1.05]
set xrange [0:1800]
set ytics nomirror
set y2tics nomirror
set key right bottom
plot '1800_rcp.dat' using 1:10 with lines title 'Rcp', \
'1800_completeness.dat' using 1:2 with lines title 'Completeness' axis x1y2, \
'1800_completeness.dat' using 1:5 with lines title 'Anomalous' axis x1y2
eof

cat 1800_rcp2.tex | python ../fix.py > 1800_rcp2.tmp
mv 1800_rcp2.tmp 1800_rcp2.tex

pdflatex 1800_rcp2.tex

