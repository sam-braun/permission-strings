// This is for Part 1

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>

int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
               S_IRGRP, S_IWGRP, S_IXGRP,
               S_IROTH, S_IWOTH, S_IXOTH};

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

char* permission_string(struct stat *statbuf) {
    char *perm_string;
    if ((perm_string = malloc(11 * sizeof(char))) == NULL) {
        fprintf(stderr, "Error: malloc failed. %s.\n",
                strerror(errno));
        exit(EXIT_FAILURE);
    }
    perm_string[0] = '-';
    for (int i = 0; i < 9; i += 3) {
        // Using the ternary operator for succinct code.
        perm_string[i + 1] = statbuf->st_mode & perms[i] ? 'r' : '-';
        perm_string[i + 2] = statbuf->st_mode & perms[i + 1] ? 'w' : '-';
        perm_string[i + 3] = statbuf->st_mode & perms[i + 2] ? 'x' : '-';
    }
    perm_string[10] = '\0';
    return perm_string;
}

int main(int argc, char **argv) {

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
				fprintf(stdout, "enters case d\n");
				dflag = 1;
				dir_name = optarg;
				fprintf(stdout, "%s\n", dir_name);
				break;
				fprintf(stdout, "doesn't break after case 'd'\n");
			case 'p':
				fprintf(stdout, "reached case 'p'\n");
				pflag = 1;
				perm_string =  optarg;
				break;
			case 'h':
				fprintf(stdout, "enters case h\n");
				fprintf(stdout, "Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
				return EXIT_SUCCESS;
			case '?':
				fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
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
	
	if (perm_check(perm_string) == -1) {
		return EXIT_FAILURE;
	}

	// Now recurse through given directory
	DIR *dir;
	if ((dir = opendir(dir_name)) == NULL) {
		fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", dir_name, strerror(errno));
		return EXIT_FAILURE;
	}

	char full_filename[PATH_MAX + 1];
	size_t pathlen = strlen(full_filename) + 1;
	full_filename[pathlen - 1] = '/';
	full_filename[pathlen] = '\0';
	//strcpy(full_filename, dir_name);

	struct dirent *entry;
	struct stat sb;
	while ((entry = readdir(dir)) != NULL) {
		strncpy(full_filename + pathlen, entry->d_name, PATH_MAX - pathlen);
		if (lstat(full_filename, &sb) < 0) {
		fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n",
			full_filename, strerror(errno));
			continue;
		}
		if (strcmp(permission_string(&sb), perm_string) == 0) {
			strcat(full_filename, entry->d_name);
			fprintf(stdout, "%s\n", full_filename);
		}
	}

	closedir(dir);
	return EXIT_SUCCESS;

}
