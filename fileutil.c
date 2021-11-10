/*
 * Author: Daiki Kubo
 * Student ID: 30523346
 * Description of the program:
 * The program has a multiple file utility function, which is combined from
 * LINUX utilities, such as tail, cat, and cp.
 * Start Date: 22nd of August
 * Last modified: 27th of August 20:00pm
 */

/* List of libraries used */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

/* List of declared functions*/
int writeContent(int fd, int s_file_size, int count, int copy_file, int l_valid);
int checkFileExists(char *path);
int retrieveFileSize(char *s_path); 
int openFile(char *path);
int checkNextArgvExist(char **argv, int next_index);
int copyToDestDir(char **argv, int s_file_exist, int next_index);
int retrieveNumLines(char **argv, int next_index);

int main(int argc, char *argv[]) {
	/* 
	 * Function: main
	 * --------------
	 *  Function description:
	 *  This function loops through to find all the necessary operations 
	 *  given from the command line and call relevant functions.
	 *  Specifically, it loops through all the command line given to find all the relavant operations through the use of validator variables
	 *  and strcmp function. 
	 *
	 *  List of parameters:
	 *  argc: number of arguments entered in the command line when calling the program.
	 *  *argv[]: a list of arguments entered in the command line when calling the program.
	 *  
	 *  Return:
	 *  returns: This function returns 0, which indicates that the program runs successfully.
	 */
	int argc_counter, fd, i;
	char *tail = "-L";
	char *destdir = "-d";
	char *numlines = "-n";
	char *path; // path name for -d command 
	char *file_name; // to retrieve file name of the specified source file for -d
	int slash = 47; // 47 is the ascii value of "/" and used for checking if it has a valid folder path

	int n_num = 10; // default is set to 10.
	int l_valid = 0;
	int d_valid = 0;
	int next_index = 0;
	int s_file_valid = 0;
	int s_file, d_file, s_file_size;

	if (argc == 1) { // if the source file at argc = 1 is not specified, open sample.txt
		fd = openFile("sample.txt");
		s_file_size = retrieveFileSize("sample.txt");
		writeContent(fd, s_file_size, 10, 1, 0);
	} else { // or else open the specified source file
		for (i = 1; i < argc; ++i) {
			if (i == 1) { // source file is always located at argv[1]

				//check if the file exist 
				if (checkFileExists(argv[i]) == 1) {
					s_file = openFile(argv[i]);
					s_file_size = retrieveFileSize(argv[i]);
					s_file_valid = 1; 

				// if it does not exist then assign to 0
				} else {
					s_file_valid = 0;
					s_file = openFile("sample.txt");
					s_file_size = retrieveFileSize("sample.txt");
				}
			}

			//check if numlines exist in the command line 
			if (strcmp(argv[i], numlines) == 0) {
				next_index = i + 1;
				checkNextArgvExist(argv, next_index); // check if the next index of the argv exist or not
				n_num = retrieveNumLines(argv, next_index);

				//after that, re-initialize next_index to 0 
				next_index = 0;
			}

			//check if tail exist
			if (strcmp(argv[i], tail) == 0) {
				l_valid = 1; 
			}

			// check if -d exists
			if (strcmp(argv[i], destdir) == 0) {
				d_valid = 1;
				next_index = i + 1;
				checkNextArgvExist(argv, next_index); 
				if (slash != argv[next_index][0]) { // if the ascii value of "/" at the start of the folder path is not specified
					write(2, "Invalid folder path \n", 22);
					exit(1);
				}

				// retrieve the fd of the destination folder path with file name combined
				d_file = copyToDestDir(argv, s_file_valid, next_index); 

				next_index = 0;
			}
		}

		if (d_valid == 0) { //if the -d was not a part of the command lines 
			d_file = 1; // make it 1 so that it writes to the system in write
		}

		// call the function to write/print content of the file to the system or to destination file.
		writeContent(s_file, s_file_size, n_num, d_file, l_valid); 
	}

	return 0;
}

int retrieveNumLines(char **argv, int next_index) {
	/*
	 * Function: retrieveNumLines
	 * ---------------------------
	 *  Function Description:
	 *  This function retrieves a number of lines specified following after the -n command.
	 *  Since the number of lines is specified at the next index of the position where -n is called, 
	 *  we use atoi to convert the number into int data type. 
	 *  
	 *  List of parameters:
	 *  **argv: This is the array of char pointers that hold arguments from the comand line.
	 *  next_index: the next index position of the index at where the -n command was found.
	 *  
	 *  Return:
	 *  returns: number of lines to retrieve
	 */
	int n_num;
	n_num = atoi(argv[next_index]); // convert it into int type

	if (n_num == 0) { // if it is not a number or 0 then the value of n_num would be 0 
		write(2, "Invalid Number or 0 was entered \n", 33);
		exit(1);
	}

	return n_num;
}

