
#include"boundery_and_control.cpp"

point A, B;
class Rover{
   protected:
     double pixel;
     vector<point> rover;
     field* surface;
     double angle_F;
     double angle_R;//back
     double speed;
     double clirence;
     vector<point> way;

   public:
     
     Rover(field* F) : surface(F){
        Porder orderA = F->order_of_point(A);
        int i = orderA.geti();
        int j = orderA.getj();
        pixel = F->GetPixel();
        rover.push_back(A);//p0
        rover.push_back(A + point(0, pixel, 0) +  point(0, 0, F->Field[i][j+1].getz()));//p1
        rover.push_back(A + point(pixel, pixel, 0 ) + point(0, 0, F->Field[i+1][j+1].getz()));//p2
        rover.push_back(A + point(pixel, 0, 0) + point(0, 0, F->Field[i+1][j].getz()));//p3
        rover.push_back(A + point(pixel, -pixel, 0) + point(0, 0, F->Field[i+1][j-1].getz()));//p4
        rover.push_back(A + point(0, -pixel, 0) + point(0, 0, F->Field[i][j-1].getz()));//p5
        rover.push_back(A + point(-pixel, -pixel, 0) + point(0, 0, F->Field[i-1][j-1].getz()));//p6
        rover.push_back(A + point(-pixel, 0, 0) + point(0, 0, F->Field[i-1][j].getz()));//p7
        rover.push_back(A + point(-pixel, pixel, 0) + point(0, 0, F->Field[i-1][j+1].getz()));//p8
        angle_F = 3;
        angle_R = 3;
        speed = 4*pixel;
        clirence = pixel/2;
        way.clear();   
     }

     string GG(){
        double angle_front, angle_back, angle_right, angle_left;
        angle_front = atan((rover[1].getx()-rover[0].getx())/(rover[1].getz()-rover[0].getz()));
        angle_back = atan((rover[5].getx()-rover[0].getx())/(rover[5].getz()-rover[0].getz()));
        angle_right = atan((rover[3].getx()-rover[0].getx())/(rover[3].getz()-rover[0].getz()));
        angle_left = atan((rover[7].getx()-rover[0].getx())/(rover[7].getz()-rover[0].getz()));
        if(angle_front > angle_F) return "good game";
        if(angle_back > angle_F) return "good game";
        if(angle_right > angle_R) return "good game";
        if(angle_left > angle_R) return "good game";
        if(rover[1].getz() < clirence) return "good game";
        if(rover[0].getz() < clirence) return "good game";
        if(rover[5].getz() < clirence) return "good game";
        return "good enough";
     }

     string orientation(point p0, point p1){
      if(p0.getx()==p1.getx() && p0.gety() > p1.gety()) return "left";
      if(p0.getx()==p1.getx() && p0.gety() < p1.gety()) return "right";
      if(p0.getx()>p1.getx() && p0.gety()==p1.gety()) return "back";
      return "front";
     }

