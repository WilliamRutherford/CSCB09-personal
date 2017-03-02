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

void show_hash(char *hash_val, long block_size) {
    for(int i = 0; i < block_size; i++) {
        printf("%.2hhx ", hash_val[i]);
    }
    printf("\n");
}


void file_in_dir(char *result, char *dir, char *fname){

    printf("finding path of %s in %s \n", dir, fname);

    if(strcmp(dir, ".") == 0 || strlen(dir) == 0){

	strcpy(result, fname);

    } else {

	strcpy(result, dir);
	strcat(strcat(result, "/"), fname);

    }

    printf("result was %s \n", result);

}


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

    //printf("%s is at depth %d \n", file_open_path, dirdepth);	
    
    //file_open_path should be the path to the file. filepath is the path to it's parent

    if(strlen(filepath) == 0 || strcmp(filepath, ".") == 0){

	strcpy(file_open_path, fname);

    }  else {

	strcpy(file_open_path, filepath);
	strcat(strcat(file_open_path, "/"), fname);

    }


    //printf("file: %s    path: $%s$    file path: %s \n", fname, filepath, file_open_path);
   
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
	   //printf("hash: %s \n", root->hash);
	
	}
	//printf("hash created successfully \n");

	root->contents = NULL;
	root->next = NULL;

	fclose(file_to_read);

	//printf("%s created successfully! \n", file_open_path);

    } else if ( is_dir ) {
	

	//char *dir_open_path = malloc(256*sizeof(char));

	DIR *dirstream;  
	dirstream = opendir(file_open_path);

	if(dirstream == NULL){
		printf("opendir: error opening directory %s at path %s \n", fname, filepath );
		exit(1);
	}

	strcpy(filepath, file_open_path);

	//strcpy(filepath, file_open_path);

    	//struct dirent *currfile = readdir(dirstream);
	struct dirent *currfile = NULL;
	struct TreeNode *currnode = NULL;
	struct TreeNode *lastnode = NULL;

	//printf("currfile gotten \n");

	//printf("%s is dir \n", root->fname);	

	//contents of root must be set manually
	


	do{

   	    currfile = readdir(dirstream);

	} while (currfile != NULL && currfile->d_name[0] == '.');

	if(currfile != NULL){
	    //printf("first file in dir is: %s \n",currfile->d_name);
	    //file_in_dir(dir_open_path, filepath, currfile->d_name);
	    currnode = generate_ftree(currfile->d_name);
	    //printf("first dir recusive call complete");
	    root->contents = currnode;
	    printf("contents of %s is %s \n", root->fname, currfile->d_name);
	    lastnode = currnode;

	} else {

	    root->contents = NULL;
	
	}

	while ( currfile != NULL ){
	
	    currfile = readdir(dirstream);

	    if(currfile == NULL || currfile->d_name[0] == '.' || strcmp(currfile->d_name, "..") == 0){
		//printf("node is invisble or null \n"); 
		continue;	    
	    }
	    //file_in_dir(dir_open_path, filepath, currfile->d_name);
	    currnode = generate_ftree(currfile->d_name);
	    
	    //printf("recursive call successful \n");

	    if(currnode != NULL){
		//printf("node isn't null \n");
		lastnode->next = currnode;
		lastnode = currnode;

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
	    dirdepth++;

        } else {
	    
    	    int index = (long)((long)slashpos - (long)filepath + 1);
	    //printf("index created: %d \n", index);
	    memset(filepath+index, 0, strlen(filepath)-index);
	    
	}
	//printf("path name changed back to: %s : \n", filepath);

    }

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
    if(root == NULL){

	printf("null \n");

    } else if(root->contents == NULL && root->hash != NULL){ // if file
	 print_spaces(depth * 2);
         printf("%s (%o) \n", root->fname, root->permissions);
	 //show_hash(root->hash, 8);

    } else if (root->hash == NULL){ //if directory, go throught contents and recursively call each
	print_spaces(depth * 2);
        printf("=====  %s (%o) ===== \n",  root->fname, root->permissions);
	
	if(root->contents != NULL){
	    struct TreeNode *currnode = NULL;
	
	    //printf("printing %s...", currnode->fname);

	    currnode = root->contents;
	
	    while(currnode != NULL){
		    print_ftree_rec(currnode, depth + 1);
		    
		    currnode = currnode->next;
		    
	    }

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

