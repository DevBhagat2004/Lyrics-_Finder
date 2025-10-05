#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/lyrics.h"

using namespace std;
using namespace testing;

TEST(CleanToken,invalidIn){
  EXPECT_THAT(cleanToken("999"),StrEq(""));
  EXPECT_THAT(cleanToken(".......T.6747876hfdtsyuT.."), StrEq("t.6747876hfdtsyut"));
  EXPECT_THAT(cleanToken("ABCD&"), StrEq("abcd"));
  EXPECT_THAT(cleanToken("ABCD"), StrEq("abcd"));
  EXPECT_THAT(cleanToken("abcd"), StrEq("abcd"));
  EXPECT_THAT(cleanToken("ab.cd"), StrEq("ab.cd"));
  EXPECT_THAT(cleanToken(".ab.cd"), StrEq("ab.cd"));
  EXPECT_THAT(cleanToken("ab.cd."), StrEq("ab.cd"));
}


TEST(GatherTokens, multiSpace) {
  string text = "hi bro     .ho%w. You   doing!";
  set<string> expected = {"hi", "bro", "ho%w", "you", "doing"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, leadSpace) {
  string text = "             I'm tired now!";
  set<string> expected = {"i'm", "tired", "now"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, trailSpace) {
  string text = "this is a Good place to practise          ";
  set<string> expected = {"this", "is", "a","good" ,"place",
                          "to", "practise"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(BuildIndices, incorrectFile){
  string filename = "randofile.txt";

  map<string, set<string>> studentInvertedIndex;
  map<string, string> studentArtistIndex;
  int studentNumProcesed =
      buildIndices(filename, studentInvertedIndex, studentArtistIndex);

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(0)) << retTestFeedback;
}

TEST(BuildIndices, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedInvertedIndex = {
      {"bells", {"Jingle bells", "Brother John", "Carol of the bells"}},        {"wool", {"Baa baa Black Sheep"}},
      {"sir", {"Baa baa Black Sheep"}}, {"jingle", {"Jingle bells"}}, {"all", {"Jingle bells","Brother John"}},
      {"the",{"Jingle bells","Carol of the bells"}}, {"way",{"Jingle bells"}}, {"are",{"Brother John"}},
      {"you",{"Brother John","Baa baa Black Sheep"}}, {"sleeping",{"Brother John"}}, {"morning",{"Brother John"}},
      {"ringing",{"Brother John"}}, {"have",{"Baa baa Black Sheep"}}, {"any",{"Baa baa Black Sheep"}}, {"yes",{"Baa baa Black Sheep"}},
      {"bags",{"Baa baa Black Sheep"}}, {"three", {"Baa baa Black Sheep"}}, {"full",{"Baa baa Black Sheep"}}, {"hark",{"Carol of the bells"}}, 
      {"how",{"Carol of the bells"}}, {"sweet",{"Carol of the bells"}}, {"silver",{"Carol of the bells"}}
    };

  map<string, string> expectedArtistIndex = {{"Jingle bells", "James Pierpont"},
                                            {"Brother John", "Unknown"},
                                            {"Baa baa Black Sheep","Unknown"},
                                            {"Carol of the bells","Mykola Leontovych"}};

  map<string, set<string>> studentInvertedIndex;
  map<string, string> studentArtistIndex;
  int studentNumProcesed =
      buildIndices(filename, studentInvertedIndex, studentArtistIndex);

  string invertedIndexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) inverted index incorrect\n";
  EXPECT_THAT(studentInvertedIndex, ContainerEq(expectedInvertedIndex))
      << invertedIndexTestFeedback;

  string artistIndexTestFeedback =
      "buildIndices(\"" + filename + "\", ...) artist index incorrect\n";
  EXPECT_THAT(studentArtistIndex, ContainerEq(expectedArtistIndex))
      << artistIndexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

map<string, set<string>> INDEX = {
    {"hello", {"Example", "School"}},
    {"there", {"Example"}},
    {"according", {"School"}},
    {"to", {"School"}},
    {"all", {"Example", "School", "Random"}},
    {"known", {"School"}},
    {"laws", {"Random"}},
    {"of", {"Random"}},
    {"aviation", {"Random"}},
    {"a", {"School", "Random"}},
};

TEST(FindQueryMatches, noFirstIndex) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "baking"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "soda"), ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "SODA!"), ContainerEq(expected));
}

TEST(FindQueryMatches, noMidPIndex) {
  
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "known +soda"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "all -hello +baking"), ContainerEq(expected));
}

TEST(FindQueryMatches, noMidMIndex) {
  
  set<string> expected;
  expected = {"Example","School"};
  EXPECT_THAT(findQueryMatches(INDEX, "all +hello -broo"), ContainerEq(expected));
}

TEST(FindQueryMatches, noModified) {
  
  set<string> expected;
  expected = {"Example"};
  EXPECT_THAT(findQueryMatches(INDEX, "there yay"), ContainerEq(expected));
}


// something for the invalid middle query token
