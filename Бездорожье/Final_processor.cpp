#include <cmath>
#include <vector>
#include <limits>
#include <queue>
#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include "Rover.cpp"


using namespace std;

class Processor {
private:
    int N;
    int size;
    field *f;
    Rover *r;
    vector<vector<double>> graph;
    vector<double> distance;
    vector<int> path;

public:
    Processor(field *f, Rover *r) : f(f), r(r)
    {
        N = f->Field.size();
        size = N*N;
        graph = vector<vector<double>>(size, vector<double>(size, -1));
        distance = vector<double>(size, -1);
        path = vector<int>(size, -1); 
    }

//-----------------------------------------------------------------------------------------------------------------------------------------

//создаем методы которые представляют из себя биективные отображения из NхN->N и наоборот 
//по сути отображаем поле в граф
   int coder(int i, int j){
     int number = N*i + j;
     return number;
   }
   pair<size_t, size_t> decoder(int number){
    size_t j = number%N;
    size_t i = (number-j)/N;
    //cout << number << endl;
    return make_pair(i, j);
   }
   int Code_point(point p){
      int peak;
      Porder order_of_point = f->order_of_point(p);

      return peak = this->coder(order_of_point.geti(), order_of_point.getj());
   }
//считаем вес ребер используя стандартную евклидову метрику 
//есть недоработка, планируется непроходимым ребрам задать вес -1 
   double Weight(int first_peak, int second_peak){
      //if(first_peak == second_peak) exit;

      pair<size_t,size_t> f_cor, s_cor;
      double x1,x2,y1,y2,z1,z2;
      double dx = 0.0, dy = 0.0, dz = 0.0;
      f_cor=decoder(first_peak);
      s_cor=decoder(second_peak);
      //проблема(падает программа)
      /*string scence_of_first_peak = r->Try_Move_Rover_each_directions(f->Field[f_cor.first][f_cor.second]);
      string scence_of_second_peak = r->Try_Move_Rover_each_directions(f->Field[s_cor.first][s_cor.second]);
      if(scence_of_first_peak == "not hehe" || scence_of_second_peak == "not hehe") return -1;*/


      if(f_cor.first >= f->Field.size() || f_cor.second >= f->Field[0].size() ||
         s_cor.first >= f->Field.size() || s_cor.second >= f->Field[0].size())
      {
         throw out_of_range("Index out of range");
      }


      x1 = f->Field[f_cor.first][f_cor.second].getx();
      y1 = f->Field[f_cor.first][f_cor.second].gety();
      z1 = f->Field[f_cor.first][f_cor.second].getz();
      x2 = f->Field[s_cor.first][s_cor.second].getx();
      y2 = f->Field[s_cor.first][s_cor.second].gety();
      z2 = f->Field[s_cor.first][s_cor.second].getz();
      dx = x1 - x2;
      dy = y1 - y2;
      dz = z1 - z2;
      return sqrt((dx*dx)+(dy*dy)+(dz*dz));
   }

//-----------------------------------------------------------------------------------------------------------------------------------------------

//обработка исключений (недоделано)
   //bool isInside(size_t x, size_t y) {
     // return (x >= 0 && x < f->Field.size() && y >= 0 && y < f->Field[0].size());
   //}

   void addEdge(int first_peak, int second_peak) {
      //if(first_peak == second_peak) exit;

       graph[first_peak][second_peak] = this->Weight(first_peak, second_peak);
   }

//---------------------------------------------------------------------------------------------------------------------------------------------------------
   //проверяем вершины на соседство 
   bool try_neighbors(int first_peak, int second_peak){
       pair<int,int> f = this->decoder(first_peak), s = this->decoder(second_peak);
       if(abs(f.first - s.first)<= 1 && abs(f.second - s.second)<= 1) return true;
       return false;
   }
      
   //присваиваем вес ребрам 
   void Give_Graph_Weight(){
      for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(this->try_neighbors(i,j)) this->addEdge(i,j);
        }
      }
   }

//--------------------------------------------------------------------------------------------------------------------------------------------------------
  //выполняем стандартный алгоритм дейкстера для графов 
  //находим кратчайший путь для вершин графа
    vector<int> findShortestPath(int start, int end) {
        this->Give_Graph_Weight();//задаем граф(весами ребер)
        distance[start] = 0;
        std::priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> queue;
        queue.push({0, start});

        while (!queue.empty()) {
            double dist = queue.top().first;
            int u = queue.top().second;
            queue.pop();

            if (abs(distance[u] - dist)>eps) continue;

            for (int v = 0; v < size; v++) {
                if (abs(graph[u][v]+1)<eps) continue;
                
                double alt = distance[u] + graph[u][v];
                if (abs(distance[v] + 1)<eps || alt < distance[v]) {
                    distance[v] = alt;
                    path[v] = u;
                    queue.push({alt, v});
                }
            }
        }

        vector<int> shortestPath;
        int current = end;
        while (current != -1) {
            shortestPath.push_back(current);
            current = path[current];
        }
        reverse(shortestPath.begin(), shortestPath.end());

        return shortestPath;
    }
//----------------------------------------------------------------------------------------------------------------------------------------
//возвращаемся к полю отображая вершины графа обратно в точки поля 
    vector<point> Ways_doer(){
        //cout << A << endl;
        //cout << B << endl; 
        int start = Code_point(A);
        int end = Code_point(B);
        //cout << start << endl;
        //cout << end << endl;
        vector<int> graph_peaks = this->findShortestPath(start, end);

        vector<point> real_way;
        for(size_t k = 0 ; k < graph_peaks.size(); k++){
            pair<int,int> cordinate = this->decoder(graph_peaks[k]);
            //cout << cordinate.first << " " << cordinate.second << endl;
            real_way.push_back(f->Field[cordinate.first][cordinate.second]);
        }
        return real_way;
    }
};


int main(){
  ofstream file;
  file.open("way.txt");
  control c;
  boundery b(&c);
  b.correct();
  b.DoWork();
  Rover r(&c.f);
  Processor proc(&c.f, &r);



 try {
    vector<point> way=proc.Ways_doer();

    for(size_t i = 0 ; i < way.size() ; i++)
      file << way[i] << endl;

    file.close();
    cout<<"ok"<<endl;
 } catch (const out_of_range& oor) {
    cerr << "Out of Range error: " << oor.what() << '\n';
 }
  return 0;   
}
