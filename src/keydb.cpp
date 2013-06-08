#include "keydb.h"

/*KeyDB::KeyDB()
{
    dbOpen = false;
}*/

bool KeyDB::connectDB(){
    if (sqlite3_open(DB.c_str(), &db) == SQLITE_OK){
        dbOpen = true;
        return true;
    }

    return false;
}

void KeyDB::disconnectDB ()
{
    if (dbOpen == true){
        sqlite3_close(db);
        dbOpen = false;
    }
}

//Inserts the name and public key into the pubKey table
int KeyDB::insertKey(string keyName, string pubKey, string locator){
    string statement = "";
    if (locator == "")
        statement = "INSERT INTO pubKey values (\"" + keyName + "\", \"" + pubKey + "\", NULL);";
    else
        statement = "INSERT INTO pubKey values (\"" + keyName + "\", \"" + pubKey + "\", \"" + locator + "\");";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        sqlite3_finalize(sqlStmt);
        return status;
    }
    cerr << sqlite3_errstr(errno) << endl;
    return errno;
}

//Updates the key with keyName with new keyLocator locator
int KeyDB::updateLocator(string keyName, string locator){
    string statement = "UPDATE pubKey SET locator = \"" + locator + "\" WHERE keyName = \"" + keyName + "\";";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        sqlite3_finalize(sqlStmt);
        return status;
    }
    cerr << sqlite3_errstr(errno) << endl;
    return errno;
}

//Get the public key from its name
string KeyDB::getKey(string keyName){
    string statement = "SELECT pubKey FROM pubKey WHERE keyName = \"" + keyName + "\";";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    string key;
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        if (status == SQLITE_ROW)
            key = string((const char*)sqlite3_column_text (sqlStmt, 0));
        sqlite3_finalize(sqlStmt);;
        return key;
    }
    return sqlite3_errstr(errno);
}

//Get the public key's keyLocator from the keyName
string KeyDB::getLocator(string keyName){
    string statement = "SELECT locator FROM pubKey WHERE keyName = \"" + keyName + "\";";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    string locator = "";
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        if (status == SQLITE_ROW && sqlite3_column_text (sqlStmt, 0))
            locator = string((const char*)sqlite3_column_text (sqlStmt, 0));
        sqlite3_finalize(sqlStmt);
        return locator;
    }
    return sqlite3_errstr(errno);
}

//Imports public key from file fname into the string pubKey
int KeyDB::importKey(string fname, string &pubKey){
    ifstream keyFile(fname.c_str());

    if (!keyFile) {
        cerr << "Unable to open file " << fname << endl;
        exit(1);
    }

    for (string buf; getline(keyFile, buf);)
        pubKey += buf + "\n";

    return 0;
}

//converts a key string into an EVP_PKEY object
EVP_PKEY* KeyDB::str2Key(string pubKey){
    char* buf = (char*)malloc(pubKey.size());
    strcpy(buf, pubKey.c_str());
    BIO* bp = BIO_new_mem_buf(buf, pubKey.length());
    EVP_PKEY* kObj = PEM_read_bio_PUBKEY(bp, NULL, NULL, NULL);
    BIO_free(bp);
    free(buf);
    return kObj;
}

//Exports a key string into a public key file
int KeyDB::exportKey(string fname, string pubKey){
    EVP_PKEY* kObj = str2Key(pubKey);
    BIO* bp = BIO_new_file(fname.c_str(), "w");
    if (!bp){
        cerr << "Could not open file " << fname << "for writing" << endl;
        exit(1);
    }

    PEM_write_bio_PUBKEY(bp, kObj);
    BIO_free(bp);
    delete(kObj);
    return 0;
}

int KeyDB::getAllKeyNames(vector<string> &keyNames){
    string statement;
    statement = "SELECT keyName FROM pubKey;";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        while(1){
            int status = sqlite3_step(sqlStmt);
            if (status == SQLITE_ROW)
                keyNames.push_back((const char *)sqlite3_column_text (sqlStmt, 0));
            else if (status == SQLITE_DONE)
                break;
            else
                return status;

        }
        sqlite3_finalize(sqlStmt);
        return 0;
    }
    return errno;
}


int KeyDB::removePriKey(){
    string statement = "DELETE FROM priKey;";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        sqlite3_finalize(sqlStmt);
        return status;
    }
    cerr << sqlite3_errstr(errno) << endl;
    return errno;
}

