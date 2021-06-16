#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>
#include "http_get_json_data.h"
#include "htable.h"
#include "list.h"
#include "solar.h"

Htable *htable_of_bodies = NULL;

void print_data(void *body, void *arg) {						//print da informação do corpo recebido
	Body *b = (Body *) body;
	printf("Name: %s\n", b->name);
	printf("Radius: %d km\n", b->radius);
	printf("Mass: %.3f x 10^%d kg\n", b->mass, b->mass_exp);
	printf("Gravity: %.3f m/s²\n", b->gravity);
	if(b->is_planet==1) printf("Planet: Yes\n");
	else printf("Planet: No\n");
	if(NULL!=b->father) printf("Belongs to: %s\n", b->father);
	printf("\n");
}

void free_bodies( void *body, void *arg ){						//função que liberta os corpos da memória
    Body *body_to_remove = (Body *) body;    
    free( (void *)body_to_remove->name );
    free( (void *)body_to_remove->father );
    list_destroy(body_to_remove->moons);
    free( body );
}

int body_cmp_name(const void *data, const void *b) {			//função de comparação dos bodies (name)
	return strcmp(((Body *)data)->name, ((Body*)b)->name);
}

int hash_function(const void *data) {
	const char *body_name = ((const Body *)data)->name;
	int hash = 0;
	while (*body_name)
		hash += *body_name++;
	return hash;
}

void set_body_info(json_t *root, Body *body) {					//faz download da informação do site
	json_error_t error;	
	if ( !json_is_object(root)) {
		fprintf(stderr, "Error: line%d, column %d : %s\n",
			error.line, error.column, error.text); 
		exit(-1);
	}
	
	/* 			NAME 		*/
	json_t *obj_name = json_object_get(root, "id");
	if ( !json_is_string(obj_name)) {
		fprintf(stderr, "name Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		exit(-1);
	}
	const char *str = json_string_value(obj_name);
	char *n = malloc (strlen(str)+1);
	strcpy(n, str);
	body->name=n;

	/* 			RADIUS 		*/
	json_t *obj_radius = json_object_get(root, "meanRadius");
	if ( !json_is_real(obj_radius)) {
		fprintf(stderr, " radius Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		exit(-1);
	}
	body->radius = (int) json_number_value(obj_radius);
	
	
	/* 			MASS		*/
	json_t *obj_mass = json_object_get(root, "mass");
	body->mass=0;
	if(!json_is_null(obj_mass)) {
		if ( !json_is_object(obj_mass)) {
			fprintf(stderr, "mass Error: line%d, column %d : %s\n",
				error.line, error.column, error.text);
			exit(-1);
		}

		json_t *obj_mass_value = json_object_get(obj_mass, "massValue");
		if ( !json_is_real(obj_mass_value)) {
			fprintf(stderr, "mass value Error: line%d, column %d : %s\n",
				error.line, error.column, error.text);
			exit(-1);
		}
		
		json_t *obj_mass_exp = json_object_get(obj_mass, "massExponent");
		if ( !json_is_real(obj_mass_value)) {
			fprintf(stderr, "mass value Error: line%d, column %d : %s\n",
				error.line, error.column, error.text);
			exit(-1);
		}
		body->mass = (float) json_number_value(obj_mass_value);
		body->mass_exp = (int) json_number_value(obj_mass_exp);
		
	}
	
	/* 		GRAVITY			*/
	json_t *obj_gravity = json_object_get(root, "gravity");
	if ( !json_is_real(obj_gravity)) {
		fprintf(stderr, "gravity Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		exit(-1);
	}
	body->gravity = (float) json_real_value(obj_gravity);
	
	/* 		IS PLANET?		*/
	json_t *obj_is_planet = json_object_get(root, "isPlanet");
	if ( !json_is_boolean(obj_is_planet)) {
		fprintf(stderr, "is planet Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		exit(-1);
	}
	
	body->is_planet = json_is_true(obj_is_planet);	
	body->father=NULL;
	body->moons=NULL;
	
	
	json_t *obj_around_planet = json_object_get(root, "aroundPlanet");
	if(!json_is_null(obj_around_planet)) {
		if ( !json_is_object(obj_around_planet)) {
		fprintf(stderr, "around planet Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		exit(-1);
		}
		
		json_t *obj_planet = json_object_get(obj_around_planet, "planet");
		if ( !json_is_string(obj_planet)) {
		fprintf(stderr, "planet Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		exit(-1);
		}
		
		const char *father_name = json_string_value(obj_planet);		
		char *fn = malloc (strlen(father_name)+1);
		strcpy(fn, father_name);
		body->father=fn;
	}
}

int solar_insert_body(Body *b) {									//insere o corpo na data struct (htable)
	return htable_insert(htable_of_bodies, b);
}
	
Body *solar_get_body(const char *body_name) {						//obtem o corpo através do nome passado
	Body key = {.name = body_name};
	Body *body = htable_lookup(htable_of_bodies, &key);
	return body;
}


void get_planets(void *data, void *l) { 							//recebe como parametro o corpo e a lista onde vai inserir (planetas)
	Body *b = (Body *)data;
	if(b->is_planet) {
		List_node **list = (List_node **) l;
		*list = list_insert(*list, b);
	}
}

List_node *solar_get_planets(){										//percorre a htable à procura de planetas e retorna a lista de planetas
	List_node *planets_list = NULL;
	htable_foreach(htable_of_bodies, get_planets, &planets_list);
	return planets_list;
}


List_node *solar_get_satellites(Body *curr) {						//obtem a lista de satelites do corpo passado como parametro
	List_node *moons = NULL;
	if(curr!=NULL) moons = curr->moons;
	return moons;
}

void get_moons(void *b, void *l){									//associar as luas a cada corpo que as tenha (é realizado depois da criação da htable)
		Body *body = (Body *) b;									//lua
		if(!body->is_planet && body->father!=NULL) {
			Body *planet = solar_get_body(body->father);			//planeta "pai"
			planet->moons = list_insert(planet->moons, body);		//insere a lua (recebida como param) na lista de satelites do planeta "pai"
		}
}

void list_bodies() {												//lista todos os corpos presentes na htable
	htable_foreach(htable_of_bodies, print_data, NULL);
	printf("Number of bodies: %ld\n", htable_size(htable_of_bodies));
}
