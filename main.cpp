#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

class BaseList {

private:
  std::map<int,string> idMap;
  std::map<string,int> nameMap;
  int idIterator;

public:
  RoleList(){
    idIterator = 1;
  }
  int InsertOrGet(string name) {
    map<string,int>::iterator it = this->nameMap.find(name);
    if(it != this->nameMap.end()) {
      return this->nameMap[name];
    }
    
    this->idMap[idIterator] = name;
    this->nameMap[name] = idIterator;
    ++idIterator;
  };
  
  
};


int main() {
  cout<<"hello world"<<endl;
  string line;
  ifstream infile("data.txt");
  while (std::getline(infile,line))
    {
      std::istringstream iss(line);
      cout<<line<<endl;
    }
  return 0;
}
