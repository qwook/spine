/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

// todo: label all the errors

#include "keyval.h"
#include <string>

using namespace std;

KeyValue::KeyValue( TYPE type, std::string data )
: type(type), dat(data) {}

KeyValue::KeyValue( TYPE type )
: type(type) {}

KeyValue::~KeyValue() {
    for (KeyValueMap::iterator i = map.begin(); i != map.end(); ++i) {
        delete i->second;
    }
}

void KeyValue::insert(std::string key, KeyValue *val) {
    map[key] = val;
}

KeyValue *KeyValue::getMember(std::string key) {
    return map[key];
}

KeyValue *KeyValue::getMember(unsigned int i) {
    char key[50];
    sprintf(key, "%u", i);
    return map[key];
}

std::string KeyValue::getString() {
    return dat;
}

int KeyValue::getInteger() {
    return atoi(dat.c_str());
}

float KeyValue::getFloat() {
    return atof(dat.c_str());
}

void KeyValue::release() {
    delete this;
}

KeyValueParser::KeyValueParser(char *todat) : dat(todat) {
    offset = 0;
    max = strlen(todat);
    keyval = parse();
}

KeyValueParser::~KeyValueParser() {
}

KeyValue *KeyValueParser::getKeyValue() {
    return keyval;
}

KeyValueParser::Token KeyValueParser::getNextToken(const char *dat, unsigned long *offset, unsigned long max) {
    KeyValueParser::Token t;
    t.type = KeyValueParser::NONE;
    t.lineno = 0;

    int len = 0;
    char string_atom;
    bool float_rhs = false;
    bool string_escape = false;
    
    int comment = 0;

    int i;
    for (i = *offset; i <= max; i++ )
    {
        char atom;

        if (i < max) {
            atom = dat[i];
        } else {
            atom = '\0';
        }

        // Alright, so this is the tokenizer, basically.
        
        // If this is the first character, we should determine
        // What the hell we're parsing.
        if ( t.type == KeyValueParser::NONE )
        {
            /*
            **  I must confess,
            **  I should reassess
            **  This great big mess.
            */
            
            // skip comments
            if (comment < 2) {
                if (atom == '/') {
                    comment++;
                    continue;
                } else {
                    if (atom == '*' && comment == 1) {
                        comment = 4;
                        continue;
                    }
                    comment = 0;
                }
            } else {
                if (comment == 3) {
                    if (atom == '/') {
                        comment = 0;
                    } else {
                        comment = 4;
                    }
                }
                if (atom == '\n' && comment == 2) {
                    comment = 0;
                }
                if (atom == '*' && comment == 4 ) {
                    comment = 3;
                }
                char t[2] = {atom,'\0'};
                continue;
            }

            // skip whitespace
            if (atom == ' ' || atom == '\t' || atom == '\n') {
                continue;
            }

            // it's a string
            if (atom == '\"' or atom == '\'') {
                string_atom = atom;
                t.type = KeyValueParser::STRING;
                continue;
            }

            if (atom == '=') {
                t.type = KeyValueParser::SETOP;
                len++;
                i++;
                break;
            }

            if (atom == ',') {
                t.type = KeyValueParser::COMMA;
                len++;
                i++;
                break;
            }

            if (atom == '(') {
                t.type = KeyValueParser::PARSTART;
                len++;
                i++;
                break;
            }

            if (atom == ')') {
                t.type = KeyValueParser::PAREND;
                len++;
                i++;
                break;
            }

            if (atom == '{') {
                t.type = KeyValueParser::OBJSTART;
                len++;
                i++;
                break;
            }

            if (atom == '}') {
                t.type = KeyValueParser::OBJEND;
                len++;
                i++;
                break;
            }

            if ((atom >= 'A' && atom <= 'z') || atom == '_') {
                t.type = KeyValueParser::VARIABLE;
                len++;
                continue;
            }

            if (atom >= '0' && atom <= '9') {
                t.type = KeyValueParser::INTEGER;
                len++;
                continue;
            }

            if (atom == '.') {
                t.type = KeyValueParser::FLOAT;
                float_rhs = true;
                len++;
                continue;
            }

            printf("Error?\n");
            throw(1);
        }

        // Once we determined what we are parsing,
        // We should know what to expect from a token.
        if (t.type == KeyValueParser::STRING) {
            if (atom == '\0') {
                printf("Error?\n");
                throw(1);
            }
            if (atom == '\\' && !string_escape) {
                string_escape = true;
                len++;
                continue;
            } else {
                if (atom == string_atom && !string_escape) {
                    break;
                }
                string_escape = false;
                len++;
                continue;
            }
        } else if (t.type == KeyValueParser::VARIABLE) {
            if ((atom >= 'A' && atom <= 'z') || (atom >= '0' && atom <= '9') || atom == '_') {
                t.type = KeyValueParser::VARIABLE;
                len++;
                continue;
            }
        } else if (t.type == KeyValueParser::INTEGER) {
            if (atom >= '0' && atom <= '9') {
                len++;
                continue;
            } else if (atom == '.') {
                float_rhs = true;
                t.type = KeyValueParser::FLOAT;
                len++;
                continue;
            }
        } else if (t.type == KeyValueParser::FLOAT) {
            if (atom >= '0' && atom <= '9') {
                len++;
                continue;
            } else if (atom == '.' && !float_rhs) {
                float_rhs = true;
                len++;
                continue;
            }
        }

        break;
    }

    if (t.type == KeyValueParser::STRING) {
        char *temp = new char[len+1];
        int k = 0;
        bool escape = false;
        for (int j = i - len; j < i; j++) {
            char atom = dat[j];
            if (escape) {
                // todo: special characters
                escape = false;
            } else {
                if (atom == '\\') {
                    escape = true;
                    continue;
                }
            }
            temp[k] = dat[j];
            k++;
        }
        temp[k] = '\0';
        t.value = std::string(temp);
        delete temp;

        // We didn't increase the position when we ended the string with the end-quote.
        // So we increase it here.
        i++;
    } else {
        char *temp = new char[len+1];
        strncpy(temp, dat + i - len, len);
        temp[len] = '\0';
        t.value = std::string(temp);
        delete temp;
    }

    memcpy(offset, &i, sizeof(int));

    return t;
}

