git submodule update --init --recursive > /dev/null;
cd src;
make -j`nproc`;
mv libc.a librt.a ..;
make clean > /dev/null;
cd ..;
