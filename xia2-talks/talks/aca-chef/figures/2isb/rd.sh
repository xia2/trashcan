gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'rd.tex'
set xlabel 'Image number difference'
set ylabel 'Rd'
set xrange [0:90] 
set yrange [0:0.3]
plot 'rd1.dat' using 1:2 with lines title 'Inflection', \
'rd2.dat' using 1:2 with lines title 'Low remote', \
'rd3.dat' using 1:2 with lines title 'Peak'

eof

cat rd.tex | python ../fix.py > rd.tmp
mv rd.tmp rd.tex

pdflatex rd.tex

