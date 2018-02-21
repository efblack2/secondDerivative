#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  computer"
  exit 1
fi

gnuplot -c plot.gnp $1 1
gnuplot -c plot.gnp $1 2
gnuplot -c plot.gnp $1 3

pdfunite $1X.pdf $1Y.pdf $1Z.pdf $1.pdf
rm $1X.pdf $1Y.pdf $1Z.pdf

gnuplot -c plotRatio.gnp $1 1
gnuplot -c plotRatio.gnp $1 2
gnuplot -c plotRatio.gnp $1 3

pdfunite $1XRatio.pdf $1YRatio.pdf $1ZRatio.pdf $1Ratio.pdf
rm $1XRatio.pdf $1YRatio.pdf $1ZRatio.pdf

gnuplot -c plotGnu.gnp $1 1
gnuplot -c plotGnu.gnp $1 2
gnuplot -c plotGnu.gnp $1 3

pdfunite $1X_gnu.pdf $1Y_gnu.pdf $1Z_gnu.pdf $1_gnu.pdf
rm $1X_gnu.pdf $1Y_gnu.pdf $1Z_gnu.pdf

gnuplot -c plotGnuRatio.gnp $1 1
gnuplot -c plotGnuRatio.gnp $1 2
gnuplot -c plotGnuRatio.gnp $1 3

pdfunite $1XRatio_gnu.pdf $1YRatio_gnu.pdf $1ZRatio_gnu.pdf $1Ratio_gnu.pdf
rm $1XRatio_gnu.pdf $1YRatio_gnu.pdf $1ZRatio_gnu.pdf 
