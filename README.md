# Steamloader

This tool downloads Steam workshop items using their IDs, which is useful for keeping missions from the workshop up-to-date on game servers.

The ID can be found in the item's URL. For a URL such as https://steamcommunity.com/sharedfiles/filedetails/?id=700221852 the ID is 700221852.

## Usage

Run the tool by giving it the IDs of the items to download.
`./steamloader ID1 ID2 ID3 ...`

There is also an option `-o` to specify a different directory to download the items into. Steamloader is fully compliant with the GNU command line format, so feel free to use `-?`, `--help`, `--usage` or whatever to dig into it.

## Installation

Steamloader is written in C and should run on just about everything. The only external dependency is [libcurl](https://curl.haxx.se/libcurl/), which is needed to compile the application. If you don't have it already, you might want to install it with `sudo apt-get install libcurl4-openssl-dev` or the appropriate command on your system.

After that you only need to run `make` and the `steamloader` binary should be good to go.

## Dependencies
* [libcurl](https://curl.haxx.se/libcurl/) for HTTP requests.
* [jsmn](https://github.com/zserge/jsmn) as a simple JSON parser. The source is already integrated in Steamloader, so there is no need to install anything.

## License

This project is published under the [MIT License](LICENSE), so feel free to do basically anything you'd like with it.

## Disclaimer

Steamloader relies on the Steam API to get information about the workshop items, so any changes on their end might break it.
