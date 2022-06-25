#!/bin/bash
cd ..

echo "-------------------------------------------------------------"
echo "REBUILDING PROJECT"
echo "-------------------------------------------------------------"
make
cd tests/

STEGOBMP=../bin/stegobmp
EXPECTED=expected.png

TESTS_RUN=0
TESTS_PASSED=0


file_cmp() {
  if cmp -s -- "$1" "$2"; then
    echo "$3 passed"
    ((TESTS_PASSED+=1))
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
  file_cmp $EXPECTED actual.png "test $1"
  echo "-------------------------------------------------------------"
  echo "#############################################################"
  echo ""
  ((TESTS_RUN+=1))
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
  ((TESTS_RUN+=1))
}

run_encrypted_extraction_test() {
  echo "#############################################################"
  echo "-------------------------------------------------------------"
  echo "running encrypted extraction test $1 $2 pass: $2"
  echo "-------------------------------------------------------------"
  $STEGOBMP --extract -p base_files/lado${1}${2}.bmp --steg=$1 -o actual -a=$3 -m=$4 --pass=$5
  file_cmp "expected.png" "actual.png" "test $1"
  echo "-------------------------------------------------------------"
  echo "#############################################################"
  echo ""
  ((TESTS_RUN+=1))
}

run_encrypted_embedding_test() {
  echo "#############################################################"
  echo "-------------------------------------------------------------"
  echo "running encrypted embedding test $1 $2 pass: $2"
  echo "-------------------------------------------------------------"
  $STEGOBMP --embed -i expected.png -p base_files/lado.bmp --steg=$1 -o actual.bmp -a=$3 -m=$4 --pass=$5
  file_cmp "base_files/lado${1}${2}.bmp" "actual.bmp" "test $1"
  echo "-------------------------------------------------------------"
  echo "#############################################################"
  echo ""
  ((TESTS_RUN+=1))
}

run_large_file_error_test() {
  echo "#############################################################"
  echo "-------------------------------------------------------------"
  echo "running embedding too large file test"
  echo "-------------------------------------------------------------"
  $STEGOBMP --embed -i video.wmv -p base_files/lado.bmp --steg=$1 -o tiny.bmp
  if test -f "tiny.bmp"; then
      echo "tiny embed test failed"
    else
      ((TESTS_PASSED+=1))
      echo "tiny embed test passed"
  fi
  echo "-------------------------------------------------------------"
  echo "#############################################################"
  echo ""
  ((TESTS_RUN+=1))
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

run_encrypted_extraction_test LSB1 aes192cbc AES192 CBC escondite 
run_encrypted_extraction_test LSBI aes256ofb AES256 OFB secreto

run_encrypted_embedding_test LSB1 aes192cbc AES192 CBC escondite 
run_encrypted_embedding_test LSBI aes256ofb AES256 OFB secreto

run_large_file_error_test LSB1

echo "#############################################################"
echo "-------------------------------------------------------------"
echo "CLEANING UP"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

echo ""
echo "#############################################################"
echo "-------------------------------------------------------------"
echo "PASSED $TESTS_PASSED/$TESTS_RUN"
echo "-------------------------------------------------------------"
echo "#############################################################"
echo ""

rm -f actual.bmp actual.png

cd ../
make clean
