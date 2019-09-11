#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#ifdef __BORLANDC__
    // I'm not 100% convinced about this, but Embarcadero said it was fine, so ....
    // But the Borland version of <cstdlib> defined srand() and rand()
    // however, compiling fails unless I include this file. Sigh.
    #include <stdlib.h>
#endif

using std::cout;
using std::endl;
using std::string;
using std::begin;
using std::end;

const int8_t passwordLength = 15;


void genPassword(string &newPassword) {
    static const string letters = "abcdefghijkmnoprstuvwxyzABCDEFGHIJKLMNPQRSTUVWXYZ";
    static const string numbers = "2345678923456789234567892345678923456789234567892";
    static const string specials = "$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#$_#";

    // Save effort by pre-filling the string to a big enough length.
    if (newPassword.capacity() < 20) {
        newPassword.reserve(20);
    }

    // Make sure we clear out any previous password.
    newPassword.clear();

    // The first character must be a letter. This avoids occasional errors from Oracle.
    newPassword.push_back(letters[rand() % letters.size()]);

    // And now, the remaining characters.
    for (int8_t x = 1; x < passwordLength; x++) {
        // Generate a random source for the next letter.
        int8_t source = rand() % 5;

        switch (source) {
            case 0: case 4: newPassword.push_back(letters[rand() % letters.size()]); break;
            case 1: case 3: newPassword.push_back(numbers[rand() % numbers.size()]); break;
            case 2: newPassword.push_back(specials[rand() % specials.size()]); break;
        }
    }
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
