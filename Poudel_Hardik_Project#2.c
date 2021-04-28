/* 
Class Spring 2021 Operating System (CS-4352-001)
Project 2

Author: Hardik Poudel
Student ID: R11645072
Date: 4/27/2020

desc: This is a C program to demonstrate find utility on Linux/Unix-like system.

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>

// Global variables

char *where, *name, *mmin, *inum, *command, *dest;
int w,n,m,i,d,e;

char temp[] = "/";
char temp2[] = ".";
char temp3[] = "..";

#define BUF_SIZE 8192

// Function to move files
int move(char* source, char* dest){
        int in_fd, out_fd;
        ssize_t ret_in, ret_out;
        char buffer[BUF_SIZE];

        in_fd = open(source, O_RDONLY);
        if(in_fd == -1){
                perror("open");
                return 2;
        }

        out_fd = open(dest, O_WRONLY | O_CREAT, 0644);
        if(out_fd == -1){
                perror("open");
                return 3;
        }

        while((ret_in = read(in_fd, &buffer, BUF_SIZE)) > 0){
                ret_out = write(out_fd, &buffer, (ssize_t) ret_in);
                if(ret_out != ret_in){
                        perror("write");
                        return 4;
                }
        }
        remove(source);
        close(in_fd);
        close(out_fd);

        return 0;
}

// Function to get inode of the filepath.
int get_inode(char* path){
	int fd = open(path, O_RDONLY);
	if(fd < 0){
		perror("open");
		return -1;
	}
	struct stat buf;
	int ret; 
	ret = fstat(fd, &buf);
	return buf.st_ino;
}

// Function to check if the file has been modified in the given time.
int isModified(char* path, int mmin){
	struct stat attr;
	stat(path, &attr);
	time_t mtime = attr.st_mtime;
	
	time_t now;
	time(&now);
	double timediff = difftime(now, mtime) / 60;
	if(mmin > 0){
		if(timediff >= mmin){
			return 1;
		}
	}else{
		if(timediff <= mmin*-1){
			return 1;
		}
	}
	return 0;
}

// function to check if the file is a directory
int isFile(char* path){
	struct stat sb;
	if(stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)){
		return 0;
	}
	return 1;
}


// Function to find all the file path of the given directory.
void find_where(char* where){
	//printf("Finding here: %s\n", where);
	DIR *where_p = opendir(where);
	struct dirent * where_dirp;
	if(where_p != NULL){
		while((where_dirp= readdir(where_p)) != NULL){
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, where_dirp->d_name);
			printf("%s\n", fullpath);
			
			char* temp1 = where_dirp->d_name;
					
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where(fullpath);
			}

		}
	}
	closedir(where_p);
}

// Function to find the given files that are in the given directory
void find_where_name(char* where, char* name){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(strcmp(name, temp1) == 0){
				printf("%s\n", fullpath);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where_name(fullpath, name);
			}
		}
	}
	closedir(where_p);
				
}

// Function to find the files that are modified in the given time.
void find_where_min(char* where, int mmin){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(isModified(fullpath, mmin) == 1){
				printf("%s\n", fullpath);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where_min(fullpath, mmin);
			}
		}
	}
	closedir(where_p);
}

// Function to find the files that have the given inode in the given directory.
void find_where_inode(char* where, int inum){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(get_inode(fullpath) == inum){
				printf("%s\n", fullpath);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where_inode(fullpath, inum);
			}
		}
	}
	closedir(where_p);
}

// Function to find the file in the given directory and delete it if the name matches.
void find_where_delete(char* where, char* name){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(strcmp(name, temp1) == 0){
				remove(fullpath);
				printf("%s deleted\n", fullpath);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where_delete(fullpath, name);
			}
		}
	}
	closedir(where_p);
}

// Function to find the file in the given directory if it was modified in the given time and delete it.
void find_where_delete_min(char* where, int mmin){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(isModified(fullpath, mmin) == 1){
				remove(fullpath);
				printf("%s deleted.\n", fullpath);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where_delete_min(fullpath, mmin);
			}
		}
	}
	closedir(where_p);
}


// Function to find the file in the given directory with the inode and delete it.
void find_where_delete_inode(char* where, int inum){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(get_inode(fullpath) == 1){
				remove(fullpath);
				printf("%s deleted.\n", fullpath);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				find_where_delete_inode(fullpath, inum);
			}
		}
	}
	closedir(where_p);
}

// Function to find the file in the given directory and move it to a given destination.
void execute_move(char* where, char* name, char* dest){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(strcmp(name, temp1) == 0 && isFile(fullpath) == 1){
				printf("%s\n", fullpath);
				move(fullpath, dest);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				execute_move(fullpath, name, dest);
			}
		}
	}
}

// Function to find the file in the given directory if the inode matches and move it to a given destination.
void execute_move_inode(char* where, int inum, char* dest){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(strcmp(name, temp1) == 0 && isFile(fullpath) == 1){
				printf("%s\n", fullpath);
				move(fullpath, dest);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				execute_move_inode(fullpath, inum, dest);
			}
		}
	}
}

// Function to find the file in the given directory and move it if the file has been modified in the given time.
void execute_move_min(char* where, int min, char* dest){
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(strcmp(name, temp1) == 0 && isFile(fullpath) == 1){
				printf("%s\n", fullpath);
				move(fullpath, dest);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				execute_move_min(fullpath, min, dest);
			}
		}
	}
}


// Function to find the file in the given directory and cat it if the name matches.
void execute_cat(char* where, char* name){
	char comm[] = "cat ";
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			char * temp1 = where_dirp->d_name;
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			if(strcmp(temp1, name) == 0 && isFile(fullpath) == 1){
				strcat(comm, fullpath);
				system(comm);
			}

			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){	
				execute_cat(fullpath, name);
			}
		}
		closedir(where_p);
	}
}

void execute_cat_inode(char* where, int inum){
	char comm[]  = "cat ";
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(get_inode(fullpath) == inum && isFile(fullpath) == 1) {
				strcat(comm, fullpath);
				system(comm);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				execute_cat_inode(fullpath, inum);
			}
		}
		closedir(where_p);
	}
}

void execute_cat_min(char* where, int mmin){
	char comm[] = "cat ";
	DIR *where_p = opendir(where);
	struct dirent* where_dirp;
	if(where_p != NULL){
		while((where_dirp = readdir(where_p)) != NULL){
			
			char *temp1 = where_dirp->d_name; 
			
			char *fullpath = malloc(sizeof(char)*2000);
			fullpath = strcpy(fullpath, where);
			strcat(fullpath, temp);
			strcat(fullpath, temp1);
			
			if(isModified(fullpath, mmin) == 1 && isFile(fullpath) == 1){
				strcat(comm, fullpath);
				system(comm);
			}
			
			if(strcmp(temp1, temp2) != 0 && strcmp(temp1, temp3) != 0){
				execute_cat_min(fullpath, mmin);
			}
		}
	}
}


void main(int argc, char* argv[]){
	if(argc >= 2){
		w = 1;
		where = argv[1];
	}
	char c;
	while((c = getopt(argc, argv, "w:n:m:i:de:")) != -1){
		switch(c){
			case 'n':
				n = 1;
				name = optarg;
				break;
			case 'm':
				m = 1;
				mmin = optarg;
				break;
			case 'i':
				i = 1;
				inum = optarg;
				break;
			case 'd':
				d = 1;
				break;
			case 'e':
				e = 1;
				command = optarg;
				dest = argv[optind];
				break;
			default:
				printf("An invalid argument detected\n");
			
		}	
	}
	
	char mv[] = "mv";
	char rm[] = "rm";
	char cat[] = "cat";


	
	if(w == 1 && n == 0 && m == 0 && i == 0 && d == 0 && e == 0){
		find_where(where);
	}else if(w == 1 && n == 1 && m == 0 && i == 0 && d == 0 && e == 0){
		find_where_name(where, name);
	}else if(w == 1 && n == 0 && m == 1 && i == 0 && d == 0 && e == 0){
		find_where_min(where, atoi(mmin));
	}else if(w == 1 && n == 0 && m == 0 && i == 1 && d == 0 && e == 0){
		find_where_inode(where, atoi(inum));
	}
	
	else if(w == 1 && n == 1 && m == 0 && i == 0 && d == 1 && e == 0){
		find_where_delete(where, name);
	}else if(w == 1 && n == 0 && m == 1 && i == 0 && d == 1 && e == 0){
		find_where_delete_min(where, atoi(mmin));
	}else if(w == 1 && n == 0 && m == 0 && i == 1 && d == 1 && e == 0){
		find_where_delete_inode(where, atoi(inum));
	}
	else if(w == 1 && n == 1 && m == 0 && i == 0 && d == 0 && e == 1){					

		if(strcmp(mv, command) == 0){
			execute_move(where, name, dest);
		}else if(strcmp(rm, command) == 0){
			find_where_delete(where, name);
		}else if(strcmp(cat, command) == 0){
			execute_cat(where, name);
		}
	}else if(w == 1 && n == 0 && m == 1 && i == 0 && d == 0 && e == 1){
	
		if(strcmp(mv, command) == 0){
			execute_move_min(where, atoi(mmin), dest);
		}else if(strcmp(rm, command) == 0){
			find_where_delete_min(where, atoi(mmin));
		}else if(strcmp(cat, command) == 0){
			execute_cat_min(where, atoi(mmin));
		}
	
	}else if(w == 1 && n == 0 && m == 0 && i == 1 && d == 0 && e == 1){
		if(strcmp(mv, command) == 0){
			execute_move_inode(where, atoi(inum), dest);
		}else if(strcmp(rm, command) == 0){
			find_where_delete_inode(where, atoi(inum));
		}else if(strcmp(cat, command) == 0){
			execute_cat_inode(where, atoi(inum));
		}
	}


}

