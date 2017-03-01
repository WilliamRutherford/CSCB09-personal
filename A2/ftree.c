#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
// Add your system includes here.

#include "ftree.h"
#include "hash.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"

/*
int perms_from_mode(st_mode mode){

	int *result = malloc(sizeof());

}
*/


/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
	
    static char *filepath;

    static char *root_run_dir;    

    /*
	filepath is the current path. If it is blank, we just call files as normal. if we're at dir depth 1, we must call dir1/file. depth 2 is dir1/dir2/file, etc. 


    */

    /*
    static char *filepath = malloc(256*sizeof(char));

    static char *root_run_dir = malloc(256*sizeof(char));
    */

    static int dirdepth = -1;

    if(dirdepth == -1){

	//printf("first loop is run \n");

	filepath = malloc(256*sizeof(char));

	root_run_dir = malloc(256*sizeof(char));
	
	strcpy(root_run_dir, fname);

	//printf("%s \n", root_run_dir);

	dirdepth = 0;

    }

    /*   
    if(runnum == 0){
   
	strcpy(filepath, fname);

    }

    runnum++;

   */ 

    struct TreeNode *root = NULL;

    root = malloc(sizeof(struct TreeNode));

    //printf("root allocated at %p \n", root);    

    root->fname = malloc(sizeof(256*sizeof(char)));
    

    root->contents = malloc(sizeof(struct TreeNode*));

    root->next = malloc(sizeof(struct TreeNode*));

    root->hash = NULL;

    //printf("file name allocated \n");

    //strcpy(root->fname, fname);
    strcpy(root->fname, fname);
    
    char *file_open_path = malloc(256 * sizeof(char));
    strcpy(file_open_path, filepath);
    //printf("%s is at depth %d \n", file_open_path, dirdepth);	
    if(dirdepth == 0) {
    
	strcpy(file_open_path, root_run_dir);

    } else if (strcmp(root_run_dir, ".") == 0 && dirdepth == 1) {

	strcpy(file_open_path, fname);

    } else if (dirdepth > 0) {

	strcat(strcat(file_open_path, "/"), fname);

    }

    printf("file: %s path: $%s$ file path: %s root: %s depth: %d \n", fname, filepath, file_open_path, root_run_dir, dirdepth);
    
    //printf("%s, %s \n", filepath, fname);

    //printf("file name : %s at %p \n", root->fname, root);

    struct stat buf;
    if (lstat(file_open_path, &buf) == -1){

	printf("lstat: error opening file/dir %s \n", file_open_path);
	exit(1);

    }

    int is_dir = S_ISDIR(buf.st_mode);
    int is_reg = S_ISREG(buf.st_mode);
    int is_link = S_ISLNK(buf.st_mode);
    root->permissions = buf.st_mode & 0777; 

	

    if( is_link || is_reg ){

	
	//printf("opening: %s \n", file_open_path);
        FILE *file_to_read = fopen( file_open_path, "r" );
	
	if(file_to_read == NULL){

	   printf("fopen: error opening file %s at path %s \n", fname, filepath);
	   exit(1);

	} else {
	 
	   root->hash = hash(file_to_read);
	
	}
	//printf("hash created successfully \n");

	root->contents = NULL;
	root->next = NULL;

	fclose(file_to_read);

	//printf("%s created successfully! \n", file_open_path);

    } else if ( is_dir ) {
	
	if(dirdepth > 1){

	    strcat(strcat(filepath, "/"), fname);	

	} else if(strcmp(filepath, ".") == 0){

	    strcpy(filepath, 0);

	} else if()dirdepth++;

	DIR *dirstream;
	if(strcmp(fname, ".") == 0){	    
	    dirstream = opendir(fname);
	} else {
            dirstream = opendir(filepath);
	}

	if(dirstream == NULL){
		printf("opendir: error opening directory %s at path %s \n", fname, filepath );
		exit(1);
	}

    	//struct dirent *currfile = readdir(dirstream);
	struct dirent *currfile = NULL;
	struct TreeNode *currnode = NULL;
	struct TreeNode *lastnode = NULL;

	//printf("currfile gotten \n");

	//printf("%s is dir \n", root->fname);	

	//contents of root must be set manually

	currfile = readdir(dirstream);
	if(currfile != NULL){
	    //printf("first file in dir is: %s \n",currfile->d_name);
	    currnode = generate_ftree(currfile->d_name);
	    //printf("first dir recusive call complete");
	    root->contents = currnode;
	    //printf("contents of %s is %s \n", root->fname, currfile->d_name);
	    lastnode = currnode;

	}

	while ( currfile != NULL ){
	
	    currfile = readdir(dirstream);

	    if(currfile == NULL || currfile->d_name[0] == '.'){
		//printf("node is invisble or null \n", currfile->d_name); 
		continue;	    
	    }

	    currnode = generate_ftree(currfile->d_name);
	    
	    //printf("recursive call successful \n");

	    if(currnode != NULL){
		//printf("node isn't null \n");
	        //lastnode = currnode;
		//currnode->next = lastnode;
		lastnode->next = currnode;
		lastnode = currnode;
		
		//printf("move to next node successful \n");

	    } else {

		//printf("current node is null \n");		

	    }
	    lastnode->next = NULL;
	    
	    //currfile = readdir(dirstream);
	} 
	
	//printf("directory if done \n");
	
	//now we need to remove fname from filepath

	char *slashpos = NULL;
	slashpos = strrchr(filepath,',');

	    //printf("editing path name %s \n", filepath);
	    //printf("filepath address: %s   slashpos address: %s", filepath, slashpos);
	    //printf("slashpos created \n");
	if( slashpos == NULL ){
		
	    if(root_run_dir[0] != '.'){

	        strcpy(filepath, root_run_dir);

	    } else {

		memset(filepath, 0, 256*sizeof(char));
	
	    }
	    dirdepth = 0;

        } else if(dirdepth > 1) {
	    
    	    int index = (long)((long)slashpos - (long)filepath + 1);
	    //printf("index created: %d \n", index);
	    memset(filepath+index, 0, strlen(filepath)-index);
	    
	    }
	    /*
	    for(int i = index; i < 256 ; i++){
		
		printf("%s", filepath);
	        if(filepath[i] == 0){

		    break;	

    	        } else {
    
	            filepath[i] = 0;
   
                }
            }
	    */
	} else {

	    

	}
	//printf("path name changed back to: %s : \n", filepath);

    

    return root;

}

void print_spaces(int x) {
	for(int i = 0; i < x; i++){
	

	    printf(" ");

	}
}

//Recursive call to print tree
void print_ftree_rec(struct TreeNode *root, int depth) {
    //printf("beginning to print tree \n");
    //if file, print.
    if(root->contents == NULL){
	 print_spaces(depth * 2);
         printf("%s (%o) \n", root->fname, root->permissions);
    } else if (root->contents != NULL && root->hash == NULL){ //if directory, go throught contents and recursively call each
	print_spaces(depth * 2);
        printf("=====  %s (%o) ===== \n",  root->fname, root->permissions);
	struct TreeNode *currnode;
	currnode = root->contents;
	while(currnode != NULL){
		print_ftree_rec(currnode, depth + 1);
		currnode = currnode->next;
	}
	
    } else {

	printf("error");

    }
}



/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.

    //printf("%*s", depth * 2, "");
    // Your implementation here.

    print_ftree_rec(root, 0); //recursive call to recursive function

    

}

