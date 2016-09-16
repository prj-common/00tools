#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE      (1024*5)
#define MAX_FILE_SIZE      (1024*10)
#define MAX_CMD_SIZE       (1024*100)
#define MAX_DIR_NAME_SIZE  (1024*200)
#define MAX_FILE_BUF_SIZE  (1024*200)

static char *dir_backup_name = "00my_backup";
static char dir_backup_full_name[MAX_FILE_SIZE];
static int backup_subdir_num;
static int todo_flag = 0;

static char parse_dir_names[MAX_DIR_NAME_SIZE];
static char file_buf[MAX_FILE_BUF_SIZE];

static int create_dir(char *dir) {
	struct stat st = {0};

	if(stat(dir, &st) == -1) {
		mkdir(dir, 0755);
	}

	return 0;
}

static int backup_file(char *name) {
	static char cmd[MAX_FILE_SIZE];
	static char back_dir_tmp[MAX_FILE_SIZE];
	char tmp[16];

	memset(cmd, 0, MAX_FILE_SIZE);
	memset(back_dir_tmp, 0, MAX_FILE_SIZE);
	memset(tmp, 0, 16);

	sprintf(tmp, "%d", backup_subdir_num);

	strcpy(back_dir_tmp, dir_backup_full_name);
	strcat(back_dir_tmp, "/");
	strcat(back_dir_tmp, tmp);
	printf("back_dir_tmp: %s\n", back_dir_tmp);
	create_dir(back_dir_tmp);

	strcpy(cmd, "mv ");
	strcat(cmd, name);
	strcat(cmd, " ");
	strcat(cmd, back_dir_tmp);

	system(cmd);

	printf("cmd: %s\n", cmd);
	return 0;
}

static int handle_compressed_file(char *name, char *path, char *raw_name) {
	int fd;
	char buffer[32];
	static char name_back[MAX_FILE_SIZE];
	if(NULL == name) {
		return 0;
	}

	memset(buffer, 0, 32);
	fd = open(name, O_RDONLY);
	if(fd < 0) {
		return 0;
	}

	read(fd, buffer, sizeof(buffer));
	close(fd);

	buffer[31] = 0;
	if(!strncmp("\x1F\x8B\x08", buffer, 3)) // gzip file
	{
		todo_flag = 1;
		memset(name_back, 0, MAX_FILE_SIZE);
		strcpy(name_back, name);
		strcat(name, ".gz");
		rename(name_back, name);

		strcpy(name_back, "gunzip ");
		strcat(name_back, name);
		system(name_back);

		return 0;
	} 
	else if(!strncmp("\x50\x4B\x03\x04", buffer, 4)) { // zip file
		strcpy(name_back, "unzip -o -q -d ");
		strcat(name_back, path);
		strcat(name_back, "/");
		strcat(name_back, raw_name);
		strcat(name_back, "_d ");
		strcat(name_back, name);
		printf("%s\n", name_back);
		system(name_back);
		printf("file name: %s\n", name);
		backup_file(name);
		todo_flag = 1;
		return 1;
	} else if(!strncmp("\x95\x5F\x2B\x55\x75\x6F\x12\xC8", buffer, 8)) { // mtk dump pack file
		strcpy(name_back, "aee_extract ");
		strcat(name_back, name);
		system(name_back);
		printf("file name: %s\n", name);
		backup_file(name);

		strcat(parse_dir_names, name);
		strcat(parse_dir_names, ".DEC\n");

		return 0;
	}

	return 0;
}

void unpack_dir(const char *name) {
	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(name)))
		return;
	if (!(entry = readdir(dir)))
		return;

	do {
		if (entry->d_type == DT_DIR) {
			char path[MAX_FILE_SIZE];
			int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
			path[len] = 0;
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, dir_backup_name) == 0) {
				//printf("skiped directory: %s\n", entry->d_name);
				continue;
			}
			unpack_dir(path);
			printf("path: %s\n", path);
		}
		else {
			int flag = 0;
			static char name_tmp[MAX_FILE_SIZE] = {0};
			memset(name_tmp, 0, MAX_FILE_SIZE);
			snprintf(name_tmp, sizeof(name_tmp)-1, "%s/%s", name, entry->d_name);
			
			flag = handle_compressed_file(name_tmp, (char *)name, entry->d_name);
			if(flag) {
				//printf("%s\n", name_tmp);
			}
		}
	} while (entry = readdir(dir));
	closedir(dir);
}

