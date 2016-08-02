struct downloadinfo {
    char *filename;
    char *url;
};

/**
 * Downloads the given files into the current directory or somewhere else.
 * 
 * @param files array of pointers to downloadinfo structures, which define the files to download
 * @param filecount number of entries in the aforementioned array
 * @param location optional path to download files into
 * @param verbose set to 1 for detailed output, 0 for error messages only
 */
int curl_download(struct downloadinfo **files, int filecount, char *location, int verbose);
