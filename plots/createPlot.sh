#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  computer"
  exit 1
fi

cd $1/gnu
paste MPI_sm.txt OpenMP.txt > result.txt
paste MPI_sm_Pin.txt OpenMP_Pin.txt > result_Pin.txt

cd ../intel
paste MPI_sm.txt OpenMP.txt > result.txt
paste MPI_sm_Pin.txt OpenMP_Pin.txt > result_Pin.txt

cd ../pgi
paste MPI_sm.txt OpenMP.txt > result.txt
paste MPI_sm_Pin.txt OpenMP_Pin.txt > result_Pin.txt

cd ../..


gnuplot -c plot.gnp $1 1
gnuplot -c plot.gnp $1 2
gnuplot -c plot.gnp $1 3

pdfunite $1X.pdf $1Y.pdf $1Z.pdf $1.pdf
rm $1X.pdf $1Y.pdf $1Z.pdf

gnuplot -c plotPin.gnp $1 1
gnuplot -c plotPin.gnp $1 2
gnuplot -c plotPin.gnp $1 3

pdfunite $1X_Pin.pdf $1Y_Pin.pdf $1Z_Pin.pdf $1_Pin.pdf
rm $1X_Pin.pdf $1Y_Pin.pdf $1Z_Pin.pdf


gnuplot -c plotRatio.gnp $1 1
gnuplot -c plotRatio.gnp $1 2
gnuplot -c plotRatio.gnp $1 3

pdfunite $1XRatio.pdf $1YRatio.pdf $1ZRatio.pdf $1Ratio.pdf
rm $1XRatio.pdf $1YRatio.pdf $1ZRatio.pdf

gnuplot -c plotRatioPin.gnp $1 1
gnuplot -c plotRatioPin.gnp $1 2
gnuplot -c plotRatioPin.gnp $1 3

pdfunite $1XRatio_Pin.pdf $1YRatio_Pin.pdf $1ZRatio_Pin.pdf $1Ratio_Pin.pdf
rm $1XRatio_Pin.pdf $1YRatio_Pin.pdf $1ZRatio_Pin.pdf




gnuplot -c plotGnu.gnp $1 1
gnuplot -c plotGnu.gnp $1 2
gnuplot -c plotGnu.gnp $1 3

pdfunite $1X_Gnu.pdf $1Y_Gnu.pdf $1Z_Gnu.pdf $1_Gnu.pdf
rm $1X_Gnu.pdf $1Y_Gnu.pdf $1Z_Gnu.pdf 

gnuplot -c plotGnuPin.gnp $1 1
gnuplot -c plotGnuPin.gnp $1 2
gnuplot -c plotGnuPin.gnp $1 3

pdfunite $1X_GnuPin.pdf $1Y_GnuPin.pdf $1Z_GnuPin.pdf $1_GnuPin.pdf
rm $1X_GnuPin.pdf $1Y_GnuPin.pdf $1Z_GnuPin.pdf


gnuplot -c plotGnuRatio.gnp $1 1
gnuplot -c plotGnuRatio.gnp $1 2
gnuplot -c plotGnuRatio.gnp $1 3

pdfunite $1XRatio_Gnu.pdf $1YRatio_Gnu.pdf $1ZRatio_Gnu.pdf $1Ratio_Gnu.pdf
rm $1XRatio_Gnu.pdf $1YRatio_Gnu.pdf $1ZRatio_Gnu.pdf 

gnuplot -c plotGnuRatioPin.gnp $1 1
gnuplot -c plotGnuRatioPin.gnp $1 2
gnuplot -c plotGnuRatioPin.gnp $1 3

pdfunite $1XRatio_GnuPin.pdf $1YRatio_GnuPin.pdf $1ZRatio_GnuPin.pdf $1Ratio_GnuPin.pdf
rm $1XRatio_GnuPin.pdf $1YRatio_GnuPin.pdf $1ZRatio_GnuPin.pdf


#rm `find . -name result*.txt`

