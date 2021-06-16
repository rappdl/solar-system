extern Htable *htable_of_bodies;

typedef struct Body {
	const char *name; 			//nome 
	int radius; 				//raio médio em Km
	float mass; 				//massa em Kg
	float gravity; 				//força gravitacional em m/s^2
	int mass_exp;
	int is_planet; 				//planeta?
	const char *father;			//planeta a que "pertence"
	List_node *moons; 			//lista de satélites
} Body;

void print_data(void *body, void *arg);

void free_bodies( void *body, void *arg );

int body_cmp_name(const void *data, const void *b);

int hash_function(const void *data);

void set_body_info(json_t *root, Body *body);

int solar_insert_body(Body *b);

Body *solar_get_body(const char *body_name);

void get_planets(void *data, void *l);

List_node *solar_get_planets();

List_node *solar_get_satellites(Body *body);

void get_moons(void *b, void *l);

void list_bodies();

