       #include<GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include<stack>
//���ô���ص�
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
//���ù���ESC���Ļص�
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

struct Point2D {
    int x;
    int y;
};
void drawPoint(GLFWwindow* window,int x,int y) {
        float ratio;
        int width,height;

        /* ��ȡ����֡�Ĵ�С*/
        glfwGetFramebufferSize(window,&width,&height);
        ratio = width/(float) height;

        /*�����ӿڲ���֮ǰ����ɫ�������*/
        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT);

        /*���þ���ģʽΪͶ��ģʽ�������Ӿ���*/
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glOrtho(-ratio*500,ratio*500,-500.f,500.f,1.f,-1.f);


        /*���������β������ɫ*/
        glBegin(GL_POINTS);
        glColor3f(0.f/255.f,0.f/255.f,255.f/255.f);
        glVertex3f((float)x,(float)y,0.f);

        glEnd();

        glfwSwapBuffers(window);

        /*�����¼�*/
        printf("check\n");
        glfwPollEvents();

};
stack<Point2D> BresenhamForLine(GLFWwindow* window,int x1, int y1,int x2, int y2)
{
    float m;
    int DeltaX = 10*abs(x1-x2);
    int DeltaY = 10*abs(y1-y2);
    int TwoDeltaY = 2*DeltaY;
    int TwoDeltaX = 2*DeltaX;
    int TwoDeltaXMinusTwoDeltaY = TwoDeltaX-TwoDeltaY;//when y is independent variable
    int TwoDeltaYMinusTwoDeltaX = TwoDeltaY-TwoDeltaX;//when x is independent variable
    int P0ForY = TwoDeltaX-DeltaY;//
    int P0ForX = TwoDeltaY-DeltaX;

    struct Point2D startPoint;
    struct Point2D endPoint;

    struct Point2D pointOne;
    pointOne.x = x1;
    pointOne.y = y1;

    struct Point2D pointTwo;
    pointTwo.x = x2;
    pointTwo.y = y2;

    stack<Point2D> stackOfPoints;
    m = abs(y1-y2)/(float) abs(x1-x2);
    printf("%f",m);
    if (m<1)
    {
        if (x1<x2) {
            startPoint = pointOne;
            endPoint = pointTwo;
        } else {
            startPoint = pointTwo;
            endPoint = pointOne;
        }
        drawPoint(window,startPoint.x,startPoint.y);
        int tempForPi = P0ForX;
        int tempForXi = 10*startPoint.x;
        int tempForYi = 10*startPoint.y;
        Point2D tempForPoint;
        for(;tempForXi<10*endPoint.x;){
            tempForXi++;
            printf("%d\n",tempForXi);
            if(tempForPi <= 0) {
                tempForPoint.x = tempForXi;
                tempForPoint.y = tempForYi;
                stackOfPoints.push(tempForPoint);
                tempForPi = tempForPi+TwoDeltaY;
            }
            else {
                tempForPoint.x = tempForXi;
                tempForPoint.y = tempForYi;
                stackOfPoints.push(tempForPoint);
                tempForPi = tempForPi+TwoDeltaYMinusTwoDeltaX;
            }
        }

    }else{
        if (y1<y2) {
            startPoint = pointOne;
            endPoint = pointTwo;
        } else {
            startPoint = pointTwo;
            endPoint = pointOne;
        }
        drawPoint(window,startPoint.x,startPoint.y);
        int tempForPi = P0ForY;
        int tempForXi = 10*startPoint.x;
        int tempForYi = 10*startPoint.y;
        for(;tempForYi<10*endPoint.y;){
            tempForYi++;
            if(tempForPi <= 0) {
                tempForPoint.x = tempForXi;
                tempForPoint.y = tempForYi;
                stackOfPoints.push(tempForPoint);
                tempForPi = tempForPi+TwoDeltaX;
            }
            else {
                tempForPoint.x = tempForXi;
                tempForPoint.y = tempForYi;
                stackOfPoints.push(tempForPoint);
                tempForPi = tempForPi+TwoDeltaXMinusTwoDeltaY;
            }
        }

    }
    return stackOfPoints;
}
int main(void)
{
    int inputX1,inputY1,inputX2,inputY2,i;
    GLFWwindow* window;
    stack<Point2D> pointsToRaster;
    Point2D temp��

    glfwSetErrorCallback(error_callback);

    /* ��ʼ��GLFW�� */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    /*�������ں������� */
    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* ������ǰ������*/
    glfwMakeContextCurrent(window);
    printf("Please input the x1 y1 x2 y2 in order to draw a Line:\n\Only integer is acceptable\n");
    scanf("%d%d%d%d",&inputX1,&inputY1,&inputX2,&inputY2);
    pointsToRaster = BresenhamForLine(window,inputX1,inputY1,inputX2,inputY2);

    /* ��ѭ�� */
    while (!glfwWindowShouldClose(window))
    {
        /* ��ʼ��Ⱦ�׶� */

        float ratio;
        int width,height;

        /* ��ȡ����֡�Ĵ�С*/
        glfwGetFramebufferSize(window,&width,&height);
        ratio = width/(float) height;

        /*�����ӿڲ���֮ǰ����ɫ�������*/
        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT);

        /*���þ���ģʽΪͶ��ģʽ�������Ӿ���*/
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glOrtho(-ratio,ratio,-500.f,500.f,1.f,-1.f);


        /*���������β������ɫ*/
        glBegin(GL_POINTS);
        glColor3f(0.f/255.f,0.f/255.f,255.f/255.f);
        for (i = 0;i<pointsToRaster.size();i++) {
            pointsToRaster.push(temp);
            glVertex3f((float)temp.x,(float)temp.y,0.f);
        }


        glEnd();


        /*����������*/
        glfwSwapBuffers(window);

        /*�����¼�*/
        glfwPollEvents();
    }

    /*�رմ��ڲ�ֹͣʹ��GLFW�Ŀ�*/
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


