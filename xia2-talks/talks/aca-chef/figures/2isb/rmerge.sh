gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rmerge.tex'
set xlabel 'Image number'
set ylabel 'Rmerge'
set xrange [0:90] 
set yrange [0:0.25]
plot 'rmerge1.dat' using 2:9 with lines lw 2 title 'Inflection', \
'rmerge2.dat' using 1:9 with lines lw 2 title 'Low remote', \
'rmerge3.dat' using 1:9 with lines lw 2 title 'Peak'

eof

cat rmerge.tex | python ../fix.py > rmerge.tmp
mv rmerge.tmp rmerge.tex

pdflatex rmerge.tex

