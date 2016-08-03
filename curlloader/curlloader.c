#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "curlloader.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *) stream);
    return written;
}

int curl_download(struct downloadinfo *files[], int filecount, char *location, int verbose) {
    int length;
    char *new_loc;
    if (!location) {
        // save to current directory if no path has been specified
        location = malloc(1);
        strcpy(location, "");
    } else if ((length = strlen(location)) >= 1) {
        if (location[length - 1] != '/') {
            // if path doesn't end on '/', append it
            new_loc = malloc((length + 2) * sizeof(char));
            if (!new_loc) {
                printf("curlloader.c: Ran out of memory allocating modified path\n");
                exit(1);
            }
            strcpy(new_loc, location);
            strcat(new_loc, "/");
            location = new_loc;
        }
    }
    
    CURL *curl_handle;
    FILE *pagefile;
    
    // initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // start curl session
    curl_handle = curl_easy_init();
    
    // go over all file objects
    char pathbuf[256];
    int i;
    int successes = 0;
    for (i = 0; i < filecount; i++) {
        if (files[i]) {
            // set download url
            curl_easy_setopt(curl_handle, CURLOPT_URL, files[i]->url);
            
            // set callback
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
            
            // please servers by specifying user agent
            curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            
            // prepare file path
            strcpy(pathbuf, location);
            strcat(pathbuf, files[i]->filename);
            
            // open the file
            pagefile = fopen(pathbuf, "wb");
            
            if (!pagefile) {
                printf("curlloader.c: Failed to open file to save download - does the directory exist?\n");
                exit(1);
            } else {
                // write page body to file handle
                curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
                
                // make request
                curl_easy_perform(curl_handle);
                
                // close file
                fclose(pagefile);
                
                // reset curl handle for reuse
                curl_easy_reset(curl_handle);
            
                if (verbose) {
                    printf("Downloaded file %s\n", pathbuf);
                }
                successes++;
            }
        } else {
            printf("curlloader.c: Received an empty downloadinfo pointer\n");
        }
    }
    
    // if we allocated memory to manipulate the path, release it now
    if (new_loc) {
        free(new_loc);
    };
    
    // clean up curl session
    curl_easy_cleanup(curl_handle);
    
    // clean up curl
    curl_global_cleanup();
    
    return successes;
}
