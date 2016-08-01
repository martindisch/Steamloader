#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "steamaccess.h"
#include "../curlpost/curlpost.h"
#include "../jsmn/jsmn.h"

static const char *url = "https://api.steampowered.com/ISteamRemoteStorage/GetPublishedFileDetails/v0001/";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

/**
 * Returns the filenames and download links of given workshop items.
 * Note: the memory holding the information needs to be freed by the user.
 * 
 * @param items the Steam workshop IDs of the objects
 * @returns array of pointers to fileinfo structures with the names and download links
 * of the items or NULL if there was an error. The fileinfo structures may contain NULL
 * elements if not all details have been found in JSON, so there is a need to check the
 * results before handing them somewhere else where this might lead to a crash.
 * The memory allocated for the array, structures and strings in it needs to be freed by the user.
 */
struct fileinfo **get_fileinfo(char *items[], int itemcount) {
    // append ID to fields string
    char *field_buffer = malloc((15 + 32 * itemcount) * sizeof(char));
    if (field_buffer == NULL) {
        printf("Ran out of memory allocating field space\n");
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
    char *result = get_post(url, field_buffer);
    
    // in case of request failure, return NULL
    if (!result) {
        free(field_buffer);
        return NULL;
    }
    
    // initialize JSON parser & parse
    jsmn_parser p;
    jsmntok_t *t = malloc(90 * sizeof(jsmntok_t) * itemcount);
    if (t == NULL) {
        printf("Ran out of memory allocating tokens\n");
        exit(1);
    }
    jsmn_init(&p);
    int r = jsmn_parse(&p, result, strlen(result), t, 90 * itemcount);
    
    // in case of parsing failure return NULL
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        free(result);
        free(field_buffer);
        return NULL;
    }
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("No object in JSON\n");
        free(result);
        free(field_buffer);
        return NULL;
    }
    
    // prepare fileinfo structure for filling with details
    int z = 0;
    char buffer[200];
    struct fileinfo **inf = malloc(itemcount * sizeof(struct fileinfo *));
    if (inf == NULL) {
        printf("Ran out of memory allocating fileinfo structure\n");
        exit(1);
    }
    // go through all JSON tokens
    for (i = 1; i < r; i++) {
        if (jsoneq(result, &t[i], "filename") == 0) {
            // copy filename into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            // allocate memory for the current fileinfo structure
            inf[z] = malloc(sizeof(struct fileinfo));
            if (inf[z] == NULL) {
                printf("Ran out of memory allocating fileinfo element\n");
                exit(1);
            }
            // allocate memory in structure
            inf[z]->filename = malloc((strlen(buffer) * sizeof(char)) + 1);
            if (inf[z]->filename == NULL) {
                // exit if not enough memory
                printf("Ran out of memory processing JSON\n");
                exit(1);
            }
            // copy filename into structure
            strcpy(inf[z]->filename, buffer);
        } else if (jsoneq(result, &t[i], "file_url") == 0) {
            // copy URL into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            // allocate memory in structure
            inf[z]->download = malloc((strlen(buffer) * sizeof(char)) + 1);
            if (inf[z]->download == NULL) {
                // exit if not enough memory
                printf("Ran out of memory processing JSON\n");
                exit(1);
            }
            // copy URL into structure
            strcpy(inf[z]->download, buffer);
            z++;
        }
    }
    
    free(result);
    free(field_buffer);
    
    return inf;
}
