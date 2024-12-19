#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Function to handle the response from the server
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    printf("Server Response: %s\n", (char *)ptr);
    return size * nmemb;
}

int main() {
    CURL *curl;
    CURLcode res;

    // Variables to hold user input and JSON payload
    char city[50];
    char info[50];
    char jsonPayload[200];

    while(1) {

    // Prompt user for city and specific information
    printf("Please enter the city (e.g., Damascus, Paris, Tokyo): ");
    scanf("%49s", city); // Prevent buffer overflow
    printf("What information do you need about this city? (Options: temperature, description, humidity, pressure): ");
    scanf("%49s", info); // Prevent buffer overflow

    // Prepare the JSON payload
    snprintf(jsonPayload, sizeof(jsonPayload), "{\"city\":\"%s\", \"info\":\"%s\"}", city, info);

    // Debugging print to verify the payload
    printf("Preparing request with the following data: %s\n", jsonPayload);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL for the POST request
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/cities");

        // Set the payload (JSON data)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload);

        // Set the write callback to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Add headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the HTTP POST request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "Error: Unable to send request. %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        fprintf(stderr, "Error: Unable to initialize CURL.\n");
    }

    // Cleanup libcurl
    curl_global_cleanup();
    }

    return 0;
}
