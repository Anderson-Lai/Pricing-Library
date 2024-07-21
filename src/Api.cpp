#include "Api.hpp"
#include "Time.hpp"
#include "Log.h"

// call the url specified
const std::string& Api::CallApi(const char* url) {
	curl_easy_setopt(m_curl, CURLOPT_URL, url);

	// clear the buffer
	m_buffer.clear();

	m_result = curl_easy_perform(m_curl);
	if (m_result != CURLE_OK) {
		Log::Error("Failed to call api");
	}

	return m_buffer;
}

// setup curl to perform request (no url specified yet)
Api::Api() {
	m_curl = curl_easy_init();
	m_result = CURLE_OK;

	if (!m_curl) {
		Log::Error("Failed to initialize curl");
	}
	else {
		// set callback fn and write data to buffer
		m_result = curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteData);
		if (m_result != CURLE_OK) {
			Log::Error("Could not set the callback function for the curl handle");
		}

		m_result = curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_buffer);
		if (m_result != CURLE_OK) {
			Log::Error("Could not set the buffer for the curl handle to write the response to");
		}

		curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L); // keep TCP connection alive
		curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPIDLE, 10L); // after 10 sec send the first keep-alive probe
		curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPINTVL, 5L); // send a probe every 5 sec after the first keep-alive probe
	}
}

// cleanup
Api::~Api() {
	if (m_curl) {
		curl_easy_cleanup(m_curl);
	}
}

// function to write to m_buffer 
size_t Api::WriteData(void* responseContents, size_t elementSize, size_t numberOfElements, void* userPointer) {
	std::string* buffer = (std::string*)userPointer; // typecasting

	size_t arraySize = elementSize * numberOfElements;

	// concatenate the reponse to the old one (response can come in chunks)
	buffer->append((char*)responseContents, arraySize); 

	return arraySize;
}