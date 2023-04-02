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

int main(int argc, char *argv[]) {

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
	
	pid_t pid[2];
	if ((pid[0] = fork()) == -1) {
		// error forking
		fprintf(stderr, "Error: fork failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	} else if (pid[0] == 0) {
		// in pfind process
		close(pfind_to_sort[0]);
		dup2(pfind_to_sort[1], STDOUT_FILENO);
		close(pfind_to_sort[1]);
		// close unrelated processes
		close(sort_to_parent[0]);
		close(sort_to_parent[1]);
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
		// in sort process
		close(pfind_to_sort[1]);
		dup2(pfind_to_sort[0], STDIN_FILENO);
		close(pfind_to_sort[0]);
		close(sort_to_parent[0]);
		dup2(sort_to_parent[1], STDOUT_FILENO);
		close(sort_to_parent[1]);
		
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
	
	pid_t wpid1, wpid2;
	int status;
	
	wpid1 = waitpid(pid[0], &status, 0);
	if (wpid1 == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	wpid2 = waitpid(pid[1], &status, 0);
	if (wpid2 == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}
	
	
	
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

 	printf("Total matches: %d\n", count);

 	wait(NULL);
 	wait(NULL);
	return EXIT_SUCCESS;
}

