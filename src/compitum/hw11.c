#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	char c;
	if (strcmp(argv[0], "./lower")==0) {
		while ((c = getchar()) != EOF) {
			if (c >= 'a' && c <= 'z')
				putchar((c-32)); 
			else
				putchar(c);
		}
	} else {
		while ((c = getchar()) != EOF) {
			if (c >= 'A' && c <= 'Z')
				putchar((c+32));
			else
				putchar(c);
		}
    }
    return 0;
}		