//вариант на отправку 1
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
//координаты высоты точки 
//при желании в эту формулу можно добавить два других параметра однако я ограничился такой реализацией в пользу большей простоты
double gaussian2D(double x, double y, double x0, double y0, double sigma_x, double sigma_y, double A) {
    double exponent = -((x - x0) * (x - x0) / (2 * sigma_x * sigma_x) + (y - y0) * (y - y0) / (2 * sigma_y * sigma_y));
    return A * exp(exponent);
}
//реализация класса точки, многие методы и перегрузки операций были созданы мной здесь и не только прозапас
class point {
private:
    double x;
    double y;
    double z;

public:
    point() : x(0), y(0), z(0) {}
    point(double x, double y, double z) : x(x), y(y), z(z) {}

    point operator+(const point& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    point operator-(const point& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    point operator/(double d) {
        return point(x / d, y / d, z / d);
    }

    point operator=(const point& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    double getx() const { return x; }
    double gety() const { return y; }
    double getz() const { return z; }
};
//класс реализации объекта хранящего информацию о гауссианах 
class bell {
private:
    point Point;
    double* dis;

public:
    bell() {
        Point = point(0, 0, 0);
        dis = new double[4];
    }

    void setPoint(const point& fpoint) {
        Point = fpoint;
    }

    void setDispersion(const double fdispersion[4]) {
        for (int i = 0; i < 4; i++) {
            dis[i] = fdispersion[i];
        }
    }

    point getPoint() const { return Point; }
//метод который обращается к функции и считает высоту точки относительно конкретной гауссианы
double gauss(point b) {
    double x0 = Point.getx();
    double y0 = Point.gety();
    //здесь параметры среднеквадратичного отклонения очень удобно регулировать для получения разных поверхностей
    //дисперсия здесь задается явно однако в целом понятно что ее можно задать внутри функции BasePoints сейчас это не сделано опять же в пользу большей простоты задания через внутренние а не консольные или файловые точки
    dis[0] = 5.0;
    dis[2] = 5.0;
    double sigma_x = dis[0];
    double sigma_y = dis[2];
    double A = Point.getz(); 

    return gaussian2D(b.getx(), b.gety(), x0, y0, sigma_x, sigma_y, A);
}

    ~bell() {
        delete[] dis;
    }
};
//функция которая пока работает внутри программы однако видно что ее с легкостью можно переделать как на чтение с консоли так и на чтение из файла
void field(point* matrix, int N, int M) {
    double z1 = 0, z2 = 0, z3 = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i * M + j] = point(z1, z2, z3);
            z2 += 0.5;//можно регулировать качество изображения меняя масштаб сетки
        }
        z1 +=0.5;//можно регулировать качество изображения меняя масштаб сетки
        z2 = 0;
    }
}
//аналогичная предыдущей функция только для чтения массива хранящего информацию о гауссианах
void BasePoints(bell* matrix, int N, int M) {
    double z1 = 0, z2 = 0, z3 = 0;
    //double fdispersion[4];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            z3=rand() / double(RAND_MAX) * 10 - 5;
            matrix[i * M + j].setPoint(point(z1, z2, z3));
            //fdispersion[0] = random() / double(RAND_MAX) * 1.9 + 0.1;
            //fdispersion[2] = random() / double(RAND_MAX) * 1.9 + 0.1;
            //fdispersion[1] = 0.0;
            //fdispersion[3] = 0.0;
            //matrix[i * M + j].setDispersion(fdispersion);
            z2 += 5;//можно регулировать координаты вершин гауссиан
        }
        z1 +=5;//можно регулировать координаты вершин гауссиан
        z2 = 0;
    }
}
//функция которая создает файл с точками для gnuplot
void print(std::ofstream& file, point* Matrix, bell* Array, int N, int M, int K, int L) {
    for (int n = 0; n < N; n++) {
        for (int m = 0; m < M; m++) {
            double h = 0;
            for (int k = 0; k < K; k++) {
                for (int l = 0; l < L; l++) {
                    h += Array[k * L + l].gauss(Matrix[n * M + m]);
                }
            }
            file << (Matrix[n * M + m]).getx() << " " << Matrix[n * M + m].gety() << " " << h << "\n";
        }
        file << "\n";
    }
}

int main() {
    srand(time(NULL));
    int K = 10, L = 10, N = 100, M = 100;//удобно регулировать площадь поверхности 
    point* data = new point[N * M];
    bell* guss = new bell[K * L];
    field(data, N, M);
    BasePoints(guss, K, L);
    std::ofstream file("bruh.txt");
    print(file, data, guss, N, M, K, L);
    file.close();
    delete[] data;
    delete[] guss;
    std::cout << "good job, bro";
    return 0;
}