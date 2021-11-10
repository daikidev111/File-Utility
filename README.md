# File-Utility


This document covers instructions on how to use 30523346_fileutil.c program.

Author: Daiki Kubo

# Starter

How to compile the 30523346_fileutil.c file:

gcc -o fileutil 30523346_fileutil.c



*Make sure to have sample.txt in the same folder as 30523346_fileutil.c file

To run(Please note that this should be done in the same folder as 30523346_fileutil.c file):

./fileutil 

This will output the first 10 lines of sample.txt 



# Important
The argument following after ./fileutil should be the absolute path of the specified file if you want to read from the specified file. If a path or incorrect path is provided, it reads sample.txt.



# Instruction


1. Reading the first 10 lines of sample.txt or specified absolute path 

	Syntax:

		./fileutil [absolute_file_path]

	example:

		./fileutil /home/student/dir1/a.txt 

			This will output the first 10 lines of a.txt file 



2. -n [number]

	Syntax:

		./fileutil [absolute_file_path] -n [number]

	Example:

		./fileutil -n 20

			This outputs the first 20 lines of sample.txt

		./fileutil /home/student/dir1/a.txt -n 15

			This outputs the first 15 lines of a.txt



	Important:

		Integer number needs to be provided after -n.

		-n [number] is not fixed position, meaning you can place it anywhere as long

		as it is not placed following after -n or -d command. 



3. -d [folder]

	Syntax:

		./fileutil [absolute_file_path] -d [Folder Path]

	Example:

		./fileutil -d /home/student/dir2/

			Copy sample.text to the /home/student/dir2/ directory path

		./fileutil /home/student/dir1/a.txt -d /home/student/dir2/

			Copy a.txt to the /home/student/dir2/

	Important:

		The [Folder path] must end with / line. E.g. /home/student/dir2 is incorrect path.

		If the same file exists in the [folder path] then it will not copy to the folder path.

		The folder path must come following after -d to copy. 



4. -L

	Syntax:

		./fileutil [absolute_file_path] -L 

	Example:

		./fileutil -L

			Display the last 10 lines of sample.txt

		./fileutil /home/student/dir1/a.txt -n 15 -L

			Display the last 15 lines of a.txt

	Important:

		-L is not fixed position, meaning you can place it anywhere as long as it is not placed following 

		after -n or -d.

