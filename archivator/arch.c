#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <fcntl.h>
#include <stdbool.h>
#include <malloc.h>

#define ARCHIVE "archive.out"

void dirwalker(char* dir, int out, int depth);
void readfromarchive(char* arch, char* dest);
int filewrite(char* filename, int out, bool isDir, int level, long size);
int fileread(int in, char* filename, long size);

int main(int argc, char *argv[]) {
	int rez = 0;
	char dirname[PATH_MAX];
	char* dir = getwd(dirname);
	char* target = NULL;

//	opterr = 0;
	while ( (rez = getopt(argc, argv, "d:t:")) != -1){
		switch (rez) {
		case 'd': 
			if(strcmp(optarg, ".") != 0)
				dir = optarg; 
			if(strcmp(optarg, "..") == 0)
				dir = "";
			break;
		case 't':
			target = optarg;
			break;
		case '?': printf("Error found !\n"); break;
		} 
	}
	if(target != NULL){
		readfromarchive(target, dir);
	}else{
		//printf("%s\n", dir);
		int out = open(ARCHIVE, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		dirwalker(dir, out, 0);
	}
}

void dirwalker(char* dir, int out, int depth){
  DIR* d;
  struct dirent* entry;
  struct stat statbuff;

  if((d = opendir(dir)) != NULL){
	chdir(dir);
	while((entry = readdir(d)) != NULL){
		lstat(entry->d_name, &statbuff);
		if(S_ISDIR(statbuff.st_mode)){
			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			filewrite(entry->d_name, out, true, depth, statbuff.st_size);
			dirwalker(entry->d_name, out, depth+1);
		}else{
			if(strcmp(entry->d_name, ARCHIVE) == 0)
				continue;
			filewrite(entry->d_name, out, false, depth, statbuff.st_size);
		}
	}
	chdir("..");
	closedir(d);
  }
}

void readfromarchive(char* arch, char* dest){
	printf("\n------------------------------------\n");
	printf("reading from archive = %s  to destination =  %s\n", arch, dest);
	chdir(dest);
	int source = open(arch, O_RDONLY);
	if(source != -1)
		printf("successfully opened source = %s\n", arch);
	else
		printf("failed to open source\n");
	 	
	int currentlevel = 0;
	char* lastdir = (char*)malloc(strlen(dest)+1);
	char type;
	int level, namesize;
	long size;
	bool isDir = false;

	printf("\n------------------------------------\n");
	printf("trying to read from archive\n");

	while(true){
		if(read(source, &type, 1) <= 0)
			break;
		isDir = false;	
		if(type == 'd')
			isDir = true;

		printf("type = %c\n", type);
		
		read(source, &level, sizeof(level));
		printf("level = %d\n", level);

		read(source, &namesize, sizeof(namesize));
		printf("namesize = %d\n", namesize);
		char* filename = (char*)malloc(namesize+1);
		read(source, filename, namesize+1);
		printf("filename = %s   size = %d\n", filename, namesize);

		read(source, &size, sizeof(size));
		printf("size = %ld\n", size);

		printf("current level = %d vs level = %d\n", currentlevel, level);
		if(level > currentlevel){
				printf("current level = %d < level so going to -> %s\n", currentlevel, lastdir);
				chdir(lastdir);
				currentlevel++;
				char PathName[PATH_MAX];
				char* PN = getwd (PathName);
				printf("current directory: %s\n", PN);
		}else if(level < currentlevel){
			printf("current level = %d > level so going up\n", currentlevel);
			while(level < currentlevel){
				printf("...up  level = %d  vs  current level = %d\n", level, currentlevel);
				chdir("..");
				currentlevel--;
			}
			printf("current level = %d == level\n", currentlevel);
		}

		if(isDir){
			int res = mkdir(filename, S_IRWXU | S_IRWXG | S_IRWXO);
			printf("\ttrying to create dir = %s  level = %d\n", filename, level);
			if(res != -1){
				printf("\t\tsuccess!!!\n");
				lastdir = (char*)realloc(lastdir, namesize+1);
				strcpy(lastdir, filename);
			}else
				printf("\t\tfailed to make a dir!\n");
			
		}else{
			printf("going to read a file = %s  level = %d\n", filename, level);
			fileread(source, filename, size);
		}
		free(filename); 
	}
}

int filewrite(char* filename, int out, bool isDir, int level, long size){
	printf("\n------------------------------------\n");
	printf("trying to write a file into archive\n");

	if(out == -1)
		printf("failed to open an archive!\n");
	else 
		printf("out = %d!\n", out);

	char buf[1024];
	int nread;
	int in = open(filename, O_RDONLY);
	if(in == -1)
		printf("failed to open a file!\n");

	char d = 'f';
	if(isDir)
		d = 'd';
	write(out, &d, 1);
	printf("type = %c\n", d);
	
	write(out, &level, sizeof(level));
	printf("level = %d\n", level);
	
	int namesize = strlen(filename);
	write(out, &namesize, sizeof(namesize));
	write(out, filename, namesize+1);
	printf("filename = %s  size = %d\n", filename, namesize);

	write(out, &size, sizeof(size));
	printf("size of file = %ld\n", size);

	if(!isDir){
		long sum = 0;
		while((nread = read(in, buf, sizeof(buf))) > 0){ 
			write(out, buf, nread);
			sum += nread;
		}
		
		printf("successfully has written : %ld\n", sum);
	}
	close(in);
	printf("\n------------------------------------\n");
}

int fileread(int in, char* filename, long size){
	char buf[1024];
	printf("\n\t------------------------------------\n");
	printf("trying to create a file = %s\n", filename);
	int fout = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if(fout != -1)
		printf("\t\tsuccess!!!\n");
	else
		printf("\t\tfailed to make a dir!\n");
	
	printf("writing to file = %s\n", filename);
	int nread;
	long sum = 0;
	long fullblock = size / 1024;
	long remains = size - fullblock*1024;
	for(int i = 0; i < fullblock; i++){
		read(in, buf, sizeof(buf));
		write(fout, buf, sizeof(buf));
	}
	read(in, buf, remains);
	write(fout, buf, remains);
	printf("DONE writing to file = %s\n", filename);
	printf("\n\t------------------------------------\n");
}