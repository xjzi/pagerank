## Introduction
Wiki page finding and ranking.

## External Libraries
- [libcurl](https://curl.se/libcurl/) manages the HTTP requests
- [rapidjson](https://rapidjson.org/) parses the JSON responses

## Procedure
When visiting a page, the program adds the page to the graph and visits each of its internal links.
The result is a graph of all pages within a certain number of degrees of seperation from the source page.
Each node is a page that stores all of its outgoing links to other pages in the graph.

The program converts the graph into an adjacency matrix, where each point (X, Y) is a boolean showing if the Xth page links to the Yth page.

The program runs the pagerank algorithm on the adjacency matrix and returns the rank of each page.

## Structure
The project is not complete.

## Demonstration
According to the MediaWiki User-Agent Policy, "Scripts should use an informative User-Agent string with contact information, or they may be blocked without notice."
Any contact information in `contact.txt` will be included in the User-Agent header when querying the Wikipedia API.
```
echo -n "example@wikipedia.org" >> contact.txt
```
Make sure you have `libcurl` and `rapidjson` installed.
```
g++ main.rs -lcurl && ./a.out
```
The project is not complete, so the output is probably debug information.

## Motivation
- I read about pagerank and it sounded interesting
- I'm supposed to be learning c++ for my robotics team

## Future
It is too early to predict the future of this project.

