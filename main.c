#include<GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define ROUND(a) ((int)(a+0.5))
#define F(x,y,r) x*x+y*y-r*r
/*above is the formula for the circle*/

//set error callback
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
//set key ESC callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

struct Point2D {
    int x;
    int y;
};
/*points used to store the (x,y)
num used to indicates that how many point we should draw*/
int points[4000];
int num = 0;

/*we start from (0,r) and end when x >= y
each loop we check if F(x,y,r) is over zero,
if then let y decrease keep x unchanged,
else let x increase,keep y unchanged
O X
X   such like that*/
void PositiveNegativeForCircle(int r) {
    int xi = 0;
    int yi = r;
    int i = 0;
    points[i] = 0;
    points[i+1] = yi;
    i +=2;
    num++;
    for (;xi<=yi;i = i+2) {
        if (F(xi,yi,r)<=0) {
            xi++;
            points[i] = xi;
            points[i+1] = yi;
            num++;
        } else {
            yi--;
            points[i] = xi;
            points[i+1] = yi;
            num++;
        }
    }
}

/*In DDA , the main idea is to calculate the slope,
if slope >1 ,then let  y be the independent variable,each loop
    y increase by 1 , x increase by slope , and round x
In the another case , slope<1 , then let x be the independent variable ,
each loop x increase by 1 , y increase by slope ,and round y*/
void DDA(int x1 ,int y1 , int x2 , int y2) {
    int dx=x2-x1,dy=y2-y1,steps,k;
    float xIncrement,yIncrement,x = x1, y = y1;

    if (abs(dx)>abs(dy)) steps = abs(dx);
        else steps = abs(dy);
    xIncrement = dx/(float) steps;
    yIncrement = dy/(float) steps;

    int i = 0;
    points[i] = ROUND(x);
    points[i+1] = ROUND(y);
    i = i+2;
    num++;

    for (k = 0;k<steps;k++)
    {
        x+=xIncrement;
        y+=yIncrement;
        points[i] = ROUND(x);
        points[i+1] = ROUND(y);
        i = i+2;
        num++;
    }
}

/*This algorithm is very similar to PositiveNegative,but this method
is choose the middle point ,and calculate the x^2+y^2+r^2,
check whether it is over zero.If over zero ,then y decrease ,otherwise y remains
O  X
   X such like that*/
void BresenhamForCircle(int r) {
    float di = 1.25 -r;
    int xi = 0;
    int yi = r;
    int i = 0;
    points[i] = 0;
    points[i+1] = yi;
    i +=2;
    num++;
    for (; xi<=yi;xi++,i = i+2) {
        if (di<=0) {
            di = di+2*xi+3;
            points[i] = xi+1;
            points[i+1] = yi;
        } else {
            di = di + 2*(xi-yi)+5;
            points[i] = xi+1;
            points[i+1] = yi-1;
            yi--;
        }
        num++;
    }

}

