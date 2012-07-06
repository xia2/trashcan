gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'comp.tex'
set xlabel 'Image number'
set ylabel 'Anomalous completeness'
set xrange [0:90] 
set yrange [0:1]
plot 'comp1.dat' using 1:5 with lines title 'Inflection', \
'comp2.dat' using 1:5 with lines title 'Low remote', \
'comp3.dat' using 1:5 with lines title 'Peak'

eof

cat comp.tex | python ../fix.py > comp.tmp
mv comp.tmp comp.tex

pdflatex comp.tex

