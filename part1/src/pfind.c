// This is for Part 1

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int perm_check(char* perm_string) {
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
	return 0;
}

int perm_match(char* legit, char* curr) {
	for (int i = 0; i < 9; i++) {
		if (*legit != *curr) {
			return -1;
		}
		legit++;
		curr++;
	}
	return 1;
}

char* get_perm_string(struct stat srcc) {
	char[9] ps = "---------";
	
	(srcc.stmode & S_IRUSR) ? ps[0] = 'r' : ps[0] = '-';
	(srcc.stmode & S_IWUSR) ? ps[0] = 'w' : ps[0] = '-';
	(srcc.stmode & S_IXUSR) ? ps[0] = 'x' : ps[0] = '-';

	(srcc.stmode & S_IRGRP) ? ps[0] = 'r' : ps[0] = '-';
	(srcc.stmode & S_IWGRP) ? ps[0] = 'w' : ps[0] = '-';
	(srcc.stmode & S_IXGRP) ? ps[0] = 'x' : ps[0] = '-';

	(srcc.stmode & S_IROTH) ? ps[0] = 'r' : ps[0] = '-';
	(srcc.stmode & S_IWOTH) ? ps[0] = 'w' : ps[0] = '-';
	(srcc.stmode & S_IXOTH) ? ps[0] = 'x' : ps[0] = '-';

	return ps;

}

int main(int argc, char **argv) {

	int dflag = 0;
	int pflag = 0;
	opterr = 0;
	int c;
	char* dir_name = NULL;
	char* permission_string = NULL;

	// Getopt - options -d, -p, -h
	while ((c = getopt(argc, argv, "d:p:h")) != -1) {
		switch(c) {
			case 'd':
				fprintf(stdout, "enters case d\n");
				dflag = 1;
				dir_name = optarg;
				fprintf(stdout, "%s\n", dir_name);
				break;
				fprintf(stdout, "doesn't break after case 'd'\n");
			case 'p':
				fprintf(stdout, "reached case 'p'\n");
				pflag = 1;
				permission_string =  optarg;
				break;
			case 'h':
				fprintf(stdout, "enters case h\n");
				fprintf(stdout, "Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
				return EXIT_SUCCESS;
			case '?':
				fprintf(stderr, "Error: Unknown option '%c' received.\n", optopt);
				return EXIT_FAILURE;
			default:
				fprintf(stdout, "reaches default case\n");
				return EXIT_FAILURE;
		}
	}

	fprintf(stdout, "before error check\n");

	// Error handling
	if (pflag == 0) {
		fprintf(stdout, "reached pflag == 0\n");
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
