gnuplot << eof
set terminal epslatex size 4,2.82 standalone color colortext 9
set output 'shelxd_cc_a.tex'
set xrange [0:40]
set yrange [0:200]
set xlabel 'CC/\%'
set ylabel 'ntry / 200'
plot '900_shelxd_cc_a.dat' using 1:2 with lines lw 4 title '900 frames', \
'900_shelxd_cc_a.dat' using 1:3 with lines lw 4 title '900 frames (weak)', \
'1800_shelxd_cc_a.dat' using 1:2 with lines lw 4 title '1800 frames', \
'1800_shelxd_cc_a.dat' using 1:3 with lines lw 4 title '1800 frames (weak)'
eof

pdflatex shelxd_cc_a.tex

