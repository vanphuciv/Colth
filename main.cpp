#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <GL/gl.h>
#include <vecmath.h>
#include "camera.h"
#include <string.h> 


/// TODO: đưa thêm tiêu đề nếu cần thiết

#include "TimeStepper.hpp"
#include "ClothSystem.h"
#include "VaCham.h"

using namespace std;

// Toàn cầu ở đây.
namespace
{
    //------------------------------------------------------------

    ParticleSystem *system;
    TimeStepper * timeStepper;
    VaCham *vacham;

  // khởi tạo các hệ thống hạt của bạn
  ///TODO: read argv here. set timestepper , step size etc
  void initSystem(int argc, char * argv[])
  {
    // gieo hạt bộ tạo số ngẫu nhiên với thời gian hiện tại
    srand( time( NULL ) );
    system = new ClothSystem(10);
    timeStepper = new RK4();	
    vacham = new Sphere();
  }

  // Đi một bước tiến đối với các vòi sen hạt
  ///TODO: Tùy chọn. sửa đổi chức năng này để hiển thị các hệ thống hạt khác nhau và phù thủy giữa timeSteppers khác nhau
  void stepSystem()
  {
      ///TODO Các kích thước bước nên thay đổi theo đối số dòng lệnh
    const float h = 0.04f;
    if(timeStepper!=0){
      timeStepper->takeStep(system,h);
      vacham->sphereVC(system);                 //Bỏ nó nếu không thích va chạm
    }
  }

  // Vẽ các vị trí hạt hiện tại
  void drawSystem()
  {
    
    // Màu sắc vật liệu cơ bản (không thay đổi)
    GLfloat particleColor[] = {0.4f, 0.7f, 1.0f, 1.0f};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, particleColor);
    
