#!/bin/bash

cd ./txt

# Array of words and phrases to search for
one_word_queries=(
    "alas"
    "art"
    "aye"
    "beseech"
    "betwixt"
    "blest"
    "brave"
    "cast"
    "chide"
    "clamor"
    "come"
    "crown"
    "decree"
    "doth"
    "ere"
    "fair"
    "fie"
    "fleet"
    "forsooth"
    "foul"
    "fret"
    "gaze"
    "hark"
    "haste"
    "hence"
    "herald"
    "hither"
    "honesty"
    "humble"
    "kind"
    "kiss"
    "knave"
    "lady"
    "lament"
    "lest"
    "lo"
    "lord"
    "majesty"
    "marry"
    "nay"
    "noble"
    "oath"
    "perchance"
    "peril"
    "pious"
    "quoth"
    "realm"
    "renown"
    "resolve"
    "revenge"
    "ruthless"
    "scorn"
    "slain"
    "sovereign"
    "sworn"
    "sword"
    "tarry"
    "tempest"
    "thee"
    "thine"
    "thou"
    "thy"
    "tis"
    "tragedy"
    "tread"
    "treason"
    "tremble"
    "troth"
    "valor"
    "vexed"
    "vile"
    "virtue"
    "vow"
    "wanton"
    "waste"
    "woe"
    "wrath"
    "yonder"
    "zeal"
    "bold"
    "coy"
    "dear"
    "doom"
    "fickle"
    "feast"
    "fool"
    "gallant"
    "gleam"
    "haste"
    "joy"
    "jest"
    "lust"
    "mock"
    "plight"
    "serpent"
    "sigh"
    "swoon"
    "tale"
    "valor"
    "weary"
)

two_word_queries=(
    "brave new"
    "foul play"
    "fair play"
    "green-eyed monster"
    "foregone conclusion"
    "good riddance"
    "heart sore"
    "high heaven"
    "cold comfort"
    "false heart"
    "faint heart"
    "hot blood"
    "wild goose"
    "golden crown"
    "proud man"
    "noble mind"
    "bated breath"
    "cruel kindness"
    "harsh world"
    "heavenly power"
    "heavy heart"
    "honest knave"
    "hot haste"
    "hungry lion"
    "idle words"
    "ill deeds"
    "kind blessing"
    "loyal heart"
    "mad world"
    "noble lord"
    "patient mind"
    "pale face"
    "poor wretch"
    "proud heart"
    "quiet strength"
    "ruthless foe"
    "sad hours"
    "sacred trust"
    "scornful eyes"
    "sharp tooth"
    "silent moon"
    "smooth brow"
    "soft voice"
    "sour face"
    "sweet sorrow"
    "swift foot"
    "tender kiss"
    "true faith"
    "vile body"
    "weary night"
    "wicked spirit"
    "wise folly"
    "worthy cause"
    "false hope"
    "blind love"
    "broken vow"
    "burning love"
    "dead silence"
    "deep grief"
    "divine justice"
    "eternal bond"
    "evil deeds"
    "faithful friend"
    "fierce storm"
    "fickle fortune"
    "fiery eyes"
    "foolish wit"
    "forsworn faith"
    "gentle breath"
    "gilded tomb"
    "glad tidings"
    "golden sleep"
    "heavy burden"
    "honest fool"
    "humble pride"
    "jealous thoughts"
    "kind soul"
    "lawful prey"
    "light hearts"
    "mortal coil"
    "noble spirit"
    "peaceful sleep"
    "perfect love"
    "poisoned mind"
    "poor soul"
    "private grief"
    "public good"
    "purple testament"
    "raging sea"
    "shallow wit"
    "silent tear"
    "sour woe"
    "stony heart"
    "timeless honor"
    "trembling hand"
    "trusty servant"
    "twilight hours"
    "virtuous mind"
    "wanton winds"
    "wretched state"
)


# Prepare the JSON structure
output_json="{\n  \"one_word\": [\n"

# Search for each one-word term and collect the results
for query in "${one_word_queries[@]}"; do
    echo "Searching for: $query"
    # Find all .txt files containing the word
    search_results=$(find . -name "*.txt" -exec grep -il "$query" {} +)

    # Convert search results into an array of file names (without path or extension)
    result_ids=()
    while IFS= read -r result; do
        filename=$(basename "$result" .txt)
        result_ids+=("\"$filename\"")
    done <<< "$search_results"

    # Add to the JSON structure
    output_json+="    {\n      \"query\": \"$query\",\n      \"expected_result_ID\": [\n        $(IFS=,; echo "${result_ids[*]}")\n      ]\n    },\n"
    echo "--------------------------------"
done

# Remove the last comma and close the one_word JSON array
output_json=$(echo "$output_json" | sed '$ s/,$//')
output_json+="  ],\n  \"two_word\": [\n"

# Search for each two-word phrase and collect the results
for query in "${two_word_queries[@]}"; do
    echo "Searching for: $query"
    # Split the two-word query into individual words
    word1=$(echo "$query" | cut -d' ' -f1)
    word2=$(echo "$query" | cut -d' ' -f2)

    # Search for each word separately and find common files
    search_results_word1=$(find . -name "*.txt" -exec grep -il "$word1" {} +)
    search_results_word2=$(find . -name "*.txt" -exec grep -il "$word2" {} +)

    # Convert the results into arrays and find the intersection
    result_ids=()
    while IFS= read -r file1; do
        filename1=$(basename "$file1" .txt)
        while IFS= read -r file2; do
            filename2=$(basename "$file2" .txt)
            if [[ "$filename1" == "$filename2" ]]; then
                result_ids+=("\"$filename1\"")
            fi
        done <<< "$search_results_word2"
    done <<< "$search_results_word1"

    # Add to the JSON structure
    output_json+="    {\n      \"query\": \"$query\",\n      \"expected_result_ID\": [\n        $(IFS=,; echo "${result_ids[*]}")\n      ]\n    },\n"
    echo "--------------------------------"
done

# Remove the last comma and close the two_word JSON array
output_json=$(echo "$output_json" | sed '$ s/,$//')
output_json+="  ]\n}"

cd -

# Write the output to a .json file
echo -e "$output_json" > data.json