/*
* Name: Tai Nguyen, Ryan Stobbe, Branden Wallace
* Account: cssc1218, cssc 1216
* Class: CS570, Summer 2018
* Assignment Info: Assignment #2, File Management
* Filename: menu.c
*/

#include "process.h"


int main(){
	//different processes to handle jobs
	pid_t pid1,
	      pid2,
	      pid3;
	int input;					//option
LOOP:
        printf("\n\t-Menu-\t\n0:Exit Menu\n1:Create File/Directory\n2:Read File\n3:Write File\n4:Status\n5:Dir Listing\nEnter action number [0-5] below:\n");
        scanf("%d",&input);
        flush();


///////////////////////////////////////////////////////////////

        if(input==0){
            printf("Exiting the menu. Goodbye.\n");
		exit(0);
///////////////////////////////////////////////////////////////
        }else if(input==1){					//create new file/directory
 
                printf("0: Exit Sub-Menu\n1: New File\n2: New Directory\n");
                scanf("%d",&input);
                flush();
		

                if(input==0){
                    printf("Exiting Sub-Menu\n");
			goto LOOP;
                }else if(input==1){				//create file
                    create_file();
			goto LOOP;

                }else if(input==2){			//create directory
                    create_directory();
			goto LOOP;
                }else{
                    printf("Not a valid option!\n");
			goto LOOP;
                }
///////////////////////////////////////////////////////////////
        }else if(input==2){				//read file
            get_file_name(file_name);
            pid1 = fork();
            

            if(pid1==0){
                //child1
                strcpy(s_cpy,file_name);
                if((file = fopen(file_name,"r"))==NULL){
                    perror("");
                    exit(1);
                }
                if((file = fopen(strcat(s_cpy,".bak"),"w"))!=NULL){//if the .bak file already existed.
                    remove(s_cpy);			//remove old .bak file
                    file = fopen(s_cpy,"w");	//create new .bak file
                }
                read_file_to_file(file_name,file);	//read from original file to .bak file
                read_file_to_file(file_name,stdout);	//print to terminal
                printf("\n");


            }else if(pid1 > 0){
                wait(NULL);
			goto LOOP;


                }else{
                    // fork failed
                    printf("fork() failed!\n");
                }


///////////////////////////////////////////////////////////////

        }else if(input==3){				//write file
            get_file_name(file_name);
            pid1 = fork();
	    input = 0;
            if(pid1==0){
CHILD_LOOP:
                //in child: this child will make .bak file and copy it to the real file.
		printf("0: Exit Sub-Menu\n1: Insert mode.\n2: Append mode.\n3: Overwrite mode\n");
                scanf("%d",&input);
		flush();
		if(input==0){//exit
			printf("Exiting Sub-Menu\n");
			exit(0);
		}else if(input==1){//insert
			insert_file(file_name);
		}else if(input==2){//append
			append_file(file_name);
		}else if(input==3){//overwrite
			overwrite_file(file_name);
		}else{//not an option
			printf("Not a valid option!\n");
			goto CHILD_LOOP;
		}

            }else if(pid1 >0){
                //in parent
                wait(NULL);			//wait for child
                pid2=fork();
		int reverseSort; //used for decideing which sort is going to be used

                if(pid2==0){
                    //in child2: this child will create another copy and sort alphabetically
		    reverseSort = 0;
		    strcpy(s_cpy,file_name);
                    char s[32] = "sorted-";
                    read_file_to_string(file_name,w_str);
		    sort_string_contents(w_str,reverseSort);
                    file = fopen(strcat(s,s_cpy),"w");
		    
                   // qsort(w_str,strlen(w_str),sizeof(char),cmp);
                    fprintf(file,"%s",w_str);
                    fclose(file);


                }else if(pid2>0){
                    //in parent
                    wait(NULL);
                    pid3=fork();


                    if(pid3==0){
                        //in child3:: this child will create another copy and sort
                        reverseSort = 1;
                        strcpy(s_cpy,file_name);
                        char s[32] = "sortedreverse-";
                        read_file_to_string(file_name,w_str);
                        
		        sort_string_contents(w_str,reverseSort);
			file = fopen(strcat(s,s_cpy),"w");
                        fprintf(file,"%s",w_str);
                        fclose(file);


                    }else if(pid3>0){
                        //in parent
                        wait(NULL);
			goto LOOP;
                    }
                }
                

                }else{
                    // fork failed
                    printf("fork() failed!\n");
            }

///////////////////////////////////////////////////////////////
        }else if(input==4){//file status
            get_file_name(file_name);
            print_file_status();
			goto LOOP;
///////////////////////////////////////////////////////////////
	
        }else if(input==5){				//dir listing
            get_file_name(file_name);
            dir_listing(file_name);
			goto LOOP;
///////////////////////////////////////////////////////////////
       	}else{
            printf("Not a valid option!\n");
			goto LOOP;
        }
///////////////////////////////////////////////////////////////
	exit(0);
}




