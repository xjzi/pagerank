#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <curl/curl.h>
#include <rapidjson/document.h>

class Wikipedia {
	CURL *curl;

	static std::vector<std::string> parseJSON(std::string json) {
		rapidjson::Document d;
		d.Parse(json.c_str());
	
		rapidjson::Value& links = d["parse"]["links"];
	
		std::vector<std::string> result;
		for (const rapidjson::Value& link: links.GetArray()) {
			if (link["ns"] == 0) {
				result.push_back(link["*"].GetString());
			}
		}
	
		return result;
	}
	
	// Called by libcurl to write string to a variable
	static size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
		((std::string*) stream) -> append((char*) ptr, 0, size * count);
		return size * count;
	}
	
	std::string fetchJSON(std::string page) {
		CURLcode res;
		std::string response;
	
	       	// Sanitize page title to url. Replace spaces with underscores, and encode with percentages.
	       	std::replace(page.begin(), page.end(), ' ', '_');
	       	char *output = curl_easy_escape(curl, page.c_str(), page.size());
	       	std::string sanitized(output);
	       	curl_free(output);

	       	// Use the wikipedia API to only request a JSON object with all internal links
		// prop=links: get all internal links on the requested page
	       	std::string url = "https://en.wikipedia.org/w/api.php?action=parse&format=json&prop=links&page=" + sanitized;

	       	// URL and parameters for API 	
	       	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	
	       	// Buffer to store response
	       	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	
	       	// Return code
	       	res = curl_easy_perform(curl);
	
	       	// Check for errors
	       	if(res != CURLE_OK) {
	       		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	       	}
	 
		return response;
	}
	
public:
	Wikipedia(std::string contact) {
		curl = curl_easy_init();
		if (!curl) {
			fprintf(stderr, "curl_easy_init() failed");
			exit(1);
		}
		
		// Etiquette to avoid being rate-limited
		std::string userAgent = "libcurl/7.79 (" + contact + ")";
		curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str()); 
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
		
		// Write result to variable instead of stdout
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
	}

	~Wikipedia() {
		curl_easy_cleanup(curl);
	}

	std::vector<std::string> GetPage(std::string page) {
		std::string json = fetchJSON(page);
		return parseJSON(json);
	}
};
