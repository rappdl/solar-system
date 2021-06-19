#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>
 
struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("Not enough memory (realloc returned NULL)\n");
    return 0;
  }
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
 
json_t *http_get_json_data (const char *url){
	//Create the chunk
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0;
	 
	CURL *curl;
    CURLcode res;
    
    //Init the connection to the website
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s/n", 
                curl_easy_strerror(res));
			return NULL;
        }
        else printf("%lu bytes retrieved\n\n", (unsigned long)chunk.size);

	json_error_t error;
	//Get the chunck information
	json_t *root = json_loadb(chunk.memory, chunk.size, JSON_DECODE_ANY, &error);
	if ( !json_is_object(root)) {
		fprintf(stderr, "Error: line%d, column %d : %s\n",
			error.line, error.column, error.text);
		return NULL;
	}
	//free memory
	curl_easy_cleanup(curl);
    free(chunk.memory);
	curl_global_cleanup();
	return root;
	}
}