     void ReBuild(point fix){
        Porder orderFix = surface->order_of_point(fix);
        int i = orderFix.geti();
        int j = orderFix.getj();
        string orient = orientation(rover[0], rover[1]);
        if(orient == "right"){//ok
          rover.clear();
          rover.push_back(fix + point(0, 0, surface->Field[i][j].getz()));//p0
          rover.push_back(fix + point(pixel, 0, 0) +  point(0, 0, surface->Field[i+1][j].getz()));//p1
          rover.push_back(fix + point(pixel, -pixel, 0 ) + point(0, 0, surface->Field[i+1][j-1].getz()));//p2
          rover.push_back(fix + point(0, -pixel, 0) + point(0, 0, surface->Field[i][j-1].getz()));//p3
          rover.push_back(fix + point(-pixel, -pixel, 0) + point(0, 0, surface->Field[i-1][j-1].getz()));//p4
          rover.push_back(fix + point(-pixel, 0, 0) + point(0, 0, surface->Field[i-1][j].getz()));//p5
          rover.push_back(fix + point(-pixel, pixel, 0) + point(0, 0, surface->Field[i-1][j+1].getz()));//p6
          rover.push_back(fix + point(0, pixel, 0) + point(0, 0, surface->Field[i][j+1].getz()));//p7
          rover.push_back(fix + point(pixel, pixel, 0) + point(0, 0, surface->Field[i+1][j+1].getz()));//p8
        }//
        if(orient == "front"){
          rover.clear();
          rover.push_back(fix + point(0, 0, surface->Field[i][j].getz()));//p0
          rover.push_back(fix + point(0, pixel, 0) +  point(0, 0, surface->Field[i][j+1].getz()));//p1
          rover.push_back(fix + point(pixel, pixel, 0 ) + point(0, 0, surface->Field[i+1][j+1].getz()));//p2
          rover.push_back(fix + point(pixel, 0, 0) + point(0, 0, surface->Field[i+1][j].getz()));//p3
          rover.push_back(fix + point(pixel, -pixel, 0) + point(0, 0, surface->Field[i+1][j-1].getz()));//p4
          rover.push_back(fix + point(0, -pixel, 0) + point(0, 0, surface->Field[i][j-1].getz()));//p5
          rover.push_back(fix + point(-pixel, -pixel, 0) + point(0, 0, surface->Field[i-1][j-1].getz()));//p6
          rover.push_back(fix + point(-pixel, 0, 0) + point(0, 0, surface->Field[i-1][j].getz()));//p7
          rover.push_back(fix + point(-pixel, pixel, 0) + point(0, 0, surface->Field[i-1][j+1].getz()));//p8
        }
        if(orient == "left"){
          rover.clear();
          rover.push_back(fix + point(0, 0, surface->Field[i][j].getz()));//p0
          rover.push_back(fix + point(-pixel, 0, 0) +  point(0, 0, surface->Field[i-1][j].getz()));//p1
          rover.push_back(fix + point(-pixel, pixel, 0 ) + point(0, 0, surface->Field[i-1][j+1].getz()));//p2
          rover.push_back(fix + point(0, pixel, 0) + point(0, 0, surface->Field[i][j+1].getz()));//p3
          rover.push_back(fix + point(pixel, pixel, 0) + point(0, 0, surface->Field[i+1][j+1].getz()));//p4
          rover.push_back(fix + point(pixel, 0, 0) + point(0, 0, surface->Field[i+1][j].getz()));//p5
          rover.push_back(fix + point(pixel, -pixel, 0) + point(0, 0, surface->Field[i+1][j-1].getz()));//p6
          rover.push_back(fix + point(0, -pixel, 0) + point(0, 0, surface->Field[i][j-1].getz()));//p7
          rover.push_back(fix + point(-pixel, -pixel, 0) + point(0, 0, surface->Field[i-1][j-1].getz()));//p8
        }
        if(orient == "back"){
          rover.clear();
          rover.push_back(fix + point(0, 0, surface->Field[i][j].getz()));//p0
          rover.push_back(fix + point(0, -pixel, 0) +  point(0, 0, surface->Field[i][j-1].getz()));//p1
          rover.push_back(fix + point(-pixel, -pixel, 0 ) + point(0, 0, surface->Field[i-1][j-1].getz()));//p2
          rover.push_back(fix + point(-pixel, 0, 0) + point(0, 0, surface->Field[i-1][j].getz()));//p3
          rover.push_back(fix + point(-pixel, pixel, 0) + point(0, 0, surface->Field[i-1][j+1].getz()));//p4
          rover.push_back(fix + point(0, pixel, 0) + point(0, 0, surface->Field[i][j+1].getz()));//p5
          rover.push_back(fix + point(pixel, pixel, 0) + point(0, 0, surface->Field[i+1][j+1].getz()));//p6
          rover.push_back(fix + point(pixel, 0, 0) + point(0, 0, surface->Field[i+1][j].getz()));//p7
          rover.push_back(fix + point(pixel, -pixel, 0) + point(0, 0, surface->Field[i+1][j-1].getz()));//p8
        }
     }

