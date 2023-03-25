// This is for Part 1

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {

	void display_usage() {
		fprintf(stdout, "Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_SUCCESS;
	}

	int dflag = 0;
	int pflag = 0;
	int hflag = 0;
	opterr = 0;
	char[] dir_name;
	char[] permission_string;

	// Getopt - options -d, -p, -h
	while ((c = getopt(argc, argv, "d:p:h")) != 1) {
		switch(c) {
			case 'd':
				dflag = 1;
				dir_name = optarg;
				break;
			case 'p':
				pflag = 1;
				permission_string = optarg;
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
	char 
	if (lstat(dir_name, &buf) < 0) {
		fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", dir_name, strerror(errno));
		return EXIT_FAILURE;
	}

	if (strlen(permission_string) != 10) {
		fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", permission_string);
		return EXIT_FAILURE;
	}
	if (!((char s = permission_string[0]) == '-' || s == 'd' || s == 'l' || s == 'p' || s == 's' || s == 'c' || s == 'b' || s == 'D')) {
		fprintf(stderr,  "Error: Permissions string '%s' is invalid.\n", permission_string);
		return EXIT_FAILURE;
	}
	for (int i = 1; i < 10; i++) {
		char s = permission_string[i];
		if (!(s == 'r' || s == 'x' || s == 'w' || s == '-')) {
			fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", permission_string);
		return EXIT_FAILURE;
		}
	}


}
