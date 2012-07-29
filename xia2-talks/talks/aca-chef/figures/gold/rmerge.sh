gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output '1800_rmerge.tex'
set xlabel 'Image number'
set ylabel 'Rmerge'
set xrange [0:1800] 
set yrange [0:0.20]
plot '1800_rmerge.dat' using 2:9 with lines lw 2 notitle
eof

cat 1800_rmerge.tex | python ../fix.py > 1800_rmerge.tmp
mv 1800_rmerge.tmp 1800_rmerge.tex

pdflatex 1800_rmerge.tex