     void Move_NN(){
       string orient = orientation(rover[0], rover[1]);
       if(orient == "front"){
        rover[0]+=point(0,speed,0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
       if(orient == "right"){
        rover[0]+=point(speed, 0, 0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
       if(orient == "left"){
        rover[0]+=point(-speed, 0, 0);
        point fix = rover[0];
        this->ReBuild(fix);
       }       
       if(orient == "back"){
        rover[0]+=point(0,-speed,0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
     }  

     void Move_NE(){
       string orient = orientation(rover[0], rover[1]);
       if(orient == "front"){
        rover[0]+=point(speed,speed,0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
       if(orient == "right"){
        rover[0]+=point(speed, -speed, 0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
       if(orient == "left"){
        rover[0]+=point(-speed, speed, 0);
        point fix = rover[0];
        this->ReBuild(fix);
       }       
       if(orient == "back"){
        rover[0]+=point(-speed,-speed,0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
     }

     void Move_NW(){
       string orient = orientation(rover[0], rover[1]);
       if(orient == "front"){
        rover[0]+=point(-speed,speed,0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
       if(orient == "right"){
        rover[0]+=point(speed, speed, 0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
       if(orient == "left"){
        rover[0]+=point(-speed, -speed, 0);
        point fix = rover[0];
        this->ReBuild(fix);
       }       
       if(orient == "back"){
        rover[0]+=point(speed,-speed,0);
        point fix = rover[0];
        this->ReBuild(fix);
       }
     }

     void Move_EE(){
        this->Turn_Right();
        this->Move_NN();
     }

     void Move_SE(){
        this->Turn_Around();
        this->Move_NW();
     }  

     void Move_SS(){
        this->Turn_Around();
        this->Move_NN();
     }     

     void Move_SW(){
        this->Turn_Around();
        this->Move_NE();
     }

     void Move_WW(){
        this->Turn_Left();
        this->Move_NN();
     }     

     void Turn_Right(){
       vector<point> buffer1, buffer2;
       point buffer3 = rover[0];
       for(auto i = 1; i < 7; i++) 
         buffer1.push_back(rover[i]);
       for(auto i = 7; i < 9; i++)
         buffer2.push_back(rover[i]);
       rover.clear();
       rover.push_back(buffer3);
       for(size_t i = 0; i < buffer2.size(); i++)
         rover.push_back(buffer2[i]);
       for(size_t i = 0; i < buffer1.size(); i++)
         rover.push_back(buffer1[i]);
       buffer1.clear();
       buffer2.clear();
     }

     void Turn_Left(){
       this->Turn_Right();
       this->Turn_Right();
       this->Turn_Right();
     }

     void Turn_Around(){
       this->Turn_Right();
       this->Turn_Right();
     }

     double direction(){
        point v1 = B - A, v2 = rover[1] - rover[0];
        double cosin, normal_v1, normal_v2, scalmult;
        normal_v1 = sqrt(v1.getx()*v1.getx() + v1.gety()*v1.gety());
        normal_v2 = sqrt(v2.getx()*v2.getx() + v2.gety()*v2.gety());
        scalmult = v1.getx()*v2.getx() + v1.gety()*v2.gety();
        cosin = scalmult/(normal_v1*normal_v2);
        return cosin; 
     }

     void change_direction(){
       double cosin = this->direction();
       while(cosin <= 0){
         this->Turn_Left();
         cosin = this->direction();
       }
     }



    bool check_that(){
    for(size_t i=0; i<9; i++){
       if(rover[i]==B) return false;
    }
    return true;
    }

    void processor(){
      this->change_direction();
      while(this->check_that()){
        if(this->direction() <= 0) this->change_direction();       
        this->Move_NN();
        if(this->GG()=="good game"){
          this->Move_SS();
          this->Move_EE();
          this->processor();
        }
        way.push_back(rover[0]);
      }
    }
    
    
    void getWay(vector<point> Way){
    Way = way;
    }

  ~Rover(){
       rover.clear();
       delete surface;
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







