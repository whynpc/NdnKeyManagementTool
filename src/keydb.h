#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

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
    static KeyDB& instance()
    {
        static KeyDB _instance;
        return _instance;
    }
    KeyDB() {   dbOpen = false;
    	 DB = "../db/KeyDB.db";
        connectDB();}
    KeyDB(KeyDB const&);
    void operator=(KeyDB const&);
    
//    KeyDB();
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
    int getAllKeyNames(vector<string> &keyNames);

    //For the Table "priKey"
    int removePriKey();
    int insertPriKey(string keyName, string priKey);
    int importPriKey(string fname, string &priKey);
    string getPriKey();
    string getPriKeyName();
    EVP_PKEY* str2priKey(string priKey);

    //For the Table "user"
    enum scope{
        local = 0,
        global
    };
    int addUser(string app, scope sc, string keyName, string user = "");
    int getMetaData(string keyName, vector<string> &mData);
    //Joining statement
    //Returns An array of strings {keyName, pubKey, locator} via passed-in argument key
    int getKeyFromUser(string app, scope sc, string key[], string user = "");



private:
    bool dbOpen;
    sqlite3 *db;
    string DB;
 //   const string DB = "KeyDB.db";
};

#endif // KEYDB_H
