/*
Name: Mitchell Kimbell
BlazerId: mkimbell
Project #: 3
To compile: "make build" 
To run: 

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
    is executed after all other information on all other files has been displayed
  
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/time.h>


char* superArr[1000];
int index2=0;




//this holds all of the flag information
struct flags
{
  int Sflag;
  int sflag;
  int sval;
  int fflag;
  char* fpattern;
  int fdepth;
  int tflag;
  char* tval;
  int eflag;
  char* eval;
  int Eflag;
  char* Eval;
};

void traverseDirectoryFull(char *path, int tabSpaces, struct flags flags);

#define MAX_PATH_SIZE 2000

//I implemented this function to get the file type from a stat
//It might not be essential, but it solved an important problem for me
char * filetypestat(struct stat statbuf) {
  char *filetypestat = (char*) malloc((40)*sizeof(char));
  switch(statbuf.st_mode & S_IFMT){
          case S_IFREG:
              strcpy(filetypestat,"Regular File");
              return filetypestat;
              break;
          case S_IFDIR:
              printf("Directory\n");
              strcpy(filetypestat,"Directory");
              return filetypestat;
              break;
          case S_IFBLK:
              printf("Block Device\n");
              strcpy(filetypestat,"Block Device");
              return filetypestat;
              break;
          case S_IFCHR:
              printf("Character Device\n");
              strcpy(filetypestat,"Character Device");
              return filetypestat;
              break;
          case S_IFSOCK:
              printf("Socket\n");
              strcpy(filetypestat,"Socket");
              return filetypestat;
              break;
          case S_IFLNK:
              printf("Symbolic Link\n");
              strcpy(filetypestat,"Symbolic Link");
              return filetypestat;
              break;
    }
}

//returns the file type in easy to understand string format
char *filetype(unsigned char type) {
  char *str;
  switch(type) {
  case DT_BLK: str = "block device"; break;
  case DT_CHR: str = "character device"; break;
  case DT_DIR: str = "directory"; break;
  case DT_FIFO: str = "named pipe (FIFO)"; break;
  case DT_LNK: str = "symbolic link"; break;
  case DT_REG: str = "regular file"; break;
  case DT_SOCK: str = "UNIX domain socket"; break;
  case DT_UNKNOWN: str = "unknown file type"; break;
  default: str = "UNKNOWN";
  }
  return str;
}

typedef void BONUS(char* path, int tabSpaces, struct flags flags);

void BONUSfunction(char* path, int tabSpaces, struct flags flags, BONUS *pointervalue) 
{
  pointervalue(path, tabSpaces, flags);
}

//takes in directory and a struct of flags and prints out information based on the specifications of the flags
//if no argument for path is inputted, it should default to the current working directory
void traverseDirectoryFull(char *path, int tabSpaces, struct flags flags) {
    
    

    struct dirent *dirent;
    DIR *parentDir;

    // First, we need to open the directory.
    parentDir = opendir(path);
    if (parentDir == NULL) { 
        printf ("Error opening directory '%s'\n", path); 
        exit (-1);
    }
    int count = 1; 
    int currentDepth=0;
    // After we open the directory, we can read the contents of the directory, file by file.
    while((dirent = readdir(parentDir)) != NULL){ 

        

        // If the file's name is "." or "..", ignore them. We do not want to infinitely recurse.
        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) {
            continue;
        }

        //implementing symbolic link if (filetype(dirent->d_type) strcmp("symbolic link"). == 0, replace that output with the file pointed to
      
        char pathcopy[40];
        strcpy(pathcopy,path);
        char fullpath[40];
        char *fullpointer;
        fullpointer = fullpath;
        fullpointer = strcat(pathcopy,"/");
        fullpointer = strcat(fullpointer, dirent->d_name);
        

        struct stat statbuf2;
        struct stat statbuf3;
        stat(fullpointer, &statbuf2);
        lstat(fullpointer, &statbuf3);
        
        char* filename = dirent->d_name;

        //handling t flag
        if (flags.tflag!=1 || ((strcmp(flags.tval,"d")==0) && (strcmp(filetype(dirent->d_type),"directory")==0)) || ((strcmp(flags.tval,"f")==0) && (strcmp(filetype(dirent->d_type),"regular file")==0))){
        
        //handling f flag
        if (flags.fflag!=1 || strstr(filename,flags.fpattern)!=NULL) {

        //handling s flag
        if (flags.sflag!=1 || statbuf2.st_size>flags.sval) 
        {   //symbolic link handling
            if (strcmp(filetype(dirent->d_type),"symbolic link")==0)
            {
              //storing the name of the link pointed to by the symbolic link
              char buffer[1024];
              ssize_t link_string_length;
              if ((link_string_length = readlink(filename, buffer, 1023)) == -1)
              {
                  printf("error\n");
              }
              else
              {
                  // Make sure that the buffer is terminated as a string
                  buffer[link_string_length] = '\0';
        }

              //printing the value of the symbolic link and the file it points to
              printf("%*s[%d] %s (%s)", 4 * tabSpaces, " ", count, dirent->d_name, buffer);
            } else{
              //printin the default information
            printf("%*s[%d] %s (%s)", 4 * tabSpaces, " ", count, dirent->d_name, filetype(dirent->d_type));
            }

        //handling S flag
        if (flags.Sflag==1)
        {
          
          if (strcmp(filetype(dirent->d_type),"directory")==0)
          {
            printf("        (Size: 00");
          } else {
          printf("        Size: %ld",statbuf2.st_size);
          }
          //knowledge comes form this website
          //https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
          printf(" FP: ");
          printf( (S_ISDIR(statbuf2.st_mode)) ? "d" : "-");
          printf( (statbuf2.st_mode & S_IRUSR) ? "r" : "-");
          printf( (statbuf2.st_mode & S_IWUSR) ? "w" : "-");
          printf( (statbuf2.st_mode & S_IXUSR) ? "x" : "-");
          printf( (statbuf2.st_mode & S_IRGRP) ? "r" : "-");
          printf( (statbuf2.st_mode & S_IWGRP) ? "w" : "-");
          printf( (statbuf2.st_mode & S_IXGRP) ? "x" : "-");
          printf( (statbuf2.st_mode & S_IROTH) ? "r" : "-");
          printf( (statbuf2.st_mode & S_IWOTH) ? "w" : "-");
          printf( (statbuf2.st_mode & S_IXOTH) ? "x" : "-");
          printf(" LA: %s",ctime(&statbuf2.st_atime));

        }
        int index=0;
        if (flags.eflag==1)
        {
          
          printf("FLAGS: %s\n", flags.eval);
          char copy[30];
          strcpy(copy,flags.eval);
          char* argArray[10];
          char* token = strtok(copy," ");
          printf("token: %s\n", token);
          
          argArray[index]=token;
          index++;
          while (token != NULL)
		    {
          token=strtok(NULL," ");
          printf("token: %s\n", token);
          argArray[index]=token;
          index++;
        }
        argArray[index-1]=fullpointer;
        
        argArray[index]=NULL;

        printf("argarr 0: %s\n", argArray[0]);
		    printf("argarr 1: %s\n", argArray[1]);
        printf("argarr 2: %s\n", argArray[2]);
        printf("argarr 3: %s\n", argArray[3]);
        
        int pid;
        pid = fork();
        if (pid == 0)
        {
        execvp(argArray[0], argArray);
        } else 
        {
          int status;
          wait(&status);
        }

        }
        if (flags.Eflag==1)
        {
          superArr[index2]=strdup(fullpointer);
          index2++;
        }

        }
        }
        }
        

        printf("\n");
        fullpointer = path;

        count++; 

        // Check to see if the file type is a directory. If it is, recursively call traverseDirectory on it.
        if (dirent->d_type == DT_DIR) {
            char *subDirPath = (char *) malloc(MAX_PATH_SIZE);
            strcpy(subDirPath, path);
            strcat(subDirPath, "/");
            strcat(subDirPath, dirent->d_name);
            if (flags.fdepth>currentDepth)
            {
            currentDepth+=1;
            traverseDirectoryFull(subDirPath, tabSpaces + 1, flags);
            }
            
        }
    } 
  
  
}


int main (int argc, char **argv)

{
  
  int symlink(const char *pathname, const char *slink);



  char file1[]="linked";
  char file2[]="symbolic";

  symlink(file1,file2);

  
    

  int Sflag = 0;

  int sflag = 0;
  int sval = -1;
  int tflag = 0;
  char *tval = NULL;

  int fflag = 0;
  char *fpattern = NULL;
  int fdepth = 90;
  int argument = 0;
  
  int eflag = 0;
  char *eval = NULL;
  int Eflag = 0;
  char * Eval = NULL;



  // int tFlag = 0; unsure
  // int dFlag = 0;


  //https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
  //basing all my knowledge off this website, need help

  //this while loop searches through every flag as well as the information passed after
  while ((argument = getopt (argc, argv, "Ss:f:t:e:E:")) != -1) 
    switch (argument)
    {
      case 'S':
        Sflag = 1;
        break;
      case 's':
        sflag = 1;
        sval = atoi(optarg);
     

        break;
      case 'f':
        fflag = 1;
        char* strtosplit = optarg;
        char* token = strtok(strtosplit," ");
        char* token1 = token;
        token = strtok(NULL, " ");
        char* token2 = token;
        // printf("token1: %s, token2: %s\n", token1, token2);
        fpattern = token1;
        fdepth = atoi(token2); 
        break;
      case 't':
        tflag = 1;
        tval = optarg;
        break;
      case 'e':
        eflag = 1;
        eval = optarg;
        break;
      case 'E':
        Eflag = 1;
        Eval = optarg;
        break;
      case '?':
        if (optopt == 's' || optopt == 'f' || optopt == 't' || optopt == 'e' || optopt == 'E')
        {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        }
        else if (isprint (optopt))
        {
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        }
        else
        {
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        }
      
      default:
        abort();
    }



//initalizing and filling struct
struct flags flags;
flags.Sflag=Sflag;
flags.sflag=sflag;
flags.sval=sval;
flags.fflag=fflag;
flags.fpattern=fpattern;
flags.fdepth=fdepth;
flags.tflag=tflag;
flags.tval=tval;
flags.eval=eval;
flags.eflag=eflag;
flags.Eval=Eval;
flags.Eflag=Eflag;

struct stat statbuf;


//https://aljensencprogramming.wordpress.com/tag/st_mode/
char *cwd;
cwd=(char *)malloc(100*sizeof(char));

getcwd(cwd,100);

fflush(stdout);

printf("Argument inputs:\n");
for (int i=0; i<argc; i++){
  
  printf("argv %d:%s\n",i,argv[i]);
}
char *filename=argv[argc-1];


stat(filename, &statbuf);



char* pointer=filetypestat(statbuf);


if (pointer==NULL) {
  
} else{
if (strcmp(pointer,"Directory")==0)
{ 
  strcat(cwd,"/");
  strcat(cwd,argv[argc-1]);
  printf("New cwd: %s",cwd);
}}






printf("\ncwd: %s \n\n",cwd);
int tabSpaces = 0;

  
  if (flags.Eflag==1)
        {
          
          printf("FLAGS: %s\n", flags.Eval);
          char copy[30];
          strcpy(copy,flags.Eval);
          char* argArray[10];
          char* token = strtok(copy," ");
          printf("token: %s\n", token);
          
          superArr[index2]=token;
          index2++;
          while (token != NULL)
		    {
          token=strtok(NULL," ");
          printf("token: %s\n", token);
          superArr[index2]=token;
          index2++;
        }
        index2--;
        }

  BONUSfunction(cwd, tabSpaces, flags, traverseDirectoryFull);
  //this is what you would call if you weren't using a function pointer:
  // traverseDirectoryFull(cwd, tabSpaces, flags);

  index2++;
  superArr[index2]=NULL;
  printf("superArr[0] %s\n", superArr[0]);
  printf("superArr[1] %s\n", superArr[1]);
  printf("superArr[2] %s\n", superArr[2]);
  printf("superArr[3] %s\n", superArr[3]);
  printf("superArr[4] %s\n", superArr[4]);

  execvp(superArr[0],superArr);
  
 
  
}