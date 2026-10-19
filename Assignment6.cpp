#include <iostream>
#include <vector>
#include <list>
using namespace std;
const string PEPPER = "pepper07";
struct User{
    string username;
    string salt;
    uint64_t password;
};
uint64_t demoHash(const std :: string & s){
    uint64_t h = 1469598103934665603ULL;
    for(unsigned char c : s){
        h = (h ^ c) * 1099511628211ULL;
    }
    return h;
}
class HashTable{
    private:
    vector<list<User>> table;
    size_t numElements;
    size_t capacity;
    const double MAX_LOAD_FACTOR = 0.75;
    size_t getIndex(const string & key){
        return demoHash(key) % capacity;
    }
    void rehash(){
        size_t oldCapacity = capacity;
        capacity *= 2;
        vector<list<User>> newTable(capacity);
        for(size_t i = 0; i < oldCapacity; ++i){
            for(const auto & record: table[i]){
                size_t newIndex = demoHash(record.username) % capacity;
                newTable[newIndex].push_back(record);
            }
        }
        table = std::move(newTable);
    }
    public:
    HashTable(size_t initialCapacity = 4): numElements(0), capacity(initialCapacity){
        table.resize(capacity);
    }
    const User * find(const string & username){
        size_t index = getIndex(username);
        for(const auto & record : table[index]){
            if(record.username == username){
                return & record;
            }
        }
        return nullptr;
    }
    bool insert(const string & username, const string & salt, uint64_t hashValue){
        if(find(username) != nullptr){
            return false;
        }
        if(loadFactor() >= MAX_LOAD_FACTOR){
            rehash();
        }
        size_t index = getIndex(username);
        User newUser = {username, salt, hashValue};
        table[index].push_back(newUser);
        numElements++;
        return true;

    }
    bool remove(const string & username){
        size_t index = getIndex(username);
        for(auto it = table[index].begin(); it != table[index].end(); ++it){
            if(it -> username == username){
                table[index].erase(it);
                numElements--;
                return true;
            }
        }
        return false;
    }
    double loadFactor() {
        return static_cast<double> (numElements) / capacity;

    }
    void displayStats(){
        cout << "Current Elements: " <<numElements << ", Capacity: " << capacity << ", Load Factor: " << loadFactor() << endl;
    }
    bool verifyLogin(const string & username, const string & submittedPassword){
        const User * user = find(username);
        if(user == nullptr){
            cout << "Login Failed: User " << username << " was not found \n";
            return false;
        }
        uint64_t computedHash = demoHash(user -> salt + submittedPassword + PEPPER);
        if(computedHash == user-> password){
            cout << "Login was Succesful: Welcome "  << username << "\n";
            return true;
        }
        else{
            cout<< "Login failed: That was the wrong passwword for the user " << username <<"\n";
            return false;
        }
    }

};

    int main() {
        HashTable userTable(4);
        userTable.insert("george22", "salt1", demoHash("salt1" + string("welcome456") + PEPPER));
        userTable.insert("vianca_r", "salt2", demoHash("salt2" + string("correct789") + PEPPER));
        userTable.insert("coco04", "salt3", demoHash("salt3" + string("safe356") + PEPPER));
        userTable.displayStats();

        userTable.insert("tay_p", "salt4", demoHash("salt4" + string("passcode") + PEPPER));
        userTable.insert("cindy_f", "salt5", demoHash("salt5" + string("entry777") + PEPPER));
        userTable.displayStats();

        userTable.verifyLogin("vianca_r", "correct789");
        userTable.verifyLogin("george22", "wrongpass");
        userTable.verifyLogin("six_455", "uknownpass");

        cout << "Removing user coco04\n";
        userTable.remove("coco04");

        userTable.verifyLogin("coco04", "safe365");


        return 0;
    }


