#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace  std;

vector<string> comandList = {"Generate_Field", "Add_Stone", "Add_Hill", "Add_Log", "cursor", "GenHill_At_cursor", "GenStone_At_cursor"};

double gaussian2D(double x, double y, double x0, double y0, double sigma_x,
                  double sigma_y, double A) {
  double exponent = -((x - x0) * (x - x0) / (2 * sigma_x * sigma_x) +
                      (y - y0) * (y - y0) / (2 * sigma_y * sigma_y));
  return A * exp(exponent);
}


class point {
private:
  double x;
  double y;
  double z;

public:
  point() : x(0), y(0), z(0) {}
  point(double x, double y, double z) : x(x), y(y), z(z) {}
  point(const point &other){
    x=other.x;
    y=other.y;
    z=other.z;
  }
  point operator+(const point &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  point operator-(const point &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  point operator/(double d) { return point(x / d, y / d, z / d); }

  point operator=(const point &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }
  void setz(double newZ) { z = newZ; }
  double getx() const { return x; }
  double gety() const { return y; }
  double getz() const { return z; }
};


class log {
private:
    double x1, x2, y1, y2;
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
      double A = (y1-y2), B = (x2-x1), C=0;
      double d = (sqrt((A*b.getx()+B*b.gety()+C)*(A*b.getx()+B*b.gety()+C)))/(sqrt(A*A+B*B));
      std::cout<<A << " "<< B ;
      if(((A*b.getx()+B*b.gety()+C)>=0)&&((A*b.getx()+B*b.gety()+C+x2-x1)<=0)&&(d<=R)){
        return z = sqrt(R*R - d*d);
      }
      return z;
    }

};

void logs(vector<class::log> vector){
    double x1 = 1, x2 = 2, y1 = 3, y2 = 4, R = 2; 
    class::log buffer;
    buffer.setx1(x1);
    buffer.setx2(x2);
    buffer.sety1(y1);
    buffer.sety2(y2);
    buffer.setR(R);
    for(int i=0;i<10;i++){
        vector.push_back(buffer);
        x1++;x2++;y1++;y2++;
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
    if (R*R - distance  >= 0) {
        z = sqrt(R * R - distance );  
    } else {
        z = 0.0;  
    }
    return z;
}
};

void stones(vector<stone> vector){
    double x = 3, y = 3, R = 2; 
    stone buffer;
    buffer.setx(x);
    buffer.sety(y);
    buffer.setR(R);
    for(int i=0;i<10;i++){
        vector.push_back(buffer);
        x++;y++;
        buffer.setx(x);
        buffer.sety(y);
        buffer.setR(R);
    }
}

class bell {
private:
  point Point;
  double *dis;

public:
  bell() {
    Point = point(0, 0, 0);
    dis = new double[4];
  }

  void setPoint(const point &fpoint) { Point = fpoint; }

  void setDispersion(const double fdispersion[4]) {
    for (int i = 0; i < 4; i++) {
      dis[i] = fdispersion[i];
    }
  }

  point getPoint() const { return Point; }
  //метод который обращается к функции и считает высоту точки относительно
  //конкретной гауссианы
  double gauss(point b) {
    double x0 = Point.getx();
    double y0 = Point.gety();
    //здесь параметры среднеквадратичного отклонения очень удобно регулировать
    //для получения разных поверхностей дисперсия здесь задается явно однако в
    //целом понятно что ее можно задать внутри функции BasePoints сейчас это не
    //сделано опять же в пользу большей простоты задания через внутренние а не
    //консольные или файловые точки
    dis[0] = 5.0;
    dis[2] = 5.0;
    double sigma_x = dis[0];
    double sigma_y = dis[2];
    double A = Point.getz();

    return gaussian2D(b.getx(), b.gety(), x0, y0, sigma_x, sigma_y, A);
  }

  ~bell() { delete[] dis; }
};

class field{
   private:
      int N;
      double pixel;
   public:
      field() : N(0), pixel(0) {}
      field(int N, double pixel) : N(N), pixel(pixel) {}
      void generate(vector<vector<point>> Field){
          for(int i=0;i<N;i++){
             for(int j=0;j<N;j++){
                Field[i][j] = point(i*pixel, j*pixel, 0);
             }
          }
      }
};
 
class boundery{
    public:
       string line;
       fstream logC;
       //fstream comand;
       fstream config;

       boundery(){
          logC.open ("logC.txt");
          //comand.open ("comand.txt");
          config.open ("config.txt");
        }

        void correct(){
            vector<string> vector = {""};
            int j = 0;
            while(getline(config, line)){
                cout << line << endl;
                for(int i = 0; i < comandList.size(); i++){
                   if(line == comandList[i]){
                      vector[j]+=line;
                      vector.push_back("");
                      j++;
                      getline(config, line);
                      cout << line << endl;
                      vector[j]+=line;
                      vector.push_back("");
                      j++;
                   }
                }
            }
            if(vector.size()!=comandList.size()){
               logC << "comands is not correct" << endl;
            }
            else{
               for(int i = 0; i < vector.size(); i++){
                  logC << vector[i] << endl;  
               }
            }
        }
        ~boundery(){
           logC.close();
           config.close();
        }
};
class control{
    public:
       fstream logB;
       fstream logC;
       
       control(){
          logB.open ("logB.txt");
          logC.open ("logC.txt");
        }
         
        vector<string> convertFileToVector(){
            vector<string> vector;
            string line;
            while(getline(logC, line)){
                cout<< line << endl;
                vector.push_back(line);
            }
            return vector;
        } 
        void doWork(){
            vector<string> log = convertFileToVector();
            for(int j = 0; j < log.size(); j++){
                if(log[j] == "Generate_Field"){
                   field Field(100, 0.1);
                   vector<vector<point>> betaField(100*100);
                   Field.generate(betaField);
                   logB << "Field successfully generated" << endl;
                };
                if(log[j] == "Add_Stone"){
                   stone Stone(5, 5, 2);
                };
                if(log[j] == "Add_Hill"){
                
                };
                if(log[j] == "Add_Log"){
                
                };
                if(log[j] == "cursor"){
                
                };
                if(log[j] == "GenHill_At_cursor"){
                
                };
                if(log[j] == "GenStone_At_cursor"){
                
                }
            }
        }
        ~control(){
           logC.close();
           logB.close();
        }
    };
    int main(){
       boundery B;
       control C;
       B.correct();
       C.doWork();
    }

