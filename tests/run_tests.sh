#!/bin/bash
cd ..

echo "-------------------------------------------------------------"
echo "REBUILDING PROJECT"
echo "-------------------------------------------------------------"
make
cd tests/

STEGOBMP=../bin/stegobmp
OUT=actual.png
EXPECTED=expected.png


file_cmp() {
  if cmp -s -- "$1" "$2"; then
    echo "$3 passed"
  else
    echo "$3 failed"
  fi
}

run_plain_extraction_test() {
  echo "#############################################################"
  echo "-------------------------------------------------------------"
  echo "running extraction test $1"
  echo "-------------------------------------------------------------"
  $STEGOBMP --extract -p base_files/lado$1.bmp --steg=$1 -o actual
  file_cmp $EXPECTED $OUT "test $1"
  echo "-------------------------------------------------------------"
  echo "#############################################################"
  echo ""
}

run_plain_embedding_test() {
  echo "#############################################################"
  echo "-------------------------------------------------------------"
  echo "running embedding test $1"
  echo "-------------------------------------------------------------"
  $STEGOBMP --embed -i expected.png -p base_files/lado.bmp --steg=$1 -o actual.bmp
  file_cmp "base_files/lado$1.bmp" "actual.bmp" "test $1"
  echo "-------------------------------------------------------------"
  echo "#############################################################"
  echo ""
}

echo "#############################################################"
echo "-------------------------------------------------------------"
echo "RUNNING TEST SUITE"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

run_plain_extraction_test LSB1
run_plain_extraction_test LSB4
run_plain_extraction_test LSBI

run_plain_embedding_test LSB1
run_plain_embedding_test LSB4
run_plain_embedding_test LSBI

echo "#############################################################"
echo "-------------------------------------------------------------"
echo "CLEANING UP"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

rm $OUT

cd ../
make clean
