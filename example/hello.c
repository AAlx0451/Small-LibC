#include <unistd.h>
#include <stdlib.h>

int main() {
  write(
	 1,
	 "hello world\n",
	 12
	 );
  exit(0);
}
