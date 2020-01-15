#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#ifdef __BORLANDC__
    // I'm not 100% convinced about this, but Embarcadero said it was fine, so ....
    // But the Borland version of <cstdlib> defines srand() and rand()
    // however, compiling fails unless I include this file. Sigh.
    #include <stdlib.h>
#endif

using std::cout;
using std::endl;
using std::string;
using std::begin;
using std::end;
using std::random_shuffle;

const uint8_t passwordLength = 15;

const uint8_t numberOfUppers = 2;
const uint8_t numberOfLowers = 2;
const uint8_t numberOfDigits = 2;
const uint8_t numberOfSpecials = 2;



void genPassword(string &newPassword) {
    static const string uppers = "ABCDEFGHIJKLMNPQRSTUVWXYZ";
    static const string lowers = "abcdefghijkmnoprstuvwxyz";
    static const string letters = lowers + uppers;
    static const string numbers = "23456789";
    static const string specials = "$_#";

    // Save effort by pre-filling the string to a big enough length.
    if (newPassword.capacity() < passwordLength) {
        newPassword.reserve(passwordLength);
    }

    // Make sure we clear out any previous password.
    newPassword.clear();

    // Do characters 2 onwards. It is assumed that the length of the passwords is bigger (or equal)
    // to the sum of all the parts required (upper, lower, etc). The developer is in charge!
    uint8_t extraCharactersRequired = passwordLength - numberOfUppers - numberOfLowers -
                                      numberOfDigits - numberOfSpecials - 1;

    for (uint8_t x = 1; x <= numberOfUppers; x++) {
        // Add upper case letters.
        newPassword.push_back(uppers[rand() % uppers.size()]);
    }

    for (uint8_t x = 1; x <= numberOfLowers; x++) {
        // Add lower case letters.
        newPassword.push_back(lowers[rand() % lowers.size()]);
    }

    for (uint8_t x = 1; x <= numberOfDigits; x++) {
        // Add digits.
        newPassword.push_back(numbers[rand() % numbers.size()]);
    }

    for (uint8_t x = 1; x <= numberOfSpecials; x++) {
        // Add special characters.
        newPassword.push_back(specials[rand() % specials.size()]);
    }

    // And now, the remaining characters.
    for (uint8_t x = 0; x < extraCharactersRequired; x++) {
        // Generate a random source for the next character.
        uint8_t source = rand() % 10;

        switch (source) {
            case 0: case 3: case 5: case 7: case 9: newPassword.push_back(letters[rand() % letters.size()]); break;
            case 1: case 4: case 6: case 8: newPassword.push_back(numbers[rand() % numbers.size()]); break;
            case 2: newPassword.push_back(specials[rand() % specials.size()]); break;
        }
    }

    // Shuffle the data added so far.
    random_shuffle(newPassword.begin(), newPassword.end());

    // The first character must be a letter. This avoids occasional errors from Oracle.
    newPassword = letters[rand() % letters.size()] + newPassword;
}


int main(int argc, char *argv[])
{
    cout << "-- Created using 'Oracle Password Generator'" << endl << endl;

    string newPassword;

    // Randomise.
    srand(time(NULL));

    if (argc < 2) {
        // If no parameters passed, just generate a single password.
        genPassword(newPassword);
        cout << '"' << newPassword << '"' << endl;

    } else {
        // If parameters passed, assume they are usernames and generate a password and ALTER USER for each.
        for (++argv; *argv; ++argv) {
            genPassword(newPassword);
            cout << "alter user " << *argv << " identified by \"" << newPassword << "\" account unlock;" << endl;
        }
    }

    return 0;
}