    system->draw();
    
    

    
  }
        

    //-------------------------------------------------------------------
    
        
    // This is the camera
    Camera camera;

    // These are state variables for the UI
    bool g_mousePressed = false;

    // Các tuyên bố của các chức năng mà hiện thực xảy ra sau này.
  //  void arcballRotation(int endX, int endY);
    void keyboardFunc( unsigned char key, int x, int y);
    void specialFunc( int key, int x, int y );
    void mouseFunc(int button, int state, int x, int y);
    void motionFunc(int x, int y);
    void reshapeFunc(int w, int h);
    void drawScene(void);
    void initRendering();

    // Chức năng này được gọi là bất cứ khi nào một "bình thường" bấm phím là nhận được.
    void keyboardFunc( unsigned char key, int x, int y )
    {
        switch ( key )
        {
        case 27: // Escape key
            exit(0);
            break;
        case ' ':
        {
            Matrix4f eye = Matrix4f::identity();
            camera.SetRotation( eye );
            camera.SetCenter( Vector3f::ZERO );
            break;
        }
        default:
            cout << "Unhandled key press " << key << "." << endl;        
        }

        glutPostRedisplay();
    }

    // Chức năng này được gọi là bất cứ khi nào một "đặc biệt" nhấn phím được tiếp nhận. Ngay bây giờ, nó xử lý các phím mũi tên.
    void specialFunc( int key, int x, int y )
    {
        switch ( key )
        {

        }
        //glutPostRedisplay();
    }

    //  Được gọi khi nút chuột được nhấn.
    void mouseFunc(int button, int state, int x, int y)
    {
        if (state == GLUT_DOWN)
        {
            g_mousePressed = true;
            
            switch (button)
            {
            case GLUT_LEFT_BUTTON:
                camera.MouseClick(Camera::LEFT, x, y);
                break;
            case GLUT_MIDDLE_BUTTON:
                camera.MouseClick(Camera::MIDDLE, x, y);
                break;
            case GLUT_RIGHT_BUTTON:
                camera.MouseClick(Camera::RIGHT, x,y);
            default:
                break;
            }                       
        }
        else
        {
            camera.MouseRelease(x,y);
            g_mousePressed = false;
        }
        glutPostRedisplay();
    }

    // Được gọi khi mouse di chuyển nút bấm.
    void motionFunc(int x, int y)
    {
        camera.MouseDrag(x,y);        
    
        glutPostRedisplay();
    }

    // Được gọi khi cửa sổ được thay đổi kích thước w, h - chiều rộng và chiều cao của cửa sổ bằng pixel.
    void reshapeFunc(int w, int h)
    {
        camera.SetDimensions(w,h);

        camera.SetViewport(0,0,w,h);
        camera.ApplyViewport();

        // Thiết lập một phối cảnh, với tỉ lệ vuông
        glMatrixMode(GL_PROJECTION);

        camera.SetPerspective(50);
        glLoadMatrixf( camera.projectionMatrix() );
    }

    // Khởi tạo chế độ dựng hình OpenGL của
    void initRendering()
    {
        glEnable(GL_DEPTH_TEST);   // Kiểm tra độ sâu phải được bật
        glEnable(GL_LIGHTING);     // Kích hoạt tính năng tính toán chiếu sáng
        glEnable(GL_LIGHT0);       // Bật đèn # 0.

        glEnable(GL_NORMALIZE);

        // Thiết lập bản vẽ đa giác
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        


        // Rõ ràng để màu đen
        glClearColor(0,0,0,1);
    }

    // Chức năng này là trách nhiệm hiển thị các đối tượng.
    void drawScene(void)
    {
        // Xóa cửa sổ render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode( GL_MODELVIEW );  
        glLoadIdentity();              

        // Màu ánh sáng (RGBA)
        
        GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
        GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

        glLoadMatrixf( camera.viewMatrix() );

        //NÀY LÀ MÃ VẼ.
        
        //------------vẽ mặt phẳng bằng cube-------------
            GLfloat floorColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor);
            glPushMatrix();
            glTranslatef(0.0f,-70.0f,0.0f);
            glScaled(50.0f,0.01f,50.0f);
            glutSolidCube(1);
            glPopMatrix();
            
            
        //--------Vẽ quả cầu lớn------------
            
            GLfloat floorColor2[] = {0.0f, 1.0f, 0.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, floorColor2);
            glPushMatrix();
            glTranslatef(2.5f,-10.0f,2.5f);
            glutSolidSphere(1.5f,20,20);
            glPopMatrix();     

            

        
        // Điều này thu hút các trục tọa độ khi bạn đang quay, giữ cho mình định hướng.
        if( g_mousePressed )
        {
            glPushMatrix();
            Vector3f eye = camera.GetCenter();
            glTranslatef( eye[0], eye[1], eye[2] );

            // Lưu trạng thái hiện tại của OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            // Điều này là để rút ra các trục khi nút chuột xuống
            glDisable(GL_LIGHTING);
            glLineWidth(3);
            glPushMatrix();
            glScaled(5.0,5.0,5.0);
            glBegin(GL_LINES);
            glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
            glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
            glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);

            glColor4f(0.5,0.5,0.5,1);
            glVertex3f(0,0,0); glVertex3f(-1,0,0);
            glVertex3f(0,0,0); glVertex3f(0,-1,0);
            glVertex3f(0,0,0); glVertex3f(0,0,-1);

            glEnd();
            glPopMatrix();

            glPopAttrib();
            glPopMatrix();
        } 
                 
        // Đổ hình ảnh lên màn hình.
        
        drawSystem();
        glutSwapBuffers();
    }

    void timerFunc(int t)
    {
        stepSystem();

        glutPostRedisplay();

        glutTimerFunc(t, &timerFunc, t);
    }

    

    
    
}

// Main
// Thiết lập OpenGL, xác định các gọi lại và bắt đầu vòng lặp chính
int main( int argc, char* argv[] )
{
    glutInit( &argc, argv );

    // Chúng tôi ta sẽ tạo hiệu hiệu ứng, bộ đệm để double
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    //Thông số ban đầu cho vị trí cửa sổ và kích thước
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 600, 600 );
    
    camera.SetDimensions( 600, 600 );

    camera.SetDistance( 10 );
    camera.SetCenter( Vector3f::ZERO );
    
    glutCreateWindow("Assignment 4");

    // Khởi tạo thông số OpenGL.
    initRendering();

    // Hệ thống hạt Cài đặt
   initSystem(argc,argv);

    // Thiết lập chức năng gọi lại cho phím bấm
    glutKeyboardFunc(keyboardFunc); // Xử lý các biểu tượng ascii "bình thường"
    glutSpecialFunc(specialFunc);   // Xử lý các phím bàn phím "đặc biệt"

    // Thiết lập chức năng gọi lại cho chuột
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    // Thiết lập các chức năng gọi lại cho thay đổi kích thước cửa sổ
    glutReshapeFunc( reshapeFunc );

    // Gọi bất cứ khi nào cửa sổ cần vẽ lại
    glutDisplayFunc( drawScene );

    //Kích hoạt timerFunc mỗi 20 msec
    glutTimerFunc(20, timerFunc, 20);

        
    // Bắt đầu vòng lặp chính. glutMainLoop không bao giờ trở về.
    glutMainLoop();

    return 0;	// Dòng này là không bao giờ đạt được.
}
