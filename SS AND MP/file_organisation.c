#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct File{
	char name[20];
	struct File *next;
};

typedef struct File File;

struct Directory {
	char name[20];
	struct Directory *parent;
	struct Directory *subdirs;
	struct Directory *next;
	File *files;
};

typedef struct Directory Directory;

Directory* create_dir(char *name){
	Directory *new_dir = (Directory*)malloc(sizeof(Directory));
	strcpy(new_dir->name,name);
	new_dir->parent = NULL;
	new_dir->subdirs = NULL;
	new_dir->next = NULL;
	new_dir->files = NULL;
	return new_dir;
}

File* create_file(char *name){
	File *new_file = (File*)malloc(sizeof(File));
	strcpy(new_file->name,name);
	new_file->next = NULL;
	return new_file;
}

void add_subdir(Directory *parent,Directory *subdir){
	subdir->parent = parent;
	subdir->next = parent->subdirs;
	parent->subdirs = subdir;
}

void add_file(Directory *dir,File *file){
	file->next = dir->files;
	dir->files = file;
}

Directory* find_dir(Directory *dir,char *dir_name){
	Directory *current = dir->subdirs;
	while(current!=NULL){
		if(strcmp(current->name,dir_name)==0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

File* find_file(Directory *dir,char *filename){
	File *current = dir->files;
	while(current!=NULL){
		if(strcmp(current->name,filename)==0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void list_dir(Directory *dir){
	Directory *current = dir->subdirs;
	File *files = dir->files;
	while(current!=NULL){
		printf("[dir]\t%s\n",current->name);
		current = current->next;
	}
	
	while(files!=NULL){
		printf("[file]\t%s\n",files->name);
		files = files->next;
	}
}

int name_check(Directory *dir,char *name){
	Directory *current = dir->subdirs;
	File *files = dir->files;
	while(current!=NULL){
		if(strcmp(current->name,name)==0)
			return 0;
		current = current->next;
	}
	
	while(files!=NULL){
		if(strcmp(files->name,name)==0)
			return 0;
		files = files->next;
	}
	return 1;
}

void delete_dir(Directory *dir){
	Directory *parent = dir->parent;
	Directory *current = parent->subdirs;
	Directory *prev = NULL;
	while(current!=NULL){
		if(current == dir){
			break;
		}
		prev = current;
		current = current->next;
	}
	if(current==NULL)
		return;
	else if(prev != NULL){
		prev->next = dir->next;
	}
	else{
		parent->subdirs = dir->next;
	}
}

void delete_file(Directory *dir,File *file){
	File *files = dir->files;
	File *prev = NULL;
	while(files!=NULL){
		if(files == file){
			break;
		}
		prev = files;
		files = files->next;
	}
	if(files==NULL)
		return;
	else if(prev != NULL){
		prev->next = file->next;
	}
	else{
		dir->files = file->next;
	}
}

void prompt(Directory *dir){
	if(dir->parent!=NULL) prompt(dir->parent);
	printf("%s/",dir->name);
}

void printPrompt(Directory *dir){
	if(dir->parent!=NULL) prompt(dir->parent);
	printf("%s$ ",dir->name);
}

void heirarchial(){
	char command[50];
	char path[50];
	char input[100];
	
	Directory *root = create_dir("root");
	Directory *current_dir = root;
	
	while(1){
		printPrompt(current_dir);
		fgets(input,sizeof(input),stdin);
		int count = sscanf(input,"%49s %49s",command,path);
		
		if(strcmp(command,"cd")==0){
			if(strcmp(path,"..")==0){
				if(current_dir->parent!=NULL) current_dir = current_dir->parent;
			}
			else{
				Directory *dir = find_dir(current_dir,path);
				if(dir!=NULL)
					current_dir = dir;
				else
					printf("directory not found\n");
								
			}
			
		}
		else if(strcmp(command,"ls")==0){
			list_dir(current_dir);
		}
		else if(strcmp(command,"touch")==0){
			if(name_check(current_dir,path))
				add_file(current_dir,create_file(path));
			else
				printf("filename already exists\n");
		}
		else if(strcmp(command,"mkdir")==0){
			if(name_check(current_dir,path))
				add_subdir(current_dir,create_dir(path));
			else
				printf("directory name already exists\n");
		}
		else if(strcmp(command,"rmf")==0){
			File *file = find_file(current_dir,path);
			if(file != NULL){
				delete_file(current_dir,file);
			}
			else
				printf("file not found\n");
		}
		else if(strcmp(command,"rmd")==0){
			Directory *dir = find_dir(current_dir,path);
			if(dir != NULL){
				delete_dir(dir);
			}
			else
				printf("directory not found\n");
		}
		else if(strcmp(command,"exit")==0){
			return;
		}
		else if(strcmp(command,"\n")==0){
			continue;
		}
		else{
			printf("unknown command\n");
		}
		
		
	}
}

void single(){
	char command[50];
	char path[50];
	char input[100];
	
	Directory *root = create_dir("root");
	Directory *current_dir = root;
	
	while(1){
		printPrompt(current_dir);
		fgets(input,sizeof(input),stdin);
		int count = sscanf(input,"%49s %49s",command,path);
		
		if(strcmp(command,"ls")==0){
			list_dir(current_dir);
		}
		else if(strcmp(command,"touch")==0){
			if(name_check(current_dir,path))
				add_file(current_dir,create_file(path));
			else
				printf("filename already exists\n");
		}
		else if(strcmp(command,"rmf")==0){
			File *file = find_file(current_dir,path);
			if(file != NULL){
				delete_file(current_dir,file);
			}
			else
				printf("file not found\n");
		}
		else if(strcmp(command,"exit")==0){
			return;
		}
		else if(strcmp(command,"\n")==0){
			continue;
		}
		else{
			printf("unknown command\n");
		}
		
		
	}
}

void twoLevel(){
	char command[50];
	char path[50];
	char input[100];
	
	Directory *root = create_dir("root");
	Directory *current_dir = root;
	
	while(1){
		printPrompt(current_dir);
		fgets(input,sizeof(input),stdin);
		int count = sscanf(input,"%49s %49s",command,path);
		
		if(strcmp(command,"cd")==0){
			if(strcmp(path,"..")==0){
				if(current_dir->parent!=NULL) current_dir = current_dir->parent;
			}
			else if(strcmp(current_dir->name,"root")==0){
				Directory *dir = find_dir(current_dir,path);
				if(dir!=NULL)
					current_dir = dir;					
			}
			else
					printf("directory not found\n");
			
		}
		else if(strcmp(command,"ls")==0){
			list_dir(current_dir);
		}
		else if(strcmp(command,"touch")==0){
			if(name_check(current_dir,path))
				add_file(current_dir,create_file(path));
			else
				printf("filename already exists\n");
		}
		else if(strcmp(command,"mkdir")==0){
			if(strcmp(current_dir->name,"root")==0)
				if(name_check(current_dir,path))
					add_subdir(current_dir,create_dir(path));
				else 
					printf("directory name already exists\n");
			else
				printf("uable to create directory\n");
		}
		else if(strcmp(command,"rmf")==0){
			File *file = find_file(current_dir,path);
			if(file != NULL){
				delete_file(current_dir,file);
			}
			else
				printf("file not found\n");
		}
		else if(strcmp(command,"rmd")==0){
			Directory *dir = find_dir(current_dir,path);
			if(dir != NULL){
				delete_dir(dir);
			}
			else
				printf("directory not found\n");
		}
		else if(strcmp(command,"exit")==0){
			return;
		}
		else if(strcmp(command,"\n")==0){
			continue;
		}
		else{
			printf("unknown command\n");
		}
		
		
	}
}

int main(){
	
	int choice;
	char newline_consumer;
	
	while(1){
		printf("\n---Choose an option---\n");
		printf("\t1.Single Level\n\t2.Two Level\n\t3.Heirarchial\n\t0.Exit\n\tEnter choice:> ");
		scanf("%d",&choice);
		scanf("%c",&newline_consumer);
		
		switch(choice){
			case 1: single(); break;
			case 2: twoLevel(); break;
			case 3: heirarchial(); break;
			case 0: return 0;
			default: printf("Enter a valid option\n"); 
		}
	}
	
	return 0;
}
