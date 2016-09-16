#ifndef  __BL_FILE_H__
#define  __BL_FILE_H__

// get file size
int  bl_get_file_size(char *file_name); // get file size
int  bl_get_file_size_ex(char *file_name);

// file extract
int  bl_file_extract(char *new_name, char *old_name, 
	int start_file_addr, int size);
int bl_file_append(char *file_name,char *append_file_name);

#endif

