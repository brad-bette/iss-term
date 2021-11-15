#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>  // for threads
#include <chrono> // for time
#include <curl/curl.h>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using std::flush;


// print the main logo
void banner() {

cout << "  __      ________  ________  "   << endl
     << " |' \\    /'       )/'       ) "  << endl
     << " ||  |  (:   \\___/(:   \\___/  " << endl
     << " |:  |   \\___  \\   \\___  \\   "  << endl
     << " |.  |    __/  \\\\   __/  \\\\  "  << endl
     << " /\\  |\\  '' \\   :) /' \\   :) " << endl
     << "(__\\_|_)(_______/ (_______/  "   << endl
     << endl << "international space station location:" << endl << endl;
}

// as shown in the curl example to make the curl (getData) function work
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// gets data from the iss-now api 
// returns it as a string
//
// also was from an example that I found on stackoverflow
string getData() {
  CURL *curl;
  CURLcode res;
  string readBuffer;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://api.open-notify.org/iss-now.json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
    return readBuffer;
}


// removes any thing that I'm not looking for Then adds it to a new string
// returns the modifed output as a string
string removeUneeded(string data) {

  string news;
  for ( int x = 0; x <= (data.size() - 1); x++ ) {

      if ( data[x] != '{' && data[x] != '}' && data[x] != '"' && data[x] != ':' && data[x] != ',' ) {
	 news += data[x];
      }
  }

  return news;
}

// returns a vector to the program
// converts a string to a vector type. Therefore its easier to go through
//
// Got from:
// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
vector<string> tovector(string input) {

   string buffer;                 // Have a buffer string
   stringstream ss(input);       // Insert the string into a stream
   vector<string> stuff;        // Create vector to hold our words

   // iterate through the stream per word
   while (ss >> buffer)
        stuff.push_back(buffer);

   return stuff;

}

// finds lat and long & returns 2 int digits
// takes the vector data by refrence and an array indexes[2] 
// Modifies the array to point to the corridints of both [lat, long].
void findlatlong(vector<string> &stuff, int *indexes) {

 string things[2] = {"latitude", "longitude"};

 // go through the array for both lat and long
  for ( int i = 0; i <= 1; ++i ) {

    // search through the array for lat and long
    // if theres a match, add it to the array
    for ( int x = 0; x <= (stuff.size() - 1); ++x ) {
          if ( stuff[x] == things[i] ) {
             indexes[i] = x;
	  }
      }
  }
}

// Print the data to the consol
void printer(vector<string> &data, int *index) {

  std::fflush(stdout);
  cout << " " << data[index[0]] << " " << data[index[0] + 1] << "\t" << data[index[1]] << " " 
       << data[index[1] + 1] << "\r" << flush;;

}

int main() {

  // indexes for [ lat, long ]
  int index[2] = {0};

  banner();

  while (true) {

    string input = getData();

    // filter out the uneeded text
    string news = removeUneeded(input);

    // get the vector form the function
    vector<string> data = tovector(news);

    // find the indexes of lat and long 
    findlatlong(data, index);

    // print the sumbitch
    printer(data, index);

    data.clear();
    
    // pause the program execution for 2 seconds
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

}

/*
Copyright 2021-2022 Bradley Bettencourt 

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
