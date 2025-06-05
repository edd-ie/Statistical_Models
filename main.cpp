#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "curl/curl.h"

#include "./TestFile.cpp"

// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

int main() {
    // testOptionInfo();

    CURL* curl = curl_easy_init();
    if (curl) {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=AAPL&apikey=YOUR_API_KEY&datatype=json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        if (CURLcode response = curl_easy_perform(curl); response != CURLE_OK) {
            std::cout << "Failed to fetch: " << curl_easy_strerror(response) << std::endl;
        } else {
            // Handle success
        }
    }


    curl_easy_cleanup(curl);

    return 0;
}
