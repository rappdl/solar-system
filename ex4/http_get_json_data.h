#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

json_t *http_get_json_data (const char *url);
