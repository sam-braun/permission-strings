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
	/*
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
	*/
	// Done testing
	if (argc == 1) {
		fprintf(stdout, "Usage: ./spfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_SUCCESS;
	}

	// Set up pipes before forking
	int pfind_to_sort[2], sort_to_parent[2];
	if (pipe(pfind_to_sort) == -1) {
		fprintf(stderr, "Error: pipe failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if (pipe(sort_to_parent) == -1) {
		fprintf(stderr, "Error: pipe failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;

	}
	printf("passed piping\n");

	pid_t pid[2];
	if ((pid[0] = fork()) == -1) {
		// error forking
		fprintf(stderr, "Error: fork failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;

	} else if (pid[0] == 0) {
		printf("passed first fork - in the child\n");
		// in pfind process
		close(pfind_to_sort[0]);
		dup2(pfind_to_sort[1], STDOUT_FILENO);
		close(pfind_to_sort[1]);

		// close unrelated processes
		close(sort_to_parent[0]);
		close(sort_to_parent[1]);

		fprintf(stderr, "above first execvp\n");

		if (execvp("./pfind", argv) == -1) {      // formerly two "pfind"
			fprintf(stderr, "Error: pfind failed.\n");
			return EXIT_FAILURE;
		}
	}
	if ((pid[1] = fork()) == -1) {
		// error forking
		fprintf(stderr, "Error: fork failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	} else if (pid[1] == 0) {
		fprintf(stderr, "enters sort child\n");
		// in sort process
		close(pfind_to_sort[1]);
		dup2(pfind_to_sort[0], STDIN_FILENO);
		close(pfind_to_sort[0]);

		close(sort_to_parent[0]);
		dup2(sort_to_parent[1], STDOUT_FILENO);
		close(sort_to_parent[1]);
		
		fprintf(stderr, "before execlp of sort\n");

		if (execlp("sort", "sort", NULL) == -1) {	// "-o", "stdout"
			fprintf(stderr, "Error: sort failed.");
			return EXIT_FAILURE;
		}
	}

	// in parent now
	close(pfind_to_sort[0]);
	close(pfind_to_sort[1]);
	close(sort_to_parent[1]);
	dup2(sort_to_parent[0], STDIN_FILENO);
	close(sort_to_parent[0]);
	
	fprintf(stderr, "enters parent\n");

	// Print directories from pipe
	char buf[10];
	int count = 0;
	while (read(STDIN_FILENO, &buf, sizeof(buf) - 1) != 0) {
		printf("%s", buf);
		if (strchr(buf, '\n') != NULL) {
			count++;
		}
	}
	printf("\n");

	// Print number of matches
	/*
	int count = 0;
	fseek(STDIN_FILENO, 0, SEEK_SET);
	for (char c = getc(STDIN_FILENO); c != EOF; c = getc(STDIN_FILENO)) {
		if (c == '\n') {
			count++;
		}
	}
	*/
	printf("Total matches: %d\n", count);

	wait(NULL);
	wait(NULL);

	return EXIT_SUCCESS;
}





