./search 
    This will execute the program from the current directory

-S  
    This flag lists all files in the hierarchy and print the size, permissions, 
    and last access time next to the filename in parenthesis. Print 0 for the size of a directory.

-s <filesize in bytes> 
    This flag lists all files in the hierarchy with file size less than or equal to the value specified.

-f <string pattern> <depth>
    This flag lists all files in the hierarchy with the following conditions: 
    1) the file name contains the substring in the string pattern option, and 
    2) the depth of the file relative to the starting directory of the traversal 
    is less than or equal to the depth option. 
    The starting directory itself has a depth of 0.
  
-t <f or d>
  t f - lists regular files only
  t d - lists directories only

-e "<unix-command with arguments>" 
    For each file that matches the search criteria the UNIX command specified with 
    arguments are executed after the other search information is dislpayed.

-E "<unix-command with arguments>" [Graduate Students Only] 
    UNIX command is executed only once but uses all the file names as the arguments and 
    is executed after all other information on all other files has been displayed.
    
    `./search`
    
    ![Screenshot 2023-04-07 at 12 04 20 PM](https://user-images.githubusercontent.com/107063397/230648795-444050cc-e015-4fc9-b51a-bcff3079aa7b.png)
