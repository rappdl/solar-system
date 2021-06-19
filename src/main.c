#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>
#include "http_get_json_data.h"
#include "htable.h"
#include "list.h"
#include "solar.h"

//get the command from CL
int get_cmd(){								
	int cmd;
	char str [64];
	List_node *list;
	
	scanf("%d", &cmd);
	
	switch (cmd) {	
		case 1:								//list all bodyes
			list_bodies();
			break;
		case 2:
			list = solar_get_planets();		//list the planets
			printf("\n");
			list_foreach(list, print_data, NULL);
			printf("Number of planets: %d\n", list_size(list));
			list_destroy(list);
			break;
		case 3:								//list the satellites from a body
			printf("\nWich body? ");
			scanf("%s", str);
			printf("\n");
			Body *body = solar_get_body(str);
			if(body!=NULL) {
				list = solar_get_satellites(body);
				if(list!=NULL){
					list_foreach(list, print_data, NULL);
					printf("Number of satellites: %d\n", list_size(list));
				}else printf("This body has no satellites\n");
			}else printf("Body not recognize\n");
			break;
		case 4:								//Ask for a body to print his information
			printf("\nWich body? ");
			scanf("%s", str);
			body = solar_get_body(str);
			if(body!=NULL) print_data(body, NULL);
			else printf("Body not recognize\n");
			break;
		case 5:
			break;
		default:
			printf("Command not found\n");
			break;
	}
	printf("\n");
	return cmd;
}

void print_menu(){									//prints the main menu
	printf("Options:\n");
	printf("1. List all bodies\n");
	printf("2. List planets\n");
	printf("3. List satellites\n");
	printf("4. List one body of your choice\n");
	printf("5. Exit\n");
	printf("Insert command: ");
}


int main(int argc, char *argv[]) {	
	const char *url = "https://api.le-systeme-solaire.net/rest/bodies";
	json_error_t error;
	json_t *root = http_get_json_data(url);
	if ( !json_is_object(root)) {
				fprintf(stderr, "Error: line%d, column %d : %s\n",
				error.line, error.column, error.text);
		return -1;
	}
	
	json_t *obj = json_object_get(root, "bodies");
	if ( !json_is_array(obj)) {
				fprintf(stderr, "Error: line%d, column %d : %s\n",
				error.line, error.column, error.text);
		return -1;
	}
	
	htable_of_bodies = htable_create(300, hash_function, body_cmp_name);			//htable de todos os corpos
	size_t  ARRAY_SIZE = json_array_size(obj);
	
	json_t *curr=NULL;
	for(int i=0; i<ARRAY_SIZE; ++i) {
		curr = json_array_get(obj, i);
		Body *body = malloc (sizeof (Body));
		set_body_info(curr,body);
		solar_insert_body(body);	
	}
	json_decref(root);
	
	
	htable_foreach(htable_of_bodies, get_moons, NULL);								//associa as luas a ao respetivo planeta (caso tenha)
	
	//MENU
	do {
		print_menu();
		}
	while(get_cmd()!=5);
	printf("Closing app...\n");
	
	
	/* FREE MEMORY */
	
	htable_foreach(htable_of_bodies, free_bodies, NULL);
	htable_destroy(htable_of_bodies);
	
}
