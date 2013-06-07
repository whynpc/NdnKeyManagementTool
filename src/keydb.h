#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

#include <openssl/evp.h>
#include <openssl/pem.h>

#include <ccn/keystore.h>

using namespace std;

#ifndef KEYDB_H
#define KEYDB_H

/*
 *This class is strictly for accessing the tables within KeyDB.db database
 */
class KeyDB
{
public:
    KeyDB();
    bool connectDB();
    void disconnectDB ();

    //For the Table "pubKey"
    int insertKey(string keyName, string pubKey, string locator="");
    int updateLocator(string keyName, string locator);
    string getKey(string keyName);
    string getLocator(string keyName);
    int importKey(string fname, string &pubKey);
    EVP_PKEY* str2Key(string pubKey);
    int exportKey(string fname, string pubKey);

    //For the Table "user"
    enum scope{
        local,
        global
    };
    int addUser(string app, scope sc, string keyName, string user = "");

    //Joining statement
    //Returns An array of strings {keyName, pubKey, locator} via passed-in argument key
    int getKeyFromUser(string app, scope sc, string key[], string user = "");


private:
    bool dbOpen;
    sqlite3 *db;
    //const string DB = "KeyDB.db";
	const string DB;
};

#endif // KEYDB_H
