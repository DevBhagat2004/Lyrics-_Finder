# Lyrics_Finder

A high-performance lyrics search engine built in C++ that enables complex queries across song databases using an inverted index architecture.

## Overview

Lyrics_Finder implements a search engine that indexes song lyrics and supports sophisticated query operations including AND, OR, and NOT logic. The system uses an inverted index data structure for efficient song retrieval based on lyrical content.

## Features

- **Inverted Index Architecture**: Optimized data structure mapping terms to songs for fast lookup
- **Complex Query Support**: 
  - Single word searches (`love`)
  - OR operations (`love time` - songs with either term)
  - AND operations (`night +day` - songs with both terms)
  - NOT operations (`eyes -heart` - songs with first term but not second)
  - Compound queries (`love -heart night +bad`)
- **Dual Index System**: Maintains both song-to-artist and term-to-song mappings
- **Case-Insensitive Matching**: Normalizes text for consistent results
- **Interactive Command Loop**: Real-time query processing

## Demo

```
Enter a filename: data/tiny.txt
Stand by while building indices...
Indexed 4 songs containing 22 unique terms and 3 artists.

Enter query sentence (press enter to quit): wool
Found 1 matching songs
Baa baa Black Sheep by Unknown

Enter query sentence (press enter to quit): bells
Found 3 matching songs
Brother John by Unknown
Carol of the bells by Mykola Leontovych
Jingle bells by James Pierpont

Enter query sentence (press enter to quit): bells +jingle
Found 1 matching songs
Jingle bells by James Pierpont
```

## Technical Implementation

### Core Components

**cleanToken**: Normalizes tokens by converting to lowercase, removing leading/trailing punctuation, and filtering tokens without letters.

**gatherTokens**: Extracts unique terms from text using string stream processing and set data structures.

**buildIndices**: Constructs two indices from song files:
- Inverted index: `map<string, set<string>>` mapping terms → songs
- Artist index: `map<string, string>` mapping songs → artists

**findQueryMatches**: Processes queries with modifiers using STL set operations (`set_union`, `set_intersection`, `set_difference`).

**searchEngine**: Main interactive loop coordinating index building and query processing.

### Data Format

Song files follow a triple-line format:
```
Song Title
Artist Name
Lyrics as single line
```

## Build & Usage

### Prerequisites
- C++ compiler with C++11 support or higher
- Google Test framework (included)
- Make

### Build Commands

```bash
# Build and run all tests
make tests
./lyrics_tests

# Run specific test suites
make test_clean_token
make test_gather_tokens
make test_build_indices
make test_find_query_matches
make test_search_engine

# Build the main application
make lyrics_main

# Build and run the search engine
make run_lyrics
```

### Running the Application

```bash
# Option 1: Build and run in one command
make run_lyrics

# Option 2: Run the built executable
./lyrics_main
```

Then enter a data file path when prompted (e.g., `data/tiny.txt`).

## Query Syntax

| Query | Meaning | Example |
|-------|---------|---------|
| `term` | Songs containing term | `love` |
| `term1 term2` | Songs with term1 OR term2 | `love hate` |
| `term1 +term2` | Songs with term1 AND term2 | `night +day` |
| `term1 -term2` | Songs with term1 but NOT term2 | `eyes -heart` |

Queries are evaluated left-to-right, enabling complex expressions like:
```
love -heart night +bad
→ ((("love" without "heart") or "night") and "bad")
```

## Testing

The project implements Test-Driven Development (TDD) using Google Test framework. Tests cover:

- Token cleaning with edge cases (punctuation, case sensitivity, special characters)
- Token gathering with whitespace handling
- Index building with file I/O operations
- Query matching with all modifier combinations
- Integration testing of the complete search pipeline

## Technical Highlights

- **STL Mastery**: Extensive use of `map`, `set`, `vector`, and `<algorithm>` library
- **Memory Efficiency**: No dynamic allocation; uses STL containers for automatic memory management
- **Modular Design**: Clean separation of concerns with single-responsibility functions
- **Error Handling**: Graceful handling of missing files and empty query results
- **Scalability**: Index structure supports datasets from 4 songs to thousands