// Get the next token without actually traveling there.
KeyValueParser::Token KeyValueParser::getNextTokenProtected(const char *dat, unsigned long *offset, unsigned long max) {
    unsigned long offset_protected;
    memcpy(&offset_protected, offset, sizeof(long));
    return getNextToken(dat, &offset_protected, max);
}

KeyValue *KeyValueParser::parse() {
    KeyValueParser::Token t = getNextToken( dat, &offset, max );

    unsigned int i = 0;
    if (t.type == KeyValueParser::INTEGER) {
        return new KeyValue( KeyValue::INTEGER, t.value );
    } else if (t.type == KeyValueParser::FLOAT) {
        return new KeyValue( KeyValue::FLOAT, t.value );
    } else if (t.type == KeyValueParser::STRING) {
        return new KeyValue( KeyValue::STRING, t.value );
    } else if (t.type == KeyValueParser::OBJSTART) {
        KeyValue *map = new KeyValue( KeyValue::MAP );
        while (true) {
            KeyValueParser::Token t_expect = getNextTokenProtected( dat, &offset, max );
            if (t_expect.type == KeyValueParser::VARIABLE) {
                KeyValueParser::Token variable = getNextToken( dat, &offset, max );
                KeyValueParser::Token eq = getNextToken( dat, &offset, max );
                if (eq.type != KeyValueParser::SETOP) {
                    printf("Error?");
                    exit(1);
                }
                map->insert(variable.value, parse());
            } else {
                char *key = new char[50];
                sprintf(key, "%u", i);
                map->insert(std::string(key), parse());
                delete key;
                i++;
            }
            KeyValueParser::Token end = getNextToken( dat, &offset, max );
            if (end.type == KeyValueParser::COMMA) {
                continue;
            } else if (end.type == KeyValueParser::OBJEND) {
                break;
            }
            printf("Error?");
            throw(1);
        }
        return map;
    }
    printf("Error?");
    throw(1);
}