int unpack_files(char *dir) {
	backup_subdir_num = 0;

	do {
		todo_flag = 0;
		unpack_dir(dir);
		backup_subdir_num++;
	}while(todo_flag != 0);

	return 0;
}

int dump_file_ascii(char *buf, char *fname, int start, int end) {
	static char line[MAX_LINE_SIZE];
	FILE *fp;
	int line_no = 0;
	
	if(start < 0 || start > end) {
		return -1;
	}

	fp = fopen(fname, "r");
	if(NULL == fp) {
		return -1;
	}

	while(!feof(fp)) {
		if(line_no < start) {
			line_no++;
			fgets(line, MAX_LINE_SIZE, fp); // discard line
			continue;
		}
		else if(line_no >= end) {
			break;
		}

		memset(line, 0, MAX_LINE_SIZE);
		fgets(line, MAX_LINE_SIZE, fp);
		strcat(buf, line);
		line_no++;
		//printf("line: %s\n", line);
	}

	fclose(fp);
	return 0;
}

int save_file_ascii_append(char *buf, char *fname) {
	return 0;
}

int parse_ke(char *dir) {
	static char fname_tmp[MAX_FILE_SIZE];

	// parse ZAEE_LOG file
	memset(fname_tmp, 0, MAX_FILE_SIZE);
	strcpy(fname_tmp, dir);
	strcat(fname_tmp, "/ZAEE_LOG");

	printf("filename: %s\n", fname_tmp);
	memset(file_buf, 0, MAX_FILE_BUF_SIZE);
	dump_file_ascii(file_buf, fname_tmp, 0, 20);
	printf("ZAEE_LOG content: %s\n", file_buf);

	// parse __exp_main.txt file
	memset(fname_tmp, 0, MAX_FILE_SIZE);
	strcpy(fname_tmp, dir);
	strcat(fname_tmp, "/__exp_main.txt");

	memset(file_buf, 0, MAX_FILE_BUF_SIZE);
	dump_file_ascii(file_buf, fname_tmp, 7, 50);
	printf("__exp_main.txt content: %s\n", file_buf);

	return 0;
}

int parse_dump_directories(char *dir) {
	char *start = dir;
	char *end;
	static char dir_tmp[MAX_FILE_SIZE];

	if(NULL == dir) {
		return -1;
	}

	while(start[0] != 0) {
		memset(dir_tmp, 0, MAX_FILE_SIZE);
		end = strchr(start, '\n');
		if(end != NULL) {
			memcpy(dir_tmp, start, end-start);
		}
		else {
			end = start + strlen(start);
			if(end == start) {
				break;
			}
			memcpy(dir_tmp, start, end-start);
		}
		start = end+1;
		parse_ke(dir_tmp);
		// printf("dir: %s\n", dir_tmp);
	}

	return 0;
}

int handle_one_issue(char *dir) {
	memset(dir_backup_full_name, 0, MAX_FILE_SIZE);
	strcpy(dir_backup_full_name, dir);
	strcat(dir_backup_full_name, "/");
	strcat(dir_backup_full_name, dir_backup_name);
	printf("dir_backup_full_name: %s\n", dir_backup_full_name);
	create_dir(dir_backup_full_name);

	memset(parse_dir_names, 0, MAX_DIR_NAME_SIZE);

	unpack_files(dir);

	printf("%s", parse_dir_names);

	parse_dump_directories(parse_dir_names);

	return 0;
}

char *test_str = "/media/disk2/10work/work53_160829/00tmp";
int main(int argc, char **argv) {
	if(argc != 2) {
		printf("parameter error\n"); // todo
		return -1;
	}

	printf("%s\n", argv[1]);
	handle_one_issue(argv[1]);

	return 0;
}