/*
* Open original file for read,
* create a .bak file and copy the content of original until indicated position to insert.
* insert the text
* copy the rest of the original file to .bak file
* update original file content with new .bak content.
*/
void insert_file(char *filename){
        strcpy(s_cpy,filename);
	int pos = 0;
	int counter=0;
	int file_size=0;
	printf("Position: ");
	scanf("%d",&pos);
	
	flush();
	printf("Write file in insert mode.\n");
	if((file = fopen(filename,"r"))==NULL){
		printf("No file match, creating a new file.\n");
		write_file(strcat(s_cpy,".bak"));
		file = fopen(filename,"w");
		read_file_to_file(s_cpy,file);
	}else{	
		//game plan: open original file to read. 
		//create .bak file to modify. copy .bak back to original
		
		p_file = fopen(filename,"r+");

		fread(w_str,sizeof *w_str,pos,p_file);
		
		if((file = fopen(strcat(s_cpy,".bak"),"a"))!=NULL){//if .bak file already existed
        		remove(s_cpy);
			file = fopen(s_cpy,"a");
		}
		fwrite(w_str,sizeof *w_str,pos,file);
		while((c=getchar())!=EOF){
			putc(c,file);
			counter++;
		}
		fseek(p_file,pos,SEEK_END);
		file_size = ftell(p_file);	//get file size
		if((pos+counter) > file_size){
			printf("\nOUT OF BOUND.\n");
			exit(0);
		}	
		fseek(p_file,pos,SEEK_SET);//set pointer

		fread(w_str,sizeof *w_str,(file_size - pos),p_file);
		fwrite(w_str,sizeof *w_str,(file_size - pos),file);
		fclose(file);
		remove(filename);
		p_file = fopen(filename,"w");
		read_file_to_file(s_cpy,p_file);
	}
}

/*
* make a copy of original file
* modify .bak file
* after finish, update .bak to original
*/
void append_file(char *filename){
        strcpy(s_cpy,filename);
	printf("Write file in append mode.\n");
       	if((p_file = fopen(strcat(s_cpy,".bak"),"w"))!=NULL){//if file already existed
        	remove(s_cpy);
		write_file(s_cpy);
	}else
		write_file(strcat(s_cpy,".bak"));//child write to a .bak file
	p_file = fopen(filename,"a");	//create a new file
	read_file_to_file(s_cpy,p_file);//read from .bak file to original file
}

/*
* Open original file for read,
* create a .bak file and copy the content of original until indicated position to insert.
* insert the text
* copy the rest of the original file  - the work reserved by the text inserted to .bak file
* update original file content with new .bak content.
*/
void overwrite_file(char *filename){
	strcpy(s_cpy,filename);
	int pos = 0;
	int counter=0;
	int file_size=0;
	printf("Write file in overwrite mode.\n");
	if((file = fopen(filename,"r"))==NULL){
		printf("No file match, creating a new file.\n");
		write_file(strcat(s_cpy,".bak"));
		file = fopen(filename,"w");
		read_file_to_file(s_cpy,file);
	}else{
		p_file = fopen(filename,"w");
		fread(w_str,sizeof *w_str,pos,p_file);
		
		if((file = fopen(strcat(s_cpy,".bak"),"a"))!=NULL){//if .bak file already existed
        		remove(s_cpy);
			file = fopen(s_cpy,"a");
		}
		fwrite(w_str,sizeof *w_str,pos,file);
		while((c=getchar())!=EOF){
			putc(c,file);
			counter++;
		}

		fseek(p_file,pos,SEEK_END);
		file_size = ftell(p_file);//get file size
		
		fseek(p_file,(pos+counter),SEEK_SET);//set pointer

		fread(w_str,sizeof *w_str,(file_size - (pos+counter)),p_file);
		fwrite(w_str,sizeof *w_str,(file_size - pos),file);
		fclose(file);
		remove(filename);
		p_file = fopen(filename,"w");
		read_file_to_file(s_cpy,p_file);
	}
}
//Get the name of the file you are about to create
void get_file_name(char str[256]){
	printf("file name: ");
	scanf("%[^\n]s", str);
	flush();
}
//read the content of one file to another
void read_file_to_file(char *name,FILE *to){
	file=fopen(name,"r");
	if(file==NULL)
		perror("");
	else{
		while (( c = getc(file)) != EOF)
			putc(c,to);
		fclose(file);
	}	
}
void read_file_to_string(char *name,char *arr){
	file=fopen(name,"r");
	if(file==NULL)
		perror("");
	else{
		int i=0;
		while (( c = getc(file)) != EOF){
			arr[i]=c;
			i++;
		}
		arr[i]='\0';
		fclose(file);
	}	
}
void write_file(char *name){
	file=fopen(name,"w");
	while((c=getchar())!=EOF){
		putc(c,file);
	}
	fclose(file);
}
int cmp(const void *a,const void*b){
    const char** aa = (const char**)a;
    const char** bb = (const char**)b;
    return strcmp(*aa,* bb);
}

