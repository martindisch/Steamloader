#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>

struct memory_struct {
    char *memory;
    size_t size;
};

static size_t write_mem_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memory_struct *mem = (struct memory_struct *) userp;
    
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("Ran out of memory using libcurl\n");
        exit(1);
    }
    
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

/**
 * Returns the answer to a HTTP POST request using the given url and fields.
 * Note: after using the result, the memory must be freed by the user.
 * 
 * @param url character pointer to the URL
 * @param fields character pointer to the fields
 * @returns character pointer to the answer string or NULL if there was an error.
 * This must be freed by the user.
 */
char *get_post(char *url, char *fields) {
    CURL *curl_handle;
    CURLcode res;
    
    struct memory_struct chunk;
    
    chunk.memory = malloc(1); // will be grown as needed
    chunk.size = 0;
    
    // initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // start curl session
    curl_handle = curl_easy_init();
    
    // specify URL to get
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    
    // specify POST data
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, fields);
    
    // specify callback
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_mem_callback);
    
    // pass chunk structure to callback function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);
    
    // please servers by specifying user agent
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    
    // make request
    res = curl_easy_perform(curl_handle);
    
    // check for errors
    if (res != CURLE_OK) {
        printf("curl failed on URL\n%s\nwith fields\n%s\n", url, fields);
        chunk.memory = NULL;
    } else {
        // everything is fine
    }

    // clean up curl session
    curl_easy_cleanup(curl_handle);
    
    // clean up curl
    curl_global_cleanup();
    
    return chunk.memory;
}