int KeyDB::insertPriKey(string keyName, string priKey){
    removePriKey(); //there can be only one private key
    string statement = "INSERT INTO priKey values (\"" + keyName + "\", \"" + priKey + "\");";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        sqlite3_finalize(sqlStmt);
        return status;
    }
    cerr << sqlite3_errstr(errno) << endl;
    return errno;
}

int KeyDB::importPriKey(string fname, string &priKey){
    ifstream keyFile(fname.c_str());

    if (!keyFile) {
        cerr << "Unable to open file " << fname << endl;
        exit(1);
    }

    for (string buf; getline(keyFile, buf);)
        priKey += buf + "\n";

    return 0;
}

string KeyDB::getPriKey(){
    string statement = "SELECT priKey FROM priKey;"; //there is only one private key
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    string key;
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        if (status == SQLITE_ROW)
            key = string((const char*)sqlite3_column_text (sqlStmt, 0));
        sqlite3_finalize(sqlStmt);;
        return key;
    }
    return sqlite3_errstr(errno);
}

string KeyDB::getPriKeyName(){
    string statement = "SELECT keyName FROM priKey;";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    string name;
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        if (status == SQLITE_ROW)
            name = string((const char*)sqlite3_column_text (sqlStmt, 0));
        sqlite3_finalize(sqlStmt);;
        return name;
    }
    return sqlite3_errstr(errno);
}

EVP_PKEY* KeyDB::str2priKey(string priKey){
    char* buf = (char*)malloc(priKey.size());
    strcpy(buf, priKey.c_str());
    BIO* bp = BIO_new_mem_buf(buf, priKey.length());
    EVP_PKEY* kObj = PEM_read_bio_PrivateKey(bp, NULL, NULL, NULL);
    BIO_free(bp);
    free(buf);
    return kObj;
}


//Adds an row into the "user" table
int KeyDB::addUser(string app, scope sc, string keyName, string user){
    string statement = "";
    char strSc[2];
    sprintf(strSc, "%i", sc);
    if (user == "")
        statement = "INSERT INTO user values (NULL, \"" + app + "\", " + strSc + ", \"" + keyName + "\");";
    else
        statement = "INSERT INTO user values (\"" + user + "\", \"" + app + "\", " + strSc + ", \"" + keyName + "\");";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        sqlite3_finalize(sqlStmt);
        return status;
    }
    cerr << sqlite3_errstr(errno) << endl;
    return errno;
}

//returns metadata in the form of "userName, appname, scope"
int KeyDB::getMetaData(string keyName, vector<string> &mData){
    string statement = "SELECT * FROM user WHERE keyName = \"" + keyName + "\";";
    sqlite3_stmt *sqlStmt;
    string metaData = "";
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        while (1){
        int status = sqlite3_step(sqlStmt);
            if (status == SQLITE_ROW){
                if (sqlite3_column_text (sqlStmt, 0)){
                    metaData = metaData + "User Name: " +
                               (const char *)sqlite3_column_text (sqlStmt, 0) + "| ";
                }
                metaData = metaData + "App Name: " + (const char *)sqlite3_column_text (sqlStmt, 1);
                metaData = metaData + "| Scope: " + (const char *)sqlite3_column_text (sqlStmt, 2);
                mData.push_back(metaData);
                metaData = "";
            }else{
                sqlite3_finalize(sqlStmt);;
                return status;
            }
        }
    }
    return errno;
}

//Retrieves the public key information using the user Table
//argument string key[] is the return value, in the form of {keyName, pubKey, locator}
int KeyDB::getKeyFromUser(string app, scope sc, string key[], string name){
    string statement;
    char strSc[2];
    sprintf(strSc, "%i", sc);
    if (name == "")
        statement = "SELECT * FROM pubKey, user WHERE pubKey.keyName = user.keyName AND \"" +
                app + "\" = user.appName AND " + strSc + " = scope;";
    else
        statement = "SELECT * FROM pubKey, user WHERE pubKey.keyName = user.keyName AND \"" +
                app + "\" = user.appName AND " + strSc + " = scope; AND \"" + name + "\" = uName;";
    sqlite3_stmt *sqlStmt;
    int errno = sqlite3_prepare(db, statement.c_str(), statement.size(), &sqlStmt, 0);
    if (errno == SQLITE_OK){
        int status = sqlite3_step(sqlStmt);
        if (status == SQLITE_ROW){
            key[0] = (const char *)sqlite3_column_text (sqlStmt, 0);
            key[1] = (const char *)sqlite3_column_text (sqlStmt, 1);
            key[2] = (const char *)sqlite3_column_text (sqlStmt, 2);
        }
        sqlite3_finalize(sqlStmt);;
        return status;
    }
    return errno;
}
