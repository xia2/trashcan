gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rd.tex'
set xlabel 'Image number difference'
set ylabel 'Rd'
set xrange [0:1440] 
set yrange [0:0.3]
plot 'rd.dat' using 1:2 with lines lw 2 notitle
eof

cat rd.tex | python ../fix.py > rd.tmp
mv rd.tmp rd.tex

pdflatex rd.tex

