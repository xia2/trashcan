gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rmerge.tex'
set xlabel 'Image number'
set ylabel 'Rmerge'
set xrange [0:1440] 
set yrange [0:0.20]
plot 'rmerge.dat' using 2:9 with lines lw 2 notitle
eof

cat rmerge.tex | python ../fix.py > rmerge.tmp
mv rmerge.tmp rmerge.tex

pdflatex rmerge.tex

