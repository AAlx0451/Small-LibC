# Small-LibC: very incomplete LibC implementation – WIP
# well, currently we have syscall(), _exit() and write(). but assembly syscall() implementation exists, so now we can bootstrap ourselves 
# designed for iOS 6, should work for 2xxx XNU tree. or not. well, you SHOULD NOT try run this at iOS 16 :). but you can port it. or not!
# portage guide:
## 1. replace syscall.h with the new one (include/sys/syscall.h)
## 2. rewrite syscall.c to AArch64 (32 now)
## 3. That's it
# and one more thing – it won't work with PC XNU 2xxx. to port it just rewrite syscall.c, syscall.h is ok. target macOS will be 10.8-10.9


CC = cc # my repo -> clang-installer package should install CC. main thing is that you don't even need an SDK!
CFLAGS = -nostdinc -I../include # included with GitHub repo
LDFLAGS = -static -lSystem -L.. -nostdlib # lSystem is a name for our libC. you can make it using "cd ../src; make; cp li*a ..", but it should be included with repo
TARGET = hello

$(TARGET): hello.o
	$(CC) -o $(TARGET) hello.o ../start.o $(LDFLAGS) 
# start.o is a symlink for ../apple/crt1.o. my own implementation with be later. or won't! anyway, it just works
	@echo -e "\n\nTESTS:\n"
	@echo 'testing if it works... (good one is "hello world", bad one is SegFault)'
	@./hello
	@echo 'testing if it is static... (good output is "hello:")'
	@otool -L hello

hello.o: hello.c
	$(CC) -c hello.c -o hello.o $(CFLAGS)

all: $(TARGET)

clean:
	rm -rf hello.o hello
