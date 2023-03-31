// file for Part 2

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_STRLEN	64
#define MAX_ELEMENTS	1024

int perm_check(char* perm_string) {
	if (strlen(perm_string) != 9) {
		fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
		return -1;
	}

	for (int i = 0; i < 9; i++) {
		int j = i % 3;
		char s = perm_string[i];
		if (j == 0 && !(s == 'r' || s == '-')) {
				fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
                		return -1;		
		}
		
		else if (j == 1 && !(s == 'w' || s == '-')) {
                                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
                                return -1;
                } 

		else if (j == 2 && !(s == 'x' || s == '-')) {
                                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_string);
                                return -1;
                }
	}
	return 1;
}

int main(int argc, char *argv[]) {
	int dflag = 0;
	int pflag = 0;
	opterr = 0;
	int c;
	char* dir_name = NULL;
	char* perm_string = NULL;

	// Getopt - options -d, -p, -h
	while ((c = getopt(argc, argv, "d:p:h")) != -1) {
		switch(c) {
			case 'd':
				dflag = 1;
				dir_name = optarg;
				break;
				fprintf(stdout, "doesn't break after case 'd'\n");
			case 'p':
				pflag = 1;
				perm_string =  optarg;
				break;
			case 'h':
				fprintf(stdout, "Usage: ./spfind -d <directory> -p <permissions string> [-h]\n");
				return EXIT_SUCCESS;
			case '?':
				fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
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

	if (perm_check(perm_string) == -1) {
		return EXIT_FAILURE;
	}
	// Done testing

	// Set up pipes before forking
	int pfind_to_sort[2], sort_to_parent[2];
	if (pipe(pfind_to_sort) == -1) {
		fprintf(stderr, "Error: pipe failed. %s.\n", strerror(errno));
	}
	if (pipe(sort_to_parent) == -1) {
		fprintf(stderr, "Error: pipe failed. %s.\n", strerror(errno));
	}

	pid_t pid[2];
	if ((pid[0] = fork()) == -1) {
		// error forking
		fprintf(stderr, "Error: fork failed. %s.\n", strerror(errno));
	} else {
		// in pfind process
		close(pfind_to_sort[0]);
		dup2(pfind_to_sort[1], STDOUT_FILENO);
		close(pfind_to_sort[1]);

		// close unrelated processes
		close(sort_to_parent[0]);
		close(sort_to_parent[1]);

		if (execlp("pfind", "pfind", dir_name, perm_string, NULL) == -1) {
			fprintf(stderr, "Error: pfind failed.");
		}
	}
	if ((pid[1] = fork()) == -1) {
		// error forking
		fprintf(stderr, "Error: fork failed. %s.\n", strerror(errno));
	} else {
		// in sort process
		close(pfind_to_sort[1]);
		dup2(pfind_to_sort[0], STDIN_FILENO);
		close(pfind_to_sort[0]);

		close(sort_to_parent[0]);
		dup2(sort_to_parent[1], STDOUT_FILENO);
		close(sort_to_parent[1]);
		
		if (execlp("sort", "sort", "-o stdout", NULL) == -1) {
			fprintf(stderr, "Error: sort failed.");
		}
	}

	// in parent now
	close(pfind_to_sort[0]);
	close(pfind_to_sort[1]);
	close(sort_to_parent[1]);
	dup2(sort_to_parent[0], STDIN_FILENO);
	close(sort_to_parent[0]);
	/*
	char buf[MAX_STRLEN][MAX_ELEMENTS];
	int i = 0;
	while (read(stdin, buf) != 0) {
		
	}
	*/

}





