/**
 * Returns the answer to a HTTP POST request using the given url and fields.
 * Note: after using the result, the memory must be freed by the user.
 * 
 * @param url constant character pointer to the URL
 * @param fields constant character pointer to the fields
 * @returns character pointer to the answer string or NULL if there was an error.
 * This memory must be freed by the user.
 */
char *curl_post(const char *url, const char *fields);
