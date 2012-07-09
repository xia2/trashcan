gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output '1800_rd.tex'
set xlabel 'Image number difference'
set ylabel 'Rd'
set xrange [0:1800] 
set yrange [0:0.4]
plot '1800_rd.dat' using 1:2 with lines notitle
eof

cat 1800_rd.tex | python ../fix.py > 1800_rd.tmp
mv 1800_rd.tmp 1800_rd.tex

pdflatex 1800_rd.tex

