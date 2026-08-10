#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#define BUFFER_SIZE 4096

const char* TEMPLATE_PATH = "/home/spade/codestuff/flakeTemplate/";

void invokeUsage() {
printf("Improper Usage\n");
printf("Usage: new-prod {NAME} --flake {TEMPLATE}\n");
}

int copyAndPasteFiles(FILE* template, char* targetPath){
	FILE* targetFile = fopen(strcat(targetPath, "/flake.nix"), "wb");

	if(targetFile == NULL){
		perror("Could not open file");
		return -1;
	
	}

	char buffer[BUFFER_SIZE];
	size_t bytes_read;

	while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, template)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, targetFile);
        if (bytes_written < bytes_read) {
            perror("Error writing to destination file");
            fclose(template);
            fclose(targetFile);
            return -1;
        }
    }
	
    fclose(template);
    fclose(targetFile);
    printf("File copied successfully.\n");
    return 0;
	
}

FILE* getTemplateSelection(char* index){
	DIR *templatesDir = opendir(TEMPLATE_PATH);
	if(templatesDir == NULL){
		perror("couldnt find template directory, oopsies\n");
	}

	struct dirent *templateEntry;

	while ((templateEntry = readdir(templatesDir)) != NULL) {
        	printf("%s\n", templateEntry->d_name);
		if(strcmp(templateEntry->d_name, index) == 0 && templateEntry->d_type == DT_DIR){
			printf("suitable entry found!\n");

			size_t len = strlen(TEMPLATE_PATH) + strlen(templateEntry->d_name) + strlen("/flake.nix")+ 1;
			char *templatePath = malloc(len * sizeof(char));
			if(templatePath != NULL){
				snprintf(templatePath, len, "%s%s%s", TEMPLATE_PATH, templateEntry->d_name, "/flake.nix");
			}	
			printf("using template: %s...\n", templatePath);

			FILE* flake = fopen(templatePath, "rb");
			free(templatePath);
			return flake;
		}
    	}

	return NULL;

}




int main (int argc, char** argv){

	// create default parameters
	char *usingTemplate = "blank";
	char name[256] = "newProject";
	
	

	
	if (argc <= 1){
		invokeUsage();
		return -1;
	}	

	for(int i = 1; i < argc; ++i){
		if(strcmp(argv[i], "--flake") == 0){
			if (i + 1 < argc){
				usingTemplate = argv[++i];
				printf("finding template %s...\n", usingTemplate);
			}
			else {
				invokeUsage();
				return -1;
			};
		}
		else if(i == 1){
			strcpy(name, argv[1]);
		}

	}
	printf("making new project named %s...\n", name);

	// make new directory with POSIX 
	if (mkdir(name, 0777) == 0) {
        	printf("Directory created successfully.\n");
    	} 
	else {
        	perror("Error creating directory");
    	}
	
	int len = strlen("./")+strlen(name)+1;

	char* path = malloc(len);
	if(path != NULL){
		snprintf(path,len,"./%s", name);
		
	}
	copyAndPasteFiles(getTemplateSelection(usingTemplate), path);
	
	
	
	return 0;



}

