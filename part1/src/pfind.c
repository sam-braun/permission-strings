// This is for Part 1

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

void display_usage() {
	fprintf(stdout, "Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
	return EXIT_SUCCESS;
}

void perm_check(char* perm_string) {
	if (strlen(perm_string) != 9) {
		fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
		return EXIT_FAILURE;
	}

	for (int i = 0; i < 9; i++) {
		int j = i % 3;
		char s = perm_string[i];
		if (j == 0 && (s != 'r' || s != '-')) {
				fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
                		return EXIT_FAILURE;		
		}
		
		else if (j == 1 && (s != 'w' || s != '-')) {
                                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
                                return EXIT_FAILURE;
                } 

		else if (j == 2 && (s != 'x' || s != '-')) {
                                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
                                return EXIT_FAILURE;
                }
	}
}

int main(int argc, char **argv) {

	int dflag = 0;
	int pflag = 0;
	int hflag = 0;
	opterr = 0;
	int c;
	char* dir_name;
	char* permission_string;

	// Getopt - options -d, -p, -h
	while ((c = getopt(argc, argv, "d:p:h")) != 1) {
		switch(c) {
			case 'd':
				dflag = 1;
				strcpy(dir_name, optarg);
				break;
			case 'p':
				pflag = 1;
				strcpy(permission_string, optarg);
				break;
			case 'h':
				display_usage();
			case '?':
				fprintf(stderr, "Error: Unknown option '%c' received.\n", optopt);
				return EXIT_FAILURE;
			default:
				return EXIT_FAILURE;
		}
	}

	// Error handling
	if (pflag == 0) {
		fprintf(stderr, "Error: Required argument -p <permissions string> not found.\n");
		return EXIT_FAILURE;
	} else if (dflag == 0) {
		fprintf(stderr, "Error: Required argument -d <directory> not found.\n");
		return EXIT_FAILURE;
	} 
	
	// Stat file
	struct stat buf;
	if (lstat(dir_name, &buf) < 0) {
		fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", dir_name, strerror(errno));
		return EXIT_FAILURE;
	}

	perm_check(permission_string);

}
