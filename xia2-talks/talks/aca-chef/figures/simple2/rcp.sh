gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rcp.tex'
set xlabel 'Total number of frames'
set ylabel 'Rcp'
set y2label 'Completeness'
set xrange [0:900] 
set yrange [0:0.06]
plot 'rcp.dat' using 1:10 with lines lw 4 notitle
eof

cat rcp.tex | python ../fix.py > rcp.tmp
mv rcp.tmp rcp.tex

pdflatex rcp.tex

gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rcp2.tex'
set xlabel 'Total number of frames'
set ylabel 'Rcp'
set y2label 'Completeness'
set yrange [0:0.06]
set y2range [0:1.05]
set ytics nomirror
set y2tics nomirror
set key right bottom
plot 'rcp.dat' using 1:10 with lines lw 4 title 'Rcp', \
'comp.dat' using 1:2 with lines lw 4 title 'Completeness' axis x1y2
eof

cat rcp2.tex | python ../fix.py > rcp2.tmp
mv rcp2.tmp rcp2.tex

pdflatex rcp2.tex

