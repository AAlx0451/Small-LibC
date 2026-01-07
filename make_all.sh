./getlibmach.sh
git submodule update --init --recursive > /dev/null;
cd src;
make -j$((1 + $(nproc))) > /dev/null;
mv libm.a libc.a librt.a ..;
make clean > /dev/null;
cd ..;
