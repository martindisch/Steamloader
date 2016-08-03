#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "steamaccess.h"
#include "../curlpost/curlpost.h"
#include "../curlloader/curlloader.h"
#include "../jsmn/jsmn.h"

static const char *url = "https://api.steampowered.com/ISteamRemoteStorage/GetPublishedFileDetails/v0001/";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

struct downloadinfo **get_downloadinfo(char *items[], int itemcount) {
    // append ID to fields string
    char *field_buffer = malloc((15 + 35 * itemcount) * sizeof(char));
    if (!field_buffer) {
        printf("steamaccess.c: Ran out of memory allocating field space\n");
        exit(1);
    }
    char itembuf[32];
    sprintf(field_buffer, "itemcount=%d", itemcount);
    int i;
    for (i = 0; i < itemcount; i++) {
        sprintf(itembuf, "&publishedfileids[%d]=%s", i, items[i]);
        strcat(field_buffer, itembuf);
    }
    
    // make request
    char *result = curl_post(url, field_buffer);
    
    // in case of request failure, return NULL
    if (!result) {
        printf("steamaccess.c: HTTP request failed\n");
        free(field_buffer);
        return NULL;
    }
    
    // initialize JSON parser & parse
    jsmn_parser p;
    jsmntok_t *t = malloc(100 * sizeof(jsmntok_t) * itemcount);
    if (!t) {
        printf("steamaccess.c: Ran out of memory allocating tokens\n");
        exit(1);
    }
    jsmn_init(&p);
    int r = jsmn_parse(&p, result, strlen(result), t, 100 * itemcount);
    
    // in case of parsing failure return NULL
    if (r < 0) {
        printf("steamaccess.c: Failed to parse JSON: %d - possibly more tokens than expected\n", r);
        free(result);
        free(field_buffer);
        free(t);
        return NULL;
    }
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("steamaccess.c: No object in JSON\n");
        free(result);
        free(field_buffer);
        free(t);
        return NULL;
    }
    
    // prepare downloadinfo structure for filling with details
    int z = 0;
    char buffer[200];
    char id_buf[15] = "all";
    struct downloadinfo **inf = malloc(itemcount * sizeof(struct downloadinfo *));
    if (!inf) {
        printf("steamaccess.c: Ran out of memory allocating downloadinfo array\n");
        exit(1);
    }
    // go through all JSON tokens
    for (i = 1; i < r; i++) {
        if (jsoneq(result, &t[i], "filename") == 0) {
            // copy filename into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            // allocate memory for the current downloadinfo structure
            inf[z] = malloc(sizeof(struct downloadinfo));
            if (!inf[z]) {
                printf("steamaccess.c: Ran out of memory allocating downloadinfo element\n");
                exit(1);
            }
            // allocate memory in structure
            inf[z]->filename = malloc((strlen(buffer) * sizeof(char)) + 1);
            if (!inf[z]->filename) {
                // exit if not enough memory
                printf("steamaccess.c: Ran out of memory processing JSON\n");
                exit(1);
            }
            // copy filename into structure
            strcpy(inf[z]->filename, buffer);
        } else if (jsoneq(result, &t[i], "file_url") == 0) {
            // copy URL into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            // allocate memory in structure
            inf[z]->url = malloc((strlen(buffer) * sizeof(char)) + 1);
            if (!inf[z]->url) {
                // exit if not enough memory
                printf("steamaccess.c: Ran out of memory processing JSON\n");
                exit(1);
            }
            // copy URL into structure
            strcpy(inf[z]->url, buffer);
            z++;
        } else if(jsoneq(result, &t[i], "publishedfileid") == 0) {
            // copy fileid into buffer
            sprintf(id_buf, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
        } else if(jsoneq(result, &t[i], "result") == 0) {
            // copy result code into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            if (strcmp(buffer, "1") != 0) {
                // throw error if result is not 1
                printf("steamaccess.c: JSON request returned result %s on item %s\n", buffer, id_buf);
                inf[z] = NULL;
                z++;
            }
        }
    }
    
    free(result);
    free(field_buffer);
    free(t);
    
    return inf;
}
