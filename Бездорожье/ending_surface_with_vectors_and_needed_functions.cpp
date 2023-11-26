#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

double gaussian2D(double x, double y, double x0, double y0, double sigma_x,
                  double sigma_y, double A) {
  double exponent = -((x - x0) * (x - x0) / (2 * sigma_x * sigma_x) +
                      (y - y0) * (y - y0) / (2 * sigma_y * sigma_y));
  return A * exp(exponent);
}
class Porder{//point order
  private:
    int i;
    int j;
  public:
    Porder() : i(0), j(0) {}
    Porder(int i, int j) : i(i), j(j) {}
    int geti() { return i; }
    int getj() { return j; }
    ~Porder(){}
};

class point {
private:
  double x;
  double y;
  double z;

public:
  point() : x(0), y(0), z(0) {}
  point(double x, double y, double z) : x(x), y(y), z(z) {}
  point(const point &other) : x(other.x), y(other.y), z(other.z) {}
  point operator+(const point &other) {
    return point(x + other.x, y + other.y, z + other.z);
  }

  point operator-(const point &other) {
    return point(x - other.x, y - other.y, z - other.z);
  }

  point operator/(double d) { return point(x / d, y / d, z / d); }

  point operator=(const point &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }
  point operator+=(const point &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  point operator-=(const point &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  bool operator==(const point &other){
    return  x==other.x && y==other.y && z==other.z;
  }
    
  void setz(double newZ) { z = newZ; }
  double getx() const { return x; }
  double gety() const { return y; }
  double getz() const { return z; }
};

class log {
private:
  double x1, y1, x2, y2;
  double R;

public:
  log() : x1(0), y1(0), x2(0), y2(0), R(0) {}
  log(double x1, double y1, double x2, double y2, double R) : x1(x1), y1(y1), x2(x2), y2(y2), R(R) {}
  void setx1(double newx1) { x1 = newx1; }
  void sety1(double newy1) { y1 = newy1; }
  void setx2(double newx2) { x2 = newx2; }
  void sety2(double newy2) { y2 = newy2; }
  void setR(double newR) { R = newR; }

double logCalc(point b) {
  double z = 0; 
  double A = (y2-y1), B = (x1-x2), C=0;
  //std::cout<< x1 << " " << x2 << " " << y1 << " " << y2 << " " << R << " " << A << " " << B <<"\n ";
  double d = (sqrt((A*b.getx()+B*b.gety()+C)*(A*b.getx()+B*b.gety()+C)))/(sqrt(A*A+B*B));
  //std::cout<< d <<"\n";
  //std::cout<<A*b.getx()+B*b.gety()+C << "  "<< A*(b.getx()-x2+x1)+B*b.gety()+C << "\n";
  if((((-B)*(b.getx()-x1)+A*b.gety()+C)>=0)&&(((-B)*(b.getx()-x2)+A*b.gety()+C)<=0)&&(d<=R)){
    z = sqrt(R*R - d*d);
    //cout<< z;
    return z;
  }
  return z;
}
};

void Logs(vector<class::log> &logs) {
  double x1 = 1, x2 = 2, y1 = 3, y2 = 4, R = 2;
  class::log buffer;
  buffer.setx1(x1);
  buffer.setx2(x2);
  buffer.sety1(y1);
  buffer.sety2(y2);
  buffer.setR(R);
  for (int i = 0; i < 10; i++) {
    logs.push_back(buffer);
    x1++;
    x2++;
    y1++;
    y2++;
    buffer.setx1(x1);
    buffer.setx2(x2);
    buffer.sety1(y1);
    buffer.sety2(y2);
    buffer.setR(R);
  }
}

class stone {
private:
  double x;
  double y;
  double R;

public:
  stone() : x(0), y(0), R(0) {}
  stone(double x, double y, double R) : x(x), y(y), R(R) {}
  void setx(double newx) { x = newx; }
  void sety(double newy) { y = newy; }
  void setR(double newR) { R = newR; }
  double stoneCalc(point b) {
    double z = 0;
    double distance = (b.getx() - x) * (b.getx() - x) + (b.gety() - y) * (b.gety() - y);
    if (R * R - distance >= 0) {
      z = sqrt(R * R - distance);
    } else {
      z = 0.0;
    }
    return z;
  }
};

void Stones(vector<stone> &stones) {
  double x = 3, y = 3, R = 2;
  stone buffer;
  buffer.setx(x);
  buffer.sety(y);
  buffer.setR(R);
  for (int i = 0; i < 10; i++) {
    stones.push_back(buffer);
    x++;
    y++;
    buffer.setx(x);
    buffer.sety(y);
    buffer.setR(R);
  }
}

class bell {
private:
  point Point;
  double dis[4];

public:
  bell(point NewPoint, double NewDis[4]) {
    Point = NewPoint;
    dis[0] = NewDis[0];
    dis[1] = NewDis[1];
    dis[2] = NewDis[2];
    dis[3] = NewDis[3];
  }

  double gauss(point b) {
    double x0 = Point.getx();
    double y0 = Point.gety();
    double sigma_x = dis[0];
    double sigma_y = dis[2];
    double A = Point.getz();

    return gaussian2D(b.getx(), b.gety(), x0, y0, sigma_x, sigma_y, A);
  }
};

class field {
friend class Rover;
friend class sensor;
protected:
  int N;
  double pixel;
  ofstream file;
  vector<bell> bells;
  vector<stone> stones;
  vector<class::log> logs;
  vector<vector<point>> Field;

public:
  field(){}

  Porder order_of_point(point A){
     point B = A/pixel;
     int i = (int)B.getx();
     int j = (int)B.gety();
     return Porder(i,j);
  } 

  double GetPixel(){
    return pixel;
  }

  void Setfield1(int NewN, double NewPixel){
     N = NewN;
     pixel = NewPixel;
  }
  
  void Resize(){
    Field.resize(N, vector<point>(N));
  }

  void Setfield2( vector<bell> Newbells, vector<stone> Newstones, vector<class::log> Newlogs)
  {
     bells = Newbells;
     stones = Newstones;
     logs = Newlogs;
  }

  void generate() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        Field[i][j] = point(i * pixel, j * pixel, 0);
      }
    }
  }

  void create_surface() {
    file.open("out.txt");
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         double h = 0;
                for (size_t k = 0; k < logs.size(); k++) {
                  h += logs[k].logCalc(Field[i][j]);
                }
                for (size_t k = 0; k < stones.size(); k++) {
                  h += stones[k].stoneCalc(Field[i][j]);
                }
                for (size_t k = 0; k < bells.size(); k++) {
                  h += bells[k].gauss(Field[i][j]);
                }
                file << Field[i][j].getx() << " " << Field[i][j].gety() << " " << h << endl;
                Field[i][j]+=point(0,0,h);
              }
              file << endl;
            }
            file.close();
          }

          ~field() {
            Field.clear();
            bells.clear();
            stones.clear();
            logs.clear();
          }
};