int copyToDestDir(char **argv, int s_file_exist, int next_index) {
	/*
	 * Function: copyToDestDir
	 * ----------------------
	 *  Function Description:
	 *  it concatenates the folder path given with the source file name through the use of dynamic memory allocation and
	 *  strcat. We use s_file_exist variable to check if the source file is mentioned or not in the comand line and 
	 *  if it is not mentioned then we concatenate the retrieved folder path following after -d command with sample.txt, or else 
	 *  we concatenate the source file name speicifed with the path folder.
	 *
	 *  List of parameters:
	 *  **argv: This is the array of char pointers that hold arguments from the comand line.
	 *  s_file_exist: This is the validator variable that determines if the source file is mentioned or not. 
	 *  next_index: the next index position of the index at where the -n command was found.
	 *
	 *  Return value: 
	 *  returns: d_file, which is the file descriptor of the path constructed in the function.
	 */

	char *path;
	int d_file;
	char *file_name;

	if (s_file_exist == 0) { //if the source file is not mentioned

		//dynamically allocate the memory so that the memory size can be flexible for the path length
		char *path = (char *) malloc(sizeof(char) * (strlen("sample.txt") + strlen(argv[next_index])) + 1);
		strcpy(path, argv[next_index]);
		strcat(path, "sample.txt"); 

		//if the file does not exist then 
		if (checkFileExists(path) == 0) {
			//create new file
			if ((d_file = open(path, O_WRONLY | O_CREAT, 0664)) < 0) {
				write(2, "Unable to open the text file \n", 29);
				free(path);
				exit(1);
			}
			free(path);
		} else { // if the file exists in the destination directory then return an error.
			write(2, "File already exists in the folder path \n", 40);
			free(path);
			exit(1);
		}
	} else { //if the s_file is specified

		char *path = (char *) malloc(sizeof(char) * (strlen(argv[1]) + strlen(argv[next_index])) + 1);
		strcpy(path, argv[next_index]);
		
		//retrieve the base name of the specified source file name from the absolute path given 
		file_name = basename(argv[1]);
		
		strcat(path, file_name);

		if (checkFileExists(path) == 0) {
			if ((d_file = open(path, O_WRONLY | O_CREAT, 0664)) < 0) {
				write(2, "Unable to open the text file \n", 29);
				free(path);
				exit(1);
			}
			free(path); //once done, free the memory 
		} else { 
			write(2, "File already exists in the folder path \n", 40);
			free(path);
			exit(1);
		}
	}
	return d_file;
}

int checkNextArgvExist(char **argv, int next_index) {
	/*
	 * Function checkNextArgvExist
	 * ---------------------------
	 *  File Description:
	 *  This function is to check if the value that the argv holds at the next index of the index at where the command, such as -n 
	 *  exist or not. If it does not exist then it means that we cannot initate the command. Hence, we terminate the process with an
	 *  error message.
	 *
	 *  List of parameters:
	 *  **argv: This is the array of char pointers that hold arguments from the comand line.
	 *  next_index: the next index position of the index at where the -n command was found.
	 *
	 *  Return:
	 *  returns: it returns 0 that indicates the argument at the next index exists
	 */
	if (!argv[next_index]) {
		write(2, "An argument following after the command -n or -d does not exist \n", 66);
		exit(1);
	}
	return 0;
}

int openFile(char *path) {
	/*
	 * Function openFile
	 * ------------------
	 * File Description:
	 * This function retrives the file descriptor of the abosolute path passed from the parameter by opening the path.  
	 * 
	 * List of parameters:
	 * *path: this is the char array that holds the path. This is absolute path. 
	 *
	 * Return:
	 * returns: return the file descriptor if the function proceeds successfully.
	 */
	int fd; 
	if ((fd = open(path, O_RDONLY)) < 0) {
		write(2, "Unable to open the text file \n", 29);
		exit(1);
	}
	return fd;
}