/*DDA uses the slope as the step ,Bresenham uses the di
di and di+1 have some certain relation so when we know di
we can decide next point.And Bresenham dont use float just use
integer */
void BresenhamForLine(int x1, int y1,int x2, int y2)
{
    float m;
    int DeltaX = abs(x1-x2);
    int DeltaY = abs(y1-y2);
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


    m = (y1-y2)/(float)(x1-x2);
    printf("%f",m);
    /*Case One : x increase y increase*/
    if (m<1&&m>0){
        if (x1<x2) {
            startPoint = pointOne;
            endPoint = pointTwo;
        } else {
            startPoint = pointTwo;
            endPoint = pointOne;
        }
        printf("%d\n",startPoint.x);
        printf("%d\n",endPoint.x);

        int tempForPi = P0ForX;
        int tempForXi = startPoint.x;
        int tempForYi = startPoint.y;
        int i;
        for(i = 0;tempForXi<endPoint.x;){
            tempForXi++;
            if(tempForPi <= 0) {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForPi = tempForPi+TwoDeltaY;
                num++;
            }
            else {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForYi++;
                tempForPi = tempForPi+TwoDeltaYMinusTwoDeltaX;
                num++;
            }
            i = i + 2;
        }
    }else if(m>1){
        /*Case Two : y increase x increase*/
        if (y1<y2) {
            startPoint = pointOne;
            endPoint = pointTwo;
        } else {
            startPoint = pointTwo;
            endPoint = pointOne;
        }
        int tempForPi = P0ForY;
        int tempForXi = startPoint.x;
        int tempForYi = startPoint.y;
        int i;
        for(i=0;tempForYi<endPoint.y;){
            tempForYi++;
            if(tempForPi <= 0) {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForPi = tempForPi+TwoDeltaX;
                num++;
            }
            else {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForXi++;
                tempForPi = tempForPi+TwoDeltaXMinusTwoDeltaY;
                num++;
            }
            i = i+2;
        }
    }else if (m<-1) {
        /*Case Three : y increase x decrease*/
         if (y1<y2) {
            startPoint = pointTwo;
            endPoint = pointOne;
        } else {
            startPoint = pointOne;
            endPoint = pointTwo;
        }
        int tempForPi = P0ForY;
        int tempForXi = startPoint.x;
        int tempForYi = startPoint.y;
        int i;
        for(i=0;tempForYi<endPoint.y;){
            tempForYi++;
            if(tempForPi <= 0) {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForPi = tempForPi+TwoDeltaX;
                num++;
            }
            else {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForXi--;
                tempForPi = tempForPi+TwoDeltaXMinusTwoDeltaY;
                num++;
            }
            i = i+2;
        }
    } else if (m<0&&m>-1) {
        /*Case Four : x decrease y increase*/
        if (x1<x2) {
            startPoint = pointTwo;
            endPoint = pointOne;
        } else {
            startPoint = pointOne;
            endPoint = pointTwo;
        }
        printf("%d\n",startPoint.x);
        printf("%d\n",endPoint.x);

        int tempForPi = P0ForX;
        int tempForXi = startPoint.x;
        int tempForYi = startPoint.y;
        int i;
        for(i = 0;tempForXi>endPoint.x;){
            tempForXi--;
            if(tempForPi <= 0) {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForPi = tempForPi+TwoDeltaY;
                num++;
            }
            else {
                points[i] = tempForXi;
                points[i+1] = tempForYi;
                tempForYi++;
                tempForPi = tempForPi+TwoDeltaYMinusTwoDeltaX;
                num++;
            }
            i = i + 2;
        }
    }
}
int main(void)
{
    int inputX1,inputY1,inputX2,inputY2,signal;
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    /* initial GLFW */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    /*create window and context */
    window = glfwCreateWindow(640, 480, "Bresenham", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* create context*/
    glfwMakeContextCurrent(window);

    printf("Please input 1 If you want to draw a line,Please input 2 if you want to draw a Circle\n");
    scanf("%d",&signal);

    if (signal == 1) {

        printf("Please input the x1 y1 x2 y2 in order to draw a Line:\n\Only integer is acceptable\n");
        scanf("%d%d%d%d",&inputX1,&inputY1,&inputX2,&inputY2);
//      DDA(inputX1,inputY1,inputX2,inputY2);
        BresenhamForLine(inputX1,inputY1,inputX2,inputY2);

    } else if (signal == 2) {

        printf("Please input the x1 y1 r1 in order to draw a Circle:\n\Only integer is acceptable\n");
        scanf("%d%d%d",&inputX1,&inputY1,&inputY2);
        BresenhamForCircle(inputY2);
//      PositiveNegativeForCircle(inputY2);

    }

    /* main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* rendering period */

        float ratio;
        int width,height;

        glfwGetFramebufferSize(window,&width,&height);
        ratio = width/(float) height;

        glViewport(0,0,width,height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio*300,ratio*300,-300.f,300.f,1.f,-1.f);



        glBegin(GL_POINTS);
        glColor3f(0.f/255.f,0.f/255.f,255.f/255.f);
        int i;
        if (signal == 1)
        for (i = 0; i <num;i = i+2) {
            glVertex2f((float)points[i],(float)points[i+1]);
        }
        else if (signal ==2)
        //depending on the symmetry , when we get the 1/8 part of the circle we can draw
        // the whole
        for ( i = 0;i < num;i = i +2)  {
            glVertex2f((float)points[i]+inputX1,(float)points[i+1]+inputY1);
            glVertex2f((float)-1*points[i]+inputX1,(float)points[i+1]+inputY1);
            glVertex2f((float)points[i]+inputX1,(float)-1*points[i+1]+inputY1);
            glVertex2f((float)-1*points[i]+inputX1,(float)-1*points[i+1]+inputY1);
            glVertex2f((float)points[i+1]+inputX1,(float)points[i]+inputY1);
            glVertex2f((float)-1*points[i+1]+inputX1,(float)points[i]+inputY1);
            glVertex2f((float)points[i+1]+inputX1,(float)-1*points[i]+inputY1);
            glVertex2f((float)-1*points[i+1]+inputX1,(float)-1*points[i]+inputY1);
        }

        glEnd();


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


