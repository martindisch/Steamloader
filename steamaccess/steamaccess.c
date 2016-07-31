#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "steamaccess.h"
#include "../curlpost/curlpost.h"
#include "../jsmn/jsmn.h"

static const char *url = "https://api.steampowered.com/ISteamRemoteStorage/GetPublishedFileDetails/v0001/";
static const char *fields = "itemcount=1&publishedfileids[0]=";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

/**
 * Returns the filename and download link of a workshop item.
 * Note: the memory of the returned structure and the two strings in it needs to be freed by the user.
 * 
 * @param id the Steam workshop ID of the object
 * @returns pointer to a fileinfo structure with the name and download link of the item or NULL if there was an error.
 * This memory needs to be freed by the user.
 */
struct fileinfo *get_fileinfo(char *id) {
    // append ID to fields string
    char field_buffer[40];
    strcpy(field_buffer, fields);
    strcat(field_buffer, id);
    
    // make request
    char *result = get_post(url, field_buffer);
    
    // in case of request failure, return NULL
    if (!result) {
        return NULL;
    }
    
    // initialize JSON parser & parse
    jsmn_parser p;
    jsmntok_t t[90];
    jsmn_init(&p);
    int r = jsmn_parse(&p, result, strlen(result), t, 90);
    
    // in case of parsing failure return NULL
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        free(result);
        return NULL;
    }
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("No object in JSON\n");
        free(result);
        return NULL;
    }
    
    // prepare fileinfo structure for filling with details
    int i;
    char buffer[200];
    struct fileinfo *inf = malloc(sizeof(struct fileinfo));
    // go through all JSON tokens
    for (i = 1; i < r; i++) {
        if (jsoneq(result, &t[i], "filename") == 0) {
            // copy filename into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            // allocate memory in structure
            inf->filename = malloc((strlen(buffer) * sizeof(char)) + 1);
            if (!inf->filename) {
                // exit if not enough memory
                printf("Ran out of memory processing JSON\n");
                exit(1);
            }
            // copy filename into structure
            strcpy(inf->filename, buffer);
        } else if (jsoneq(result, &t[i], "file_url") == 0) {
            // copy URL into buffer
            sprintf(buffer, "%.*s", t[i+1].end-t[i+1].start, result + t[i+1].start);
            // allocate memory in structure
            inf->download = malloc((strlen(buffer) * sizeof(char)) + 1);
            if (!inf->download) {
                // exit if not enough memory
                printf("Ran out of memory processing JSON\n");
                exit(1);
            }
            // copy URL into structure
            strcpy(inf->download, buffer);
        }
    }
    
    // if not all required details have been found, free memory and return NULL
    if (!inf->filename || !inf->download) {
        printf("Not all data found in JSON\n");
        free(result);
        free(inf->filename);
        free(inf->download);
        free(inf);
        return NULL;
    }
    
    free(result);
    
    return inf;
}