int retrieveFileSize(char *s_path) {
	/*
	 * Function retrieveFileSize
	 * -------------------------
	 * File Description:
	 * This function determines the size of the path passed from the parameter by opeining the file and using lseek 
	 * to the end of the file with no offset. After all the operations, we close the file to prevent from reading 
	 * from the end when the fd is used again in other operatinos. 
	 *
	 * List of parameters:
	 * *s_path: this is the char array that holds the path. This is absolute path. 
	 *
	 * Return:
	 * returns: return the size of the path opened. 
	 */
	int fd;
	int size;
	if ((fd = open(s_path, O_RDONLY)) < 0) {
		write(2, "Unable to open the text file \n", 29);
		exit(1);
	}
	size = lseek(fd, 0, SEEK_END);
	close(fd);
	return size;
}
 
int checkFileExists(char *path) {
	/*
	 * Function checkFileExists
	 * ------------------
	 * File Description:
	 * This function determines if the path passed from the parameter exists or not by opening the path.
	 * This way, we can check if the path already exists for a command, such as -d.
	 *
	 * List of parameters:
	 * *path: this is the char array that holds the path. This is absolute path. 
	 *
	 * Return:
	 * returns: return 1 if the path exists or return 0 if the file does not exist.
	 */

	if (open(path, O_RDONLY) >= 0) { //file exists
		return 1;
	} else { //file does not exist
		return 0;
	}
}


int writeContent(int fd, int s_file_size, int count, int copy_file, int l_valid) {
	/*
	 * Function printContent
	 * -----------------------
	 * Function Description:
	 * This function initially creates a buffer of the file size and use the read operation 
	 * to read up to the end of the file from fd into buffer. Then, it decrements the 
	 * count number each time "\n" appears in the file from fd in the loop. Once it reaches to 0 or less, then 
	 * it retrives the number of bytes read at the termination and call the write operation to write upto the size of bytes 
	 * read in the loop. This is when the -L is not valid.
	 * On the other hand, when -L is valid (this is determined through the validator variable passed from l_valid parameter) 
	 * it obtains the total number of lines and subtract the number of line for output from the total number of lines in the file.
	 * The remaining number of lines will be decremented each time "\n" appears in the file from the fd in the loop. 
	 * Then once it reachs to 0 or less, then it retrieves the number of bytes read at the termination and use lseek from the 
	 * start with offset being the byte size retrieved from the loop operation then call the write operation to write to 
	 * the destination folder or the system terminal.
	 *
	 * List of parameters:
	 * fd: file descriptor of the source path 
	 * s_file_size: the file size of the source path 
	 * count: line numbers specified from the number following after -n command.
	 * l_valid: validator variable to determine if -L command is valid or not.
	 *
	 * Return:
	 * return: it returns 0 if the function proceeds correctly. Otherwise, it displays an error message 
	 * and terminate the program.
	 */

	char buffer[s_file_size]; // create a buffer of the file size 
	int i, j, k;
	int bytes_read;
	int n_line_count = 0; // for finding the total number of n lines in the file 
	int byte_count = 0; // to determine the byte size for the offset 

	bytes_read = read(fd, buffer, s_file_size); // reading the byte size 
	if (l_valid == 0) { // if -L is not valid 
		for (i = 0; i < bytes_read; ++i) {
			if ((int) buffer[i] == 10) { // Ascii of "\n" is 10 and if it matches
				count-=1; 
			}
			if (count <= 0) { // once it reaches to 0 or less
				bytes_read = i; // copy i index of the loop into bytes_read variable 
				break;
			}
		}
	} else { // if -L is valid 
		for (j = 0; j < bytes_read; ++j) {
			if (((int) buffer[j] == 10)) {
				n_line_count += 1;
			}
		}
		
		//finding the remaining n_line.
		int remaining_line_num = n_line_count - count;
		
		if (remaining_line_num > 0) { // if the remaining line numbers are larger than 0

			//decrementing the remaining n line size each time it finds \n and if it becomes 0 then determine 
			//the byte_count which is the ith index of the buffer.
			for (k = 0; k < bytes_read; ++k) {
				if ((int) buffer[k] == 10) {
					remaining_line_num -= 1;
					if (remaining_line_num <= 0) {
						byte_count = k;
						break;
					}
				}
			}

			//seek from the start with the offset of the byte_count obtained above 
			lseek(fd, byte_count + 1, SEEK_SET);  // +1 to avoid the empty line at the top of the file
			// re-load the buffer with the lseek position. 
			bytes_read = read(fd, buffer, s_file_size - byte_count);
		}
	}

	// write into the copy_file up to the bytes_read size. If it is 1, then it writes into the system terminal.
	write(copy_file, buffer, bytes_read);  
    
	if (copy_file != 1)  { // if the copy_file is not 1, meaning that the destination path is specified
		write(1, "Copied Successfully! \n", 24);
		close(copy_file); // close the file 
	}

	close(fd);
	return 0;
}

