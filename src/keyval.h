/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

#ifndef KEYVAL_H
#define KEYVAL_H

#include <map>
#include <vector>
#include <string>

using namespace std;

struct cmp_str {
    bool operator() (const char *a, const char *b) {
        return strcmp(a, b) < 0;
    }
};

class KeyValue {
public:
    typedef enum {
        MAP    = 0,
        STRING      = 1,
        INTEGER     = 2,
        FLOAT       = 3,
    } TYPE;
    typedef map<std::string, KeyValue *> KeyValueMap;
private:
    TYPE type;
    std::string dat;
    KeyValueMap map;
public:
    
    KeyValue( TYPE type, std::string dat );
    KeyValue( TYPE type );
    ~KeyValue();

    void insert(std::string, KeyValue *);
    KeyValue *getMember(std::string key);
    KeyValue *getMember(unsigned int key);
    std::string getString();
    int getInteger();
    float getFloat();
    
    void release();
};

class KeyValueParser {
public:
    typedef enum {
        NONE = 0,
        VARIABLE = 1,
        SETOP = 2,
        PARSTART = 3,
        PAREND = 4,
        OBJSTART = 5,
        OBJEND = 6,
        STRING = 7,
        INTEGER = 8,
        FLOAT = 9,
        COMMA = 10
    } TOKENTYPE;

    typedef struct {
        unsigned int lineno;
        KeyValueParser::TOKENTYPE type;
        std::string value;
    } Token;
    
    KeyValueParser(char *dat);
    ~KeyValueParser();
    KeyValue *getKeyValue();
    char *dat;

private:
    unsigned int offset;
    unsigned long max;
    KeyValue *keyval;

    Token getNextToken(const char *dat, unsigned int *offset, unsigned int max);
    Token getNextTokenProtected(const char *dat, unsigned int *offset, unsigned int max);
    KeyValue *parse();
};

#endif
