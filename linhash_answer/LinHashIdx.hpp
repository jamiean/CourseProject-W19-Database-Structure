#ifndef _LINHASHIDX_HPP_
#define _LINHASHIDX_HPP_

#include <vector>

#include "Bucket.hpp"
#include "constants.hpp"

class LinHashIdx
{
public:

    LinHashIdx();
    void insert(std::string input);
    void print();

private:

    bool insertHelper(std::string input);

    std::vector<Bucket*> directory;
    unsigned int next;
    unsigned int level;

};

#endif