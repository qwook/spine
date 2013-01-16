
#include <iostream>
#include <map>

#include "modulemanager.h"
#include "ios.h"

using namespace std;

IModuleManager *modulemanager;
IOSModule *os;

// written by Henry and I'm very proud of it.
bool wildcard_match( const char *wildcard, const char *match )
{
    int match_len = strlen(match);
    int wildcard_len = strlen(wildcard);

    if ( match_len == wildcard_len && wildcard_len == 0 )
        return true;

    int wild_pos = 0;
    int orig_wild = 0;
    int orig_match = 1;
    bool first_match = true;
    for( int i = 0; i < match_len; i++ )
    {
        while ( wildcard[wild_pos] == '*' )
        {
            wild_pos++;
            orig_wild = wild_pos;
        }

        const char z[] = { match[i], '\0' };
        const char a[] = { wildcard[wild_pos], '\0' };

        if ( wild_pos < wildcard_len && match[i] == wildcard[wild_pos] )
        {
            wild_pos++;
            if (first_match)
            {
                first_match = false;
                orig_match = i;
            }
            continue;
        }

        if ( wild_pos > 0 && wildcard[wild_pos-1] == '*' )
        {
            first_match = true;
            continue;
        }

        i = orig_match;
        orig_match++;
        wild_pos = orig_wild;
    }

    return wild_pos == wildcard_len;
}

int main( int argc, const char **argv )
{
    modulemanager = new CModuleManager;
    IOSModule *os = (IOSModule *)modulemanager->loadModule("os");

    float lastFrameTime = os->getTimeInternal();
    const float FPSLimit = 30; // TODO: Make this an option.
    while (true)
    {
        // Framelimit with internal time.
        float currentTime = os->getTimeInternal();
        if ( (currentTime - lastFrameTime) < ( 0.001 / FPSLimit ) )
            continue;
        lastFrameTime = currentTime;

        // Update the os module.
        os->update();
        printf("%f\n", os->getFPS());
    }

    delete (CModuleManager *)modulemanager;
    return 0;
}