//flushes the input buffer
void flush(){
    while((getchar()) != '\n');
}
int create_file(){
    get_file_name(file_name);
    file=fopen(file_name,"w");
    fclose(file);
    return 0;
}
int create_directory(){
    get_file_name(file_name);
    mkdir(file_name, S_IRWXU);
    return 0;
}
//lists the contents of the directory
void dir_listing(char *name){
	//if the directory exists
	if((dir = opendir(name))!=NULL){
		//cycle through all the files in the directory
		while ((ent = readdir (dir)) != NULL)
			printf("%s\n",ent->d_name);
		(void) closedir(dir);
	}else{
		printf("Invalid directory.\n");
	}
}
//print the status of a file with input==4
int print_file_status(){
    int file_status = 0;
    struct stat st;
    file_status = open(file_name,O_RDONLY);
    
    //Check if open() was successful
    if(-1 == file_status)
    {
        printf("\n NULL File descriptor\n");
        return -1;
    }
    
    // set the errno to default value
    errno = 0;
    // Now a call to fstat is made
    // Note that the address of struct stat object
    // is passed as the second argument
    if(fstat(file_status, &st))
    {
        printf("\nfstat error: [%s]\n",strerror(errno));
        close(file_status);
        return -1;
    }
    
    printf("\n\n The specified file is a ");
    //check the flags for the file type
    switch (st.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break;
        case S_IFCHR:  printf("character device\n");        break;
        case S_IFDIR:  printf("directory\n");               break;
        case S_IFIFO:  printf("FIFO/pipe\n");               break;
        case S_IFLNK:  printf("symlink\n");                 break;
        case S_IFREG:  printf("regular file\n");            break;
        case S_IFSOCK: printf("socket\n");                  break;
        default:       printf("unknown?\n");                break;
    }
    
    printf(" File size:                %lld bytes\n",(long long) st.st_size);
    printf(" Last status change:       %s", ctime(&st.st_ctime));
    printf(" Last file access:         %s", ctime(&st.st_atime));
    printf(" Last file modification:   %s", ctime(&st.st_mtime));
    
    printf("\n The userID of the owner of the file is %d.\n The groupID of the owner of the file is %d.\n\n\n", (int)st.st_uid, (int)st.st_gid);
    
    // Close the file
    close(file_status);
    return 0;
}

void sort_string_contents(char *arr, int reverseTrue)
{
	char delimeters[]= " .,:;!\n";
	char arr_copy[256];
	char *token_array[256];
	strncpy(arr_copy, arr, 255); //direct copy of original string to be butchered
	char *token;
	int i = 0;
	token = strtok(arr_copy, delimeters);
	while(token != NULL)
	{
		token_array[i] = token;
		token = strtok(NULL, delimeters);
		i++;	
	}
	qsort(token_array, i, sizeof(char*), cmp);
	char* str = arr;
	int count = 0;
	int j;
	if(reverseTrue == 0) // will sort in acending order
	{
		for(j = 0;j<i; j++)
		{
		count = sprintf(str, "%s\n", token_array[j]);
		str+=count;
		}
	}
	else if(reverseTrue == 1) // will sort in reverse order
	{
		for(j = i-1;j>=0; j--)
		{
		count = sprintf(str, "%s\n", token_array[j]);
		str+=count;
		}	
	}
	else
	{
		perror("No sort order selected.");
	}
}



