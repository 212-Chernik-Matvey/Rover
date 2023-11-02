#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "vector_of_separeted_comands_and_parametrs.cpp"
#include "ending_surface_with_vectors_and_needed_functions.cpp" 

using namespace  std;

vector<string> comandList = {"Generate_Field", "Add_Stone", "Add_Hill", "Add_Log"};

vector<size_t> para = {2, 3, 6, 5};
class control{
    public:
       ofstream logB;
       vector<class::log> logs;
       vector<stone> stones;
       vector<bell> bells;
       field f;


      control(){
         logB.open ("logB.txt");
      } 

      void Generate_Field(int N, double pixel){
         logB << "Field start generation" << endl;
         logB << "\n";
         f.Setfield1(N, pixel);
         f.Resize();
         f.generate();
         logB << "Field generated"<< endl;
         logB << "\n";
      }   

      void Add_Stone(double x, double y, double R){
         logB<<"Adding stone..."<<endl;
         logB<<"\n";
         stone buffer(x, y, R);
         stones.push_back(buffer);
         logB<<"stone added"<<endl;
         logB<<"\n";
      }

      void Add_Hill(point Point, double Dis[4]){
         logB<<"Adding hill..."<<endl;
         logB<<"\n";
         bell buffer(Point, Dis);
         bells.push_back(buffer);
         logB<<"hill added"<<endl;
         logB<<"\n";
      }

      void Add_Log(double x1, double y1, double x2, double y2, double R){
         logB<<"Adding log..."<<endl;
         logB<<"\n";
         class::log buffer(x1, y1, x2, y2, R);
         logs.push_back(buffer);
         logB<<"log added"<<endl;
         logB<<"\n";
      }

      void SurfaceDoer(){
         f.Setfield2(bells, stones, logs);
         f.create_surface();
      }

       ~control(){
         logB.close();
      }
};

class boundery{
    public:
       ofstream logC;
       ifstream comand;
       control* c;
       vector<parametrs> v;

      boundery(control* controlPtr) : c(controlPtr) {
        logC.open("logC.txt");
        comand.open("comand.txt");
        v = separation(comand);
      }

      void correct(){
         size_t count = 0;
         size_t i;
         for(i = 0 ; i < v.size(); i++){
            for(size_t j = 0; j < comandList.size(); j++){
               if(v[i].comand!=comandList[j]) count++;
               if((v[i].comand==comandList[j])&&(v[i].param.size()!=para[j])){
                  logC<< "Error: value of parametrs is not correct"<< endl;
                  logC<< "\n";
                  v.erase(v.begin() + i);
                  i--;
               }
            }
            if(count == comandList.size()){
               logC << "Error: used unvalible command" << endl;
               logC << "\n";
               v.erase(v.begin() + i);
               i--;
            }
            count = 0; 
         }
      }

      void DoWork(){
         if(v.size() == 0){
            logC<<"not valible commands"<<endl;
            logC<<"\n";
         }
         else{
            for(size_t i = 0; i < v.size(); i++){
               if(v[i].comand == "Generate_Field") c->Generate_Field(v[i].param[0], v[i].param[1]);
               if(v[i].comand == "Add_Stone") c->Add_Stone(v[i].param[0], v[i].param[1], v[i].param[2]);
               if(v[i].comand == "Add_Log") c->Add_Log(v[i].param[0], v[i].param[1] ,v[i].param[2], v[i].param[3], v[i].param[4]);
               if(v[i].comand == "Add_Hill"){
                  point Point(v[i].param[0], v[i].param[1], v[i].param[2]);
                  double Dis[4] = {v[i].param[3],v[i].param[5], v[i].param[4], v[i].param[5]};
                  c->Add_Hill(Point, Dis);
               }
            }
            c->SurfaceDoer();
         }
      }

      ~boundery(){
           logC.close();
           comand.close();
           v.clear();
      }
};

    int main(){
      control c;
      boundery b(&c);
      b.correct();
      b.DoWork();
      cout<<"ok"<<endl;
      return 0;   
   }

