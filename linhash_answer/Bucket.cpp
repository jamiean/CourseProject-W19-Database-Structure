#include "Bucket.hpp"

#include <iostream>
#include <algorithm>

Bucket::Bucket() : overflowBucket(nullptr)
{

}

Bucket::~Bucket()
{
    if (overflowBucket)
    {
        delete overflowBucket;
    }
}

// Insert the key into the bucket, or one of its overflow buckets.
// Return true if an overflow bucket was used
bool Bucket::insert(std::string input)
{
    bool returnVal = false;
    if (keys.size() == MAX_BUCKET_SIZE)
    {
        returnVal = true;
    }

    insertHelper(input);

    return returnVal;
}

void Bucket::insertHelper(std::string input)
{
    // If there's room in this bucket, push back and be done
    if (keys.size() < MAX_BUCKET_SIZE)
    {
        keys.push_back(input);
    }
    // If there's no room, insert into overflowBucket
    else
    {
        // Create overflowBucket if it doesn't exist already
        if (!overflowBucket)
        {
            overflowBucket = new Bucket;
        }
        
        overflowBucket->insertHelper(input);
    }
}

// Get all keys in this bucket and all of its overflow buckets, in order
std::vector<std::string> Bucket::getOrderedKeys()
{
    std::vector<std::string> orderedKeys = keys;

    Bucket* nextBucket = overflowBucket;
    while(nextBucket)
    {
        orderedKeys.insert(orderedKeys.end(), nextBucket->keys.begin(), nextBucket->keys.end());
        nextBucket = nextBucket->overflowBucket;
    }
    
    std::sort(orderedKeys.begin(), orderedKeys.end());

    return orderedKeys;
}

// Print all keys in this bucket and all of its overflow buckets.
// Keys are separated by spaces, and buckets are separated by '|'.
void Bucket::print()
{
    std::cout << "| ";
    for (auto& key : keys)
    {
        std::cout << key << " ";
    }
    std::cout << "|";

    if (overflowBucket)
    {
        overflowBucket->print();
    }
}