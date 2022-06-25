#!/bin/bash
cd ..
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

# TEST LSB1

echo "-------------------------------------------------------------"
echo "running extraction test LSB1"
echo "-------------------------------------------------------------"
$STEGOBMP --extract -p base_files/ladoLSB1.bmp --steg=LSB1 -o actual
file_cmp $EXPECTED $OUT "test_lsb1_plain"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

# TEST LSB4
echo "-------------------------------------------------------------"
echo "running extraction test LSB4"
echo "-------------------------------------------------------------"
$STEGOBMP --extract -p base_files/ladoLSB4.bmp --steg=LSB4 -o actual
file_cmp $EXPECTED $OUT "test_lsb4_plain"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

# TEST LSBI
echo "-------------------------------------------------------------"
echo "running extraction test LSBI"
echo "-------------------------------------------------------------"
$STEGOBMP --extract -p base_files/ladoLSBI.bmp --steg=LSBI -o actual
file_cmp $EXPECTED $OUT "test_lsbi_plain"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

rm $OUT

cd ../
make clean
