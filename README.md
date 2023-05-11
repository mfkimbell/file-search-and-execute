# mulitple-file-search-and-execute

The purpose of this project was to further understand file navigation and architecture in C. The program takes in command line arguments and presents different data to the user depending on the flags the user provides. All flags can be used in conjunction with one another to produced more and more limited results for the data output. The flags are described below.

---
**Concepts Covered:**
* stat and dirent, for displaying file information
* structs, for organizing file information and flag handlers
* recursion, for transversing recursively into file heiarchy 
* tokenizaiotn, for seperating command line arguments
* `execvp`, for executing commands passed in from command line
* sybolic links, the program handles files that point to other files

---

`./search`
    This will execute the program from the current directory

`-S` 
    This flag lists all files in the hierarchy and print the size, permissions, 
    and last access time next to the filename in parenthesis. Print 0 for the size of a directory.

`-s <filesize in bytes>`
    This flag lists all files in the hierarchy with file size less than or equal to the value specified.

`-f <string pattern> <depth>`
    This flag lists all files in the hierarchy with the following conditions: 
    1) the file name contains the substring in the string pattern option, and 
    2) the depth of the file relative to the starting directory of the traversal 
    is less than or equal to the depth option. 
    The starting directory itself has a depth of 0. 
  
`-t <f or d>`
  t f - lists regular files only
  t d - lists directories only

`-e "<unix-command with arguments>"` 
    For each file that matches the search criteria the UNIX command specified with 
    arguments are executed after the other search information is dislpayed.

`-E "<unix-command with arguments>"`
    UNIX command is executed only once but uses all the file names as the arguments and 
    is executed after all other information on all other files has been displayed.

The following is an example of the code being run with no flags or commands.
    
    ./search
    

![Screenshot 2023-04-07 at 12 04 47 PM](https://user-images.githubusercontent.com/107063397/230649912-7c3b6faa-17ee-450a-ae15-bd84687a9422.png)

    
Next is an example of a flag that takes in a command line executable argument, here i'm using `ls`, the command is executed before the file is printed, so if a file is a directory, its contents are displayed above it's filename. If a file is not a directory, the path to the file is printed. To be clear ***any unix file can be executed, `ls` is just an example.***
    
```
    ./search -e ls
```
![Screenshot 2023-04-07 at 12 04 20 PM](https://user-images.githubusercontent.com/107063397/230649169-2476d790-7bcb-49c1-87a1-39df8c4f8281.png)
    
In order to execute all of the commands at once after printing the file heiarchy, `-E` flag can be subsituted for the `-e` flag.

<img width="1238" alt="Screenshot 2023-05-11 at 5 28 03 PM" src="https://github.com/mfkimbell/multiple-file-search-and-execute/assets/107063397/024de4d5-fba4-4465-bf40-a7ddfb607ee0">

All flags can be used in conjunction with one another. For example, the `-S` flag adds extra information about the file to the printed heiarchy and the `-s <filesize>` flag only produces output for files within the filesize designated. Here is an example of ***only files less than 30 bytes*** being printed with extra information:

<img width="718" alt="Screenshot 2023-05-11 at 5 38 58 PM" src="https://github.com/mfkimbell/multiple-file-search-and-execute/assets/107063397/68477f05-3d5e-411c-b018-4723f768c5fc">


