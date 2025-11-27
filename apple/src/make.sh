rm -rf *.o;
cc -o crt1.o -r crt.c start.s -w;
chmod +x crt1.o;
