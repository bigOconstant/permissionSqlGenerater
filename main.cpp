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
    string tableName;
    std::map<int,string> idMap;
    std::map<string,int> nameMap;
    int idIterator;

  public:
    BaseList(string tn){
      this->tableName = tn;
      this->idIterator = 1;
    }
    BaseList(){
      this->tableName = "";
      this->idIterator = 1;
    }

    friend ostream &operator<<( ostream &output, const BaseList &D ) {
         for(int i = 1; i < D.idIterator; ++i){
           output<<"insert into "+D.tableName+ " (id,name) values ("<<i<<",'"<<D.GetById(i)<<"');\n";
         }
         
         return output;            
    };

    int InsertOrGet(string name) {
      if(name == "") {
        cout<<"empty"<<endl;
      } 
      map<string,int>::iterator it = this->nameMap.find(name);
      if(it != this->nameMap.end()) {
        auto retVal = this->nameMap[name];
        if (retVal == 0) {
          cout<<"\nRetVal 0"<<endl;
        }
        return retVal;
      }
      
      this->idMap[this->idIterator] = name;
      this->nameMap[name] = this->idIterator;
      auto retVal = this->idIterator;
      ++this->idIterator;
      if(retVal == 0) {
        cout<<"retVal is 0"<<endl;
      }
      return retVal;
    };
    int Get(string name) {
      return this->nameMap.at(name);
    }
    string GetById( int id) const{
      return idMap.at(id);
    }
};

struct Permission {
  int id;
  int roleId;
  int ResourceId;
  string value;//read,write,admin
};

class PermissionList {
  public:
    PermissionList(BaseList Roles,BaseList Resources){
      this->Roles = Roles;
      this->Resources = Resources;
    };

    void ParseFiles(){
      Permission *p = new Permission();
      int currentRole = 0;
      int currentPermission = 0;
      ifstream infile("data.txt");
      string line;
      int currentRoleId = 0;
      while (std::getline(infile,line))
      {
        if (line.find("[role]") != std::string::npos) {
          currentRole = 1;
          continue;
        }
        if (line.find("[permissions]") != std::string::npos) {
           currentPermission = 1;
           continue;
        }
        if (line == "") {
          int currentRole = 0;
          int currentPermission = 0;
          continue;
        }

        if(currentRole == 1) {
          ++currentRole;
          currentRoleId = this->Roles.Get(line);
          if(currentRoleId == 0) {
            cout<<"error"<<endl;;
            
          }
          continue;
        }

        if(currentPermission > 0 && line != "" ) {
          p->roleId = currentRoleId;
          p->ResourceId = this->Resources.Get(line.substr(0,line.find(":")));
          p->value = line.substr(line.find(":")+1,line.size()-1);
          p->id = this->Plist.size() +1;
          this->Plist.push_back(p);
          p = new Permission();
        }

        ++currentRole;
        if(currentRole != 1){
          ++currentPermission;
        }
      }

    }
    friend ostream &operator<<( ostream &output, const PermissionList &PL ) {
         int id = 1;
        for (auto itr = PL.Plist.begin(); itr< PL.Plist.end(); ++itr){
           output<<"insert into permissions (id,role_id,resource_id,value)";
           output <<" values ("<<id<<","<<(*itr)->roleId<<","<<(*itr)->ResourceId<<",'"<<(*itr)->value<<"');\n";
           ++id;
        }
         
         return output;            
    };
  
  private:
    vector<Permission *> Plist;
    BaseList Roles;
    BaseList Resources;  
};

 struct RoleItem{
    string name;
    int id;
    std::vector<std::pair<string,string>> Permissions;
};

BaseList CreateRoleList(){
  string line;
  ifstream infile("data.txt");
  BaseList retVal = BaseList("roles");
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
  BaseList retVal = BaseList("resources");
  bool startRead = false;
  while (std::getline(infile,line))
    {
      if (line.find("[permissions]") != std::string::npos) {
          startRead = true;
      }else if (line == "") {
        startRead = false;
      } else {
        if (startRead == true) {
          retVal.InsertOrGet(line.substr(0,line.find(":")));
        }
      }
      
    }
    return retVal;
}

int main() {

  std::vector<RoleItem> RoleList;
  BaseList Roles = CreateRoleList();
  cout<<Roles<<endl;
  
  BaseList Resources = CreateResourceList();
  cout<<Resources<<endl;
  PermissionList *P = new PermissionList(Roles,Resources);
  P->ParseFiles();
  cout<<*P<<endl;
  return 0;
}
