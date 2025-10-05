
#include "include/lyrics.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  bool alpha_present = false;
  string inputToken = token;

  int size = inputToken.size();

  for (char& c : inputToken) {
    if (isalpha(c)) {
      c = tolower(c);
      alpha_present = true;
    }
  }

  if (!alpha_present) {
    return "";
  }

  for (int i = 0; i < size; i++) {
    if (isalpha(inputToken[i])) {
      break;
    } else if (ispunct(inputToken[i])) {
      inputToken.erase(i, 1);
      i--;
      size--;
    }
  }

  for (int i = size - 1; i >= 0; i--) {
    if (isalpha(inputToken[i])) {
      break;
    } else if (ispunct(inputToken[i])) {
      inputToken.erase(i, 1);
    }
  }
  return inputToken;
}

set<string> gatherTokens(const string& text) {
  set<string> tokens;
  istringstream stream(text);
  string word;
  string cleanedWord;
  while (stream >> word) {
    cleanedWord = cleanToken(word);

    if (cleanedWord == "") {
      continue;
    }

    tokens.insert(cleanedWord);
  }

  return tokens;
}

int buildIndices(const string& filename,
                 map<string, set<string>>& inverted_index,
                 map<string, string>& artist_index) {
  ifstream infile(filename);
  vector<string> lines;
  string text;

  while (getline(infile, text)) {
    lines.push_back(text);
  }

  if (!infile.is_open()) {
    cout << "Invalid filename." << endl;
    return 0;
  }

  int size = lines.size();

  int songsProcessed = 0;

  for (int i = 0; i < size; i += 3) {
    artist_index.emplace(lines[i], lines[i + 1]);
    set<string> brokenLyrics = gatherTokens(lines[i + 2]);

    for (string s : brokenLyrics) {
      inverted_index[s].insert(lines[i]);
    }

    songsProcessed++;
  }

  return songsProcessed;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  if (sentence.size() == 0) {
    return {};
  }

  else if (!isalpha(sentence[0])) {
    return {};
  }

  set<string> result;

  istringstream stream(sentence);
  string word;

  vector<string> searchWords;
  while (stream >> word) {
    searchWords.push_back(word);
  }

  for (auto it : searchWords) {
    string cleanedWord = cleanToken(it);

    set<string> current;
    
    if (!index.count(cleanedWord)){
      current = {};
    }
    else{
      current = index.at(cleanedWord);
    }

    set<string> temp;

    if (it[0] == '+') {
      set_intersection(result.begin(), result.end(), current.begin(),
                       current.end(), inserter(temp, temp.begin()));
    } else if (it[0] == '-') {
      set_difference(result.begin(), result.end(), current.begin(),
                     current.end(), inserter(temp, temp.begin()));
    } else {
      set_union(result.begin(), result.end(), current.begin(), current.end(),
                inserter(temp, temp.begin()));
    }
    result.clear();
    result = temp;
  }
  return result;
}

void searchHelp ( 
  const map <string,string>artist_index, set<string> &artistFound){


    for (auto [key,value]: artist_index){
      artistFound.insert(artist_index.at(key));
    }

  }

void searchEngine(const string& filename) {
  int songprocessed;

  map<string, set<string>> inverted_index;
  map<string, string> artist_index;

  set <string> artistFound;

  songprocessed = buildIndices(filename, inverted_index, artist_index);
  cout << "Stand by while building indices..." << endl;
  searchHelp (artist_index,artistFound);
  cout << "Indexed " << songprocessed << " songs containing " << 
  inverted_index.size() <<" unique terms and " << artistFound.size() << " artists." << endl;
  
  string query;

  while (true){
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, query);

    if (query.size()==0){
      cout<< "Thank you for searching our Lyrics DB!" << endl;
      break;
    }

    set<string> matchedSongs = findQueryMatches(inverted_index, query);

    if(matchedSongs.size()==0){
      cout<< " Found 0 matching songs"<< endl;
    }
    else{
    cout << "Found "<< matchedSongs.size() << " matching songs" << endl;
    for (auto s: matchedSongs){
      cout << s << " by " <<artist_index.at(s) << endl;
      }
    }
  }
}
