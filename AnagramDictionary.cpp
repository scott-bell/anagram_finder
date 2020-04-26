#define CATCH_CONFIG_MAIN

#include <iostream>
#include <fstream>
#include "catch.hpp"

std::vector<std::string> read_all_words() {
	std::vector<std::string> result;
	{
		std::ifstream file;
		file.open("words_alpha.txt");
		std::string line;
		while (std::getline(file, line)) {
			result.push_back(line);
		}
		file.close();
	}
	return result;
}


TEST_CASE("read all words") {
	auto words = read_all_words();
	REQUIRE_FALSE(words.empty());
	CHECK(words.size() == 370104);
	CHECK(words.front() == "a");
	CHECK(words.back() == "zwitterionic");
}

using WordSet = std::set<std::string>;

struct MostAnagrams {
	bool empty() const {
		return word_sets.empty() || word_sets.begin()->empty();
	}
	size_t anagram_count() const {
		return word_sets.begin()->size();
	};
	std::set<WordSet> word_sets;
};
struct LongestAnagrams {
	bool empty() const {
		return word_sets.empty() || word_sets.begin()->empty();
	}
	size_t word_length() const { return word_sets.begin()->begin()->size(); }
	std::set<WordSet> word_sets;
};
struct ExcitingAnagrams {
	MostAnagrams most_anagrams{};
	LongestAnagrams longest_anagrams{};
};
ExcitingAnagrams find_anagrams(const std::vector<std::string>& words) {
	ExcitingAnagrams result;
	std::map<std::string, WordSet> all_anagrams;

	WordSet* most = nullptr;
	WordSet* longest = nullptr;

	unsigned int length = 0;
	for (const std::string& word : words) {
		std::string sorted_word = word;
		std::sort(sorted_word.begin(), sorted_word.end());
		all_anagrams[sorted_word].insert(word);

		if ((result.most_anagrams.empty() || all_anagrams[sorted_word].size() > result.most_anagrams.anagram_count())) {
			result.most_anagrams.word_sets = { all_anagrams[sorted_word] };
		}
		else if (all_anagrams[sorted_word].size() == result.most_anagrams.anagram_count()) {
			result.most_anagrams.word_sets.insert(all_anagrams[sorted_word]);
		}
		if (all_anagrams[sorted_word].size() > 1) {
			if ((result.longest_anagrams.empty() || sorted_word.length() > result.longest_anagrams.word_length())) {
				result.longest_anagrams.word_sets = { all_anagrams[sorted_word] };
			}
			else if (sorted_word.length() == result.longest_anagrams.word_length()) {
				result.longest_anagrams.word_sets.insert(all_anagrams[sorted_word]);
			}
		}
	}

	return result;
}

TEST_CASE("find anagrams - test template") {
	const auto exciting_anagrams = find_anagrams(read_all_words());
	REQUIRE_FALSE(exciting_anagrams.most_anagrams.empty());
	REQUIRE_FALSE(exciting_anagrams.longest_anagrams.empty());

	REQUIRE(exciting_anagrams.most_anagrams.anagram_count() == 15);
	REQUIRE(exciting_anagrams.longest_anagrams.word_length() == 22);

	REQUIRE(exciting_anagrams.most_anagrams.word_sets == std::set<WordSet>{ 
		{"alerts", "alters", "artels", "estral", "laster", "lastre", "rastle", "ratels", "relast", "resalt", "salter", "slater", "staler", "stelar", "talers"}
	});
	REQUIRE(exciting_anagrams.longest_anagrams.word_sets == std::set<WordSet>{
		{"chlorotrifluoromethane", "trifluorochloromethane"},
		{"cholecystoduodenostomy", "duodenocholecystostomy"},
		{"hydropneumopericardium", "pneumohydropericardium"}
	});

}