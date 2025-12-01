rm -rf *.o;
echo "  CC   crt.c";
cc -w crt.c -o crt.o -c;
echo "  AS   start.s";
cc -w -o start.o start.s -c;
echo "  LD   crt1.o"
cc -o crt1.o -r crt.o start.o -w;
chmod +x crt1.o;
rm crt.o start.o;
