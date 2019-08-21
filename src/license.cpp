#include "main.hpp"

using namespace std;

string encrypt(string toEncrypt) {
    char key = 'K'; //Any char will work
    string output2 = toEncrypt;

    for (int i = 0; i < toEncrypt.size(); i++)
        output2[i] = toEncrypt[i] ^ key;

    ofstream myfile;
    myfile.open("prasimax.lic");
    myfile<<output2;
    myfile.close();

    return output2;
}

string decrypt() {
    char key = 'K'; //Any char will work
    string line;
    ifstream myfile ("prasimax.lic");
    /*anjing */
    if (myfile.is_open())
      {
        while ( getline (myfile,line) )
        {
          cout << line << '\n';
        }
        myfile.close();
      }


    string output2 = line;

    for (int i = 0; i < line.size(); i++)
        output2[i] = line[i] ^ key;

    /*bajigur*/

    return output2;
}

string license()
{
    string encrypted = encrypt("freeprasimax");
    cout << "Encrypted:" << encrypted << "\n";

    string decrypted = decrypt();
    cout << "Decrypted:" << decrypted << "\n";

    return decrypted;
}
