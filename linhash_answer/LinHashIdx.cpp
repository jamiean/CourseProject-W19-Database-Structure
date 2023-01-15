#include "LinHashIdx.hpp"

#include <functional>
#include <math.h>
#include <cassert>
#include <bitset>
#include <iostream>

LinHashIdx::LinHashIdx() : next(0), level(0)
{
    // Create all initial buckets
    directory.resize(INITIAL_NUM_BUCKETS);
    for (auto& bucketPtr : directory)
    {
        bucketPtr = new Bucket;
    }
}

// Insert the input into the correct bucket, and split the "next" bucket if the insert causes an overflow
void LinHashIdx::insert(std::string input)
{
    bool split = insertHelper(input);

    if (split)
    {
        // If the next bucket needs to be split, gather all of its keys, and destroy it,
        // along with all of the overflow buckets
        std::vector<std::string> orderedKeys = directory[next]->getOrderedKeys();
        delete directory[next];

        // Create new buckets for the split keys
        directory[next] = new Bucket;
        directory.push_back(new Bucket);

        // Increment next, and if it's high enough, it overflows and the level increases
        ++next;
        if (next >= INITIAL_NUM_BUCKETS * pow(2, level))
        {
            next = 0;
            ++level;
        }

        // Insert all of the split keys into the appropriate buckets
        for (auto& key : orderedKeys)
        {
            insertHelper(key);
        }
    }
}

// Helper function for inserts.
// Insert the input into the correct bucket.
// Return true if the insert was in an overflow bucket
bool LinHashIdx::insertHelper(std::string input)
{
    // The bucket index is hash(key) % N * 2^L
    // If this index is less than the "next" bucket,
    // then it becomes hash(key) % N * 2^(L+1)
    size_t hashVal = std::hash<std::string>{}(input);
    unsigned int modulus = INITIAL_NUM_BUCKETS * pow(2, level);
    size_t bucketIdx = hashVal % modulus;

    if (bucketIdx < next)
    {
        modulus *= 2;
        bucketIdx = hashVal % modulus;
    }

    assert(bucketIdx < directory.size());

    // Insert into the bucket, and return true if it's an overflow bucket
    return directory[bucketIdx]->insert(input);
}

// Print the entire index.
// Prints the current level, each directory entry, and the keys in each bucket.
// Also points to the "next" bucket
void LinHashIdx::print()
{
    // The number of characters used to display the binary value of each bucket index
    size_t levelSize = level + log2(INITIAL_NUM_BUCKETS) + 1;
    std::string blankSpace(levelSize, ' ');

    std::cout << "Level: " << level << std::endl;

    for (unsigned int i = 0; i < directory.size(); ++i)
    {
        // The "next" bucket gets a pointer
        std::string nextStr(4, ' ');
        if (i == next)
        {
            nextStr = "--> ";
        }

        // The high-level representation of the bucket index uses 1 more bit than the low level representation
        std::bitset<BITSET_LEN> bitHash = i;
        std::string highLevelStr, lowLevelStr;

        highLevelStr = bitHash.to_string().substr(BITSET_LEN - levelSize, levelSize);
        highLevelStr += " ";
        lowLevelStr = highLevelStr.substr(1, levelSize);

        // If it's the beginning of a level, only the low level bucket index is displayed
        if (next == 0)
        {
            highLevelStr = "";
        }

        // Buckets above the initial set do not have a low level representation
        if (i >= INITIAL_NUM_BUCKETS * pow(2, level))
        {
            lowLevelStr = blankSpace;
        }

        std::cout << nextStr << highLevelStr << lowLevelStr;
        directory[i]->print();
        std::cout << std::endl;
    }

    std::cout << std::endl;
}