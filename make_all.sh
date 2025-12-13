git submodule update --init --recursive > /dev/null;
cd src;
make -j`nproc`;
mv libm.a libc.a librt.a ..;
make clean > /dev/null;
cd ..;
