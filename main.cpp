#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "wikipedia.hpp"

std::string readContact() {
	std::ifstream ifs("contact.txt");
	if (!ifs.is_open()) {
		return "Anonymous";
	}

	std::string contact( (std::istreambuf_iterator<char>(ifs) ),
               (std::istreambuf_iterator<char>()    ) );

	return contact;
}

int main(int argc, char** argv) {
	curl_global_init(CURL_GLOBAL_DEFAULT);

	Wikipedia source(readContact());

	std::vector<std::string> page = source.GetPage(argv[1]);

	for (std::string subpage: page) {
		std::cout << subpage << std::endl;
		std::cout << source.GetPage(subpage).size() << std::endl;
	}

	curl_global_cleanup();
}
