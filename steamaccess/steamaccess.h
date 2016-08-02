/**
 * Returns the filenames and download links of given workshop items.
 * Note: the memory holding the information needs to be freed by the user.
 * 
 * @param items the Steam workshop IDs of the objects
 * @returns array of pointers to downloadinfo structures with the names and download links
 * of the items or NULL if there was an error. The downloadinfo structures may contain NULL
 * elements if not all details have been found in JSON, so there is a need to check the
 * results before handing them somewhere else where this might lead to a crash.
 * The memory allocated for the array, structures and strings in it needs to be freed by the user.
 */
struct downloadinfo **get_downloadinfo(char **items, int itemcount);
