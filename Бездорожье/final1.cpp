
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
//координаты высоты точки
//при желании в эту формулу можно добавить два других параметра однако я
//ограничился такой реализацией в пользу большей простоты
double gaussian2D(double x, double y, double x0, double y0, double sigma_x,
                  double sigma_y, double A) {
  double exponent = -((x - x0) * (x - x0) / (2 * sigma_x * sigma_x) +
                      (y - y0) * (y - y0) / (2 * sigma_y * sigma_y));
  return A * exp(exponent);
}
//реализация класса точки, многие методы и перегрузки операций были созданы мной
//здесь и не только прозапас
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
    double foflog(point b) {
      double z = 0; 
      double A = (y2-y1), B = (x1-x2), C=0;
      //std::cout<< x1 << " " << x2 << " " << y1 << " " << y2 << " " << R << " " << A << " " << B <<"\n ";
      double d = (sqrt((A*b.getx()+B*b.gety()+C)*(A*b.getx()+B*b.gety()+C)))/(sqrt(A*A+B*B));
      //std::cout<< d <<"\n";
      //std::cout<<A*b.getx()+B*b.gety()+C << "  "<< A*(b.getx()-x2+x1)+B*b.gety()+C << "\n";
      if((((-B)*(b.getx()-x1)+A*b.gety()+C)>=0)&&(((-B)*(b.getx()-x2)+A*b.gety()+C)<=0)&&(d<=R)){
        return z = sqrt(R*R - d*d);
      }
      return z;
    }
};

void Logs(class log *matrix, int N, int M) {
    double x1=5, x2=40, y1=5, y2=40, R=5; 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i * M + j].setx1(x1);
            matrix[i * M + j].sety1(y1);
            matrix[i * M + j].setx2(x2);
            matrix[i * M + j].sety2(y2);
            matrix[i * M + j].setR(R);
        }
        x1+=1;
        x2+=2;
        y1+=3;
        y2+=4;
        R=5;
    }
}
void printLog(std::ofstream &file, point *Matrix, class log *Array, int N, int M, int K, int L) {
  double h =0 ;
  for (int k = 0; k < K; k++) {
    for (int l = 0; l < L; l++) {
      for (int n = 0; n < N; n++) {
        for (int m = 0; m < M; m++) {
          h = Array[k * L + l].foflog(Matrix[n * M + m]);
          //if(h!=0){
             file << (Matrix[n * M + m]).getx() << "    " << Matrix[n * M + m].gety() << "     " << h << "\n";
          //}
          
          h = 0;
        }
        file << "\n";
      }
    }
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
double fofstone(point b) {
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
void Stones(stone *matrix, int N, int M) {
  double x = 25, y = 25, R = 20;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      matrix[i * M + j].setx(x);
      matrix[i * M + j].sety(y);
      matrix[i * M + j].setR(R);
    }
    x+=12.5;
    y+=12.5;
  }
}
void printStone(std::ofstream &file, point *Matrix, stone *Array, int N, int M, int K, int L) {
  double h =0 ;
  for (int k = 0; k < K; k++) {
    for (int l = 0; l < L; l++) {
      for (int n = 0; n < N; n++) {
        for (int m = 0; m < M; m++) {
          h = Array[k * L + l].fofstone(Matrix[n * M + m]);
          if(h!=0)
             file << (Matrix[n * M + m]).getx() << "    " << Matrix[n * M + m].gety() << "     " << h << "\n";
          
          
          h = 0;
        }
        file << "\n";
      }
    }
  }
}












//класс реализации объекта хранящего информацию о гауссианах
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
//функция которая пока работает внутри программы однако видно что ее с легкостью
//можно переделать как на чтение с консоли так и на чтение из файла
void field(point *matrix, int N, int M) {
  double z1 = 0, z2 = 0, z3 = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      matrix[i * M + j] = point(z1, z2, z3);
      z2 += 0.5; //можно регулировать качество изображения меняя масштаб сетки
    }
    z1 += 0.5; //можно регулировать качество изображения меняя масштаб сетки
    z2 = 0;
  }
}
//аналогичная предыдущей функция только для чтения массива хранящего информацию
//о гауссианах
void BasePoints(bell *matrix, int N, int M) {
  double z1 = 0, z2 = 0, z3 = 0;
  // double fdispersion[4];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      z3 = rand() / double(RAND_MAX) * 10 - 5;
      matrix[i * M + j].setPoint(point(z1, z2, z3));
      // fdispersion[0] = random() / double(RAND_MAX) * 1.9 + 0.1;
      // fdispersion[2] = random() / double(RAND_MAX) * 1.9 + 0.1;
      // fdispersion[1] = 0.0;
      // fdispersion[3] = 0.0;
      // matrix[i * M + j].setDispersion(fdispersion);
      z2 += 5; //можно регулировать координаты вершин гауссиан
    }
    z1 += 5; //можно регулировать координаты вершин гауссиан
    z2 = 0;
  }
}
//функция которая создает файл с точками для gnuplot
void print(std::ofstream &file, point *Matrix, bell *Array, int N, int M, int K, int L) {
  for (int n = 0; n < N; n++) {
    for (int m = 0; m < M; m++) {
      double h = 0;
      for (int k = 0; k < K; k++) {
        for (int l = 0; l < L; l++) {
          h += Array[k * L + l].gauss(Matrix[n * M + m]);
        }
      }
      file << (Matrix[n * M + m]).getx() << " " << Matrix[n * M + m].gety()
           << " " << h << "\n";
    }
    file << "\n";
  }
}

int main() {
  srand(time(NULL));
  int K = 10, L = 10, N = 100, M = 100, count1ofStones = 1, count2ofStones = 1, count1ofLogs = 1, count2ofLogs = 1; //удобно регулировать площадь поверхности
  point *data = new point[N * M];
  bell *guss = new bell[K * L];
  stone *stones = new stone[count1ofStones * count2ofStones];
  class log *logs = new class log[count1ofLogs * count2ofLogs];
  field(data, N, M);
  BasePoints(guss, K, L);
  Stones(stones, count1ofStones, count2ofStones);
  Logs(logs, count1ofLogs, count2ofLogs);
  std::ofstream file("bruh.txt");
  std::ofstream stone("stones.txt");
  std::ofstream log("logs.txt");
  print(file, data, guss, N, M, K, L);
  printStone(stone, data, stones, N, M, count1ofStones, count2ofStones);
  printLog(log, data, logs, N, M, count1ofStones, count2ofStones);
  file.close();
  stone.close();
  log.close();
  delete[] data;
  delete[] guss;
  delete[] stones;
  delete[] logs;
  std::cout << "good job, bro";
  return 0;
}
