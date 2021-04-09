#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <vector>

using namespace std;

class BaseList {

private:
  std::map<int,string> idMap;
  std::map<string,int> nameMap;
  int idIterator;

public:
  BaseList(){
    idIterator = 1;
  }
  int InsertOrGet(string name) {
    map<string,int>::iterator it = this->nameMap.find(name);
    if(it != this->nameMap.end()) {
      return this->nameMap[name];
    }
    
    this->idMap[idIterator] = name;
    this->nameMap[name] = idIterator;
    int retVal = idIterator;
    ++idIterator;
    return retVal;
  };
  
  
};

struct RoleItem{
    string name;
    int id;
    std::vector<std::pair<string,string>> Permissions;
};

BaseList CreateRoleList(){
  string line;
  ifstream infile("data.txt");
  BaseList retVal;
  int RoleItr = 0;
  while (std::getline(infile,line))
    {
      if (line.find("[role]") != std::string::npos) {
          RoleItr = 0;
      }else {
        if (RoleItr == 1) {
          retVal.InsertOrGet(line);
        }
      }
      ++RoleItr;
    }
  return retVal;
};

BaseList CreateResourceList() {
  string line;
  ifstream infile("data.txt");
  BaseList retVal;
  bool startRead = false;
  while (std::getline(infile,line))
    {
      if (line.find("[permissions]") != std::string::npos) {
           cout<<"Setting to true"<<endl;
          startRead = true;
      }else if (line == "") {
        cout<<"setting to false"<<endl;
        startRead = false;
      } else {
        if (startRead == true) {
          cout<<"inserting:"<<line<<endl;
          cout<<line.substr(0,line.find(":"))<<endl;
          retVal.InsertOrGet(line.substr(0,line.find(":")));
        }
      }
      
    }
    return retVal;
}

int main() {
  std::vector<RoleItem> RoleList;
  BaseList Roles = CreateRoleList();
  BaseList Resources = CreateResourceList();

  cout<<"hello world"<<endl;
  string line;
  ifstream infile("data.txt");
  // while (std::getline(infile,line))
  //   {
  //     std::istringstream iss(line);
  //     cout<<line<<endl;
  //   }
  return 0;
}
