
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>

#include<iostream>
#include<vector>
#include<algorithm>

#include"loadObj.h"
#include"Vertex.h"
#include"Camera.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800




void putpixel(glm::vec2 pixel, glm::vec3 color);
void line(float x1, float y1, float x2, float y2, glm::vec3 color);
void putpixel_adjusted(glm::vec2 pixel, glm::vec3 color);

void triangle(std::vector<glm::vec4> &vertices);
void triangle(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3);

void rasterize(glm::vec4 V1, glm::vec4 V2, glm::vec4 V3, glm::vec3 color);
void rasterize(std::vector<glm::vec4> vertices);
void rasterizeByTriangle(std::vector<glm::vec4> vertices,std::vector<glm::vec3>crntPos);
void rasterizeByTriangle(std::vector<faces> trianglefaces);

bool backfaceDetection(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

std::vector<Vertex> TriangleVertices;
std::vector<Vertex> TriangleVertices1;
std::vector<glm::vec3> crntPos;

glm::vec4 v1 =  glm::vec4(-0.5,0.0,0.0,1.0);
glm::vec4 v2 = glm::vec4(0.5, 0.0, 0.0,1.0);
glm::vec4 v3 = glm::vec4(0.0, 0.8,0.0,1.0);

Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 5.0f, 20.0f));

void clear()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1);
}

int main(void)
{
    
   
    glm::mat4 cameraMatrix;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model1 = glm::mat4(1.0f);

    GLFWwindow* window;
   
    
    

    TriangleVertices = loadOBJ("objects/bed.obj");
    //TriangleVertices1 = loadOBJ("objects/TestCube.obj");

    //TriangleVertices = loadOBJ("objects/Stupa3.obj");

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    //glfwSetKeyCallback(window, key_callback);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT,-1,1); // essentially set coordinate system
     
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment

    //model = glm::scale(model,glm::vec3(1.5,1.5,1.5));
    //model = glm::translate(model,glm::vec3(0.5,0.5,0.5));
    //model1 = glm::translate(model1, glm::vec3(0.5,0.0,0.0));
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        clear();
        glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute
        
        // Render OpenGL here
        glEnable(GL_POINT_SMOOTH); // make the point circular
        
        //putpixel(glm::vec3(10, 10, 10), glm::vec3(1, 1, 1));
        //putpixel_adjusted(glm::vec3(10, 10, 10), glm::vec3(1, 1, 1));
      
        //line(10,10,30,30,glm::vec3(1.0f,0.0f,0.0f));
        camera.Inputs(window);
        cameraMatrix = camera.Matrix(45.0f, 0.1f, 100.0f);
      /*  glm::vec4 temp1 = model1 * cameraMatrix * v1;
        glm::vec4 temp2 = model1 * cameraMatrix * v2;
        glm::vec4 temp3 = model1 * cameraMatrix * v3;
        */
        std::vector<glm::vec4> Vertices1;
        std::vector<glm::vec4> Vertices2;
        std::vector<faces> triangleFace;
        

        for (int i = 0; i < TriangleVertices.size(); i++)
        {
            glm::vec4 temp = model*cameraMatrix * (TriangleVertices[i].Position);
            glm::vec3 tempPos = glm::vec3(model *(TriangleVertices[i].Position));
            Vertices1.push_back(temp);
            crntPos.push_back(tempPos);
        }
      
        faces tempTrianlge;
        for (GLint i = 0; i < TriangleVertices.size(); i += 3)
        {
            glm::vec4 V1 = Vertices1[i];
            glm::vec4 V2 = Vertices1[i + 1];
            glm::vec4 V3 = Vertices1[i + 2];

            
            tempTrianlge.initialize(V1, V2, V3);
            triangleFace.push_back(tempTrianlge);
            
        }
        
        //std::cout << Vertices1[1].x << "\t" << Vertices1[1].y << "\t" << Vertices1[1].z << "\n";
        //std::cout << "V1.x = " << triangleFace[0].vertex2.x << "V1.y = " << triangleFace[0].vertex2.y << "V1.z = "  << triangleFace[0].vertex2.z<<"\n";
        //rasterizeByTriangle(Vertices1,crntPos);
        rasterizeByTriangle(triangleFace);

      /* for (int i = 0; i < TriangleVertices1.size(); i++)
        {
            glm::vec4 temp = model1 * cameraMatrix * glm::vec4(TriangleVertices1[i].Position, 1.0);
            Vertices2.push_back(temp);
        }*/

        
        //rasterize(temp1, temp2, temp3,glm::vec3(1,1,1));
        //rasterize(v1,v2,v3,glm::vec3(1,1,1));
        //triangle(temp1, temp2, temp3);
      
         

        //rasterize(Vertices1);
        //triangle(Vertices1);
        //triangle(v1,v2,v3);
        glDisable(GL_SMOOTH);
        glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex arrayattribute
        //glDisable(GL_POINT_SMOOTH); // stop the smoothing to make the points circular
       
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
       

    }
    
    glfwTerminate();
    
    return 0;
}


void putpixel(glm::vec2 pixel, glm::vec3 color)
{
    glPointSize(1); // must be added before glDrawArrays is called
    glColor3f(color.x, color.y, color.z);
    glVertexPointer(2, GL_FLOAT, 0, &pixel); // point to the vertices to be used
    glDrawArrays(GL_POINTS, 0, 1); // draw the vertixes
}

void putpixel_adjusted(glm::vec2 pixel, glm::vec3 color)
{
    pixel.x = pixel.x + SCREEN_WIDTH / 2;
    pixel.y = pixel.y + SCREEN_HEIGHT / 2;
    putpixel(pixel,color);
}


void line(float x1, float y1, float x2, float y2, glm::vec3 color)
{
    x1 = (int)round(x1);
    x2 = (int)round(x2);
    y1 = (int)round(y1);
    y2 = (int)round(y2);
    float dx, dy;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    //Sets increment/decrement : stepsize
    int lx, ly;

    if (x2 > x1) { lx = 1; }
    else { lx = -1; }

    if (y2 > y1) { ly = 1; }
    else { ly = -1; }
    //initialize
    int x = x1, y = y1;
    //slope<1
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int k = 0; k <= dx; k++) {
            putpixel_adjusted(glm::vec2(x,y), color);
            if (p < 0) {
                x += lx;
                p += 2 * dy;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    //if slope>=1
    else {
        int p = 2 * dx - dy;
        for (int k = 0; k <= dy; k++) {
            putpixel_adjusted(glm::vec2(x, y), color);
            if (p < 0) {
                y += ly;
                p += 2 * dx;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    putpixel_adjusted(glm::vec2(x, y), color);
}


void triangle(std::vector<glm::vec4> &vertices)
{
    for (GLint i = 0; i < vertices.size(); i+=3)
    {
        glm::vec4 v1 = vertices[i];
        glm::vec4 v2 = vertices[i+1];
        glm::vec4 v3 = vertices[i+2];


        bool backface = backfaceDetection(v1, v2, v3);

        if (backface == true)
        {
           /* line(((v1.x) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v1.y) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, ((v2.x) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v2.y) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            line(((v2.x) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v2.y) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, ((v3.x) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v3.y) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            line(((v1.x) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v1.y) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, ((v3.x) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v3.y) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            */

            line(((v1.x / v1.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v1.y / v1.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, ((v2.x / v2.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v2.y / v2.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            line(((v2.x / v2.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v2.y / v2.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, ((v3.x / v3.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v3.y / v3.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            line(((v1.x / v1.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v1.y / v1.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, ((v3.x / v3.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, ((v3.y / v3.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            
        }
    }


       
}


void triangle(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
    clear();

    line((v1.x / v1.w  + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, (v1.y / v1.w + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, (v2.x / v2.w + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, (v2.y / v2.w + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    line((v2.x / v2.w + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, (v2.y / v2.w + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, (v3.x / v3.w + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, (v3.y / v3.w + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    line((v1.x / v1.w + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, (v1.y / v1.w + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, (v3.x / v3.w + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, (v3.y / v3.w + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, glm::vec3(1.0f, 0.0f, 0.0f));
}

void rasterize(glm::vec4 V1, glm::vec4 V2, glm::vec4 V3, glm:: vec3 color)
{

    //clear();
    V1.x = ((V1.x / V1.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
    V1.y = ((V1.y / V1.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    V2.x = ((V2.x / V2.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
    V2.y = ((V2.y / V2.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    V3.x = ((V3.x / V3.w) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
    V3.y = ((V3.y / V3.w) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    
    /*V1.x = ((V1.x ) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
    V1.y = ((V1.y ) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    V2.x = ((V2.x ) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
    V2.y = ((V2.y ) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    V3.x = ((V3.x ) + 1) * SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2;
    V3.y = ((V3.y ) + 1) * SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2;
    */

    if (V1.y == V2.y && V2.y == V3.y) return;
    //Bubble sort on y-position
    if (V1.y > V2.y) { std::swap(V1, V2); }
    if (V1.y > V3.y) { std::swap(V1, V3); }
    if (V2.y > V3.y) { std::swap(V3, V2); }

    //divide triangle into two halves

    int height = V3.y - V1.y;

    for (int y = V1.y; y <= V2.y; y++)
    {
        int partialHeight = V2.y - V1.y + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - V1.y) / height;// be careful with divisions by zero 
        if (partialHeight != 0)
        {
            float beta = (float)(y - V1.y) / partialHeight;
            int Ax = (V1.x + (V3.x - V1.x) * alpha), Ay = V1.y + (V3.y - V1.y) * alpha;
            int Bx = V1.x + (V2.x - V1.x) * beta, By = V1.y + (V2.y - V1.y) * beta;
            if (Ax > Bx) { std::swap(Ax, Bx); }
            for (int j = Ax; j <= Bx; j++)
            {
                putpixel_adjusted(glm::vec2(j, y), color);
            }
            
        }

    }

    for (int y = V2.y; y <= V3.y; y++)
    {
        int partialHeight = V3.y - V2.y + 1; // +1 because both upper and lower limit is included

        float alpha = (float)(y - V1.y) / height;
        if (partialHeight != 0)
        {
            float beta = (float)(y - V2.y) / partialHeight; // be careful with divisions by zero 

            int Ax = V1.x + (V3.x - V1.x) * alpha, Ay = V1.y + (V3.y - V1.y) * alpha;
            int Bx = V2.x + (V3.x - V2.x) * beta, By = V2.y + (V3.y - V2.y) * beta;
            if (Ax > Bx) { std::swap(Ax, Bx); }
            for (int j = Ax; j <= Bx; j++)
            {
                putpixel_adjusted(glm::vec2(j, y), color);
            }
        }

    }
}

bool backfaceDetection(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
    glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(v2 - v1), glm::vec3(v3 - v1)));
    auto temp = glm::dot(normal, glm::vec3(v1));

    if (temp <= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}




void rasterize(std::vector<glm::vec4> vertices)
{

    for (GLint i = 0; i < TriangleVertices.size(); i += 3)
    {
        glm::vec4 V1 = vertices[i];
        glm::vec4 V2 = vertices[i+1];
        glm::vec4 V3 = vertices[i+2];

        glm::vec3 color = glm::vec3(1, 1, 1);
        
        bool backface = backfaceDetection(V1,V2,V3);

        if(backface==true)
        { 
            rasterize(V1, V2, V3, color);

        }
         
       
    }
}

/*void draw_bresenham_adjusted(const int& x1, const int& y1, const int& x2,
    const int& y2, const glm::vec3& color = glm::vec3(1, 0, 0)) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int lx = x2 > x1 ? 1 : -1;
    int ly = y2 > y1 ? 1 : -1;

    int x = x1, y = y1;
    bool changed = false;

    if (dx <= dy) {
        changed = true;
        std::swap(dx, dy);
        std::swap(lx, ly);
        std::swap(x, y);
    }
    int p = 2 * dy - dx;
    for (int k = 0; k <= dx; k++) {
        if (!changed)
            putpixel_adjusted(glm::vec2(x, y), color);
        else
            putpixel_adjusted(glm::vec2(y, x), color);

        if (p < 0) {
            x += lx;
            p += 2 * dy;
        }
        else {
            x += lx;
            y += ly;
            p += 2 * dy - 2 * dx;
        }
    }
}*/

void sortTriangles()
{

}

void fillBottomFlatTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3,
     const glm::vec3& color) {
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float currentx1 = v1.x;
    float currentx2 = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
        //draw_bresenham_adjusted((int)currentx1, scanlineY, (int)currentx2, scanlineY,  color);
        line(currentx1,scanlineY,currentx2,scanlineY,color);
        currentx1 += invslope1;
        currentx2 += invslope2;
    }
}

void fillTopFlatTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3,
     const glm::vec3& color) {
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float currentx1 = v3.x;
    float currentx2 = v3.x;

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
        //draw_bresenham_adjusted((int)currentx1, scanlineY, (int)currentx2,scanlineY, color);
        
        line(currentx1, scanlineY, currentx2, scanlineY, color);
        currentx1 -= invslope1;
        currentx2 -= invslope2;
    }
}
glm::vec2 interpolate(const glm::vec2& src, const glm::vec2& dst, float alpha) {
    return src + (dst - src) * alpha;
}
bool sortcol(const glm::vec2& v1, const glm::vec2& v2) { return v1.y < v2.y; }

void rasterizeByTriangle(std::vector<glm::vec4> vertices, std::vector<glm::vec3>crntPos)
{
    for (int i = 0; i < vertices.size(); i += 3) {
        /* subtracting 1 because indices are 1 indexd not zero indexed */
        glm::vec4 vertex1 = vertices[i];
        glm::vec4 vertex2 = vertices[i + 1];
        glm::vec4 vertex3 = vertices[i + 2];

        

        /* this if condition is completely for testing purpose, it helps to
         * check how each traingle are drawn */
        int filter = 1;
        if (filter)
        {

            /* camera vanda paxadi paro vane aile lai puraai traingle nai
             * display nagarne */
            if (vertex1.w < 0.1 || vertex2.w < 0.1 || vertex3.w < 0.1) {
                continue;
            }

            glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertex2 - vertex1), glm::vec3(vertex3 - vertex1)));

            /* calculate the normal here and if the normal and camera
             * direction dot product gives positive the trangle should not
             * be drawn */
            auto temp = glm::dot(normal, glm::vec3(vertex1));
            if (temp <= 0) {
                continue;
            }

            
            assert(vertex1.w != 0 and vertex2.w != 0 and vertex3.w != 0);

            float window_width = 800;
            float window_height = 800;

            std::vector<glm::vec2> traingle = std::vector<glm::vec2>(3);
            traingle[0] = glm::vec2((int)round(((vertex1.x / vertex1.w) + 1) *
                window_width / 2 -
                window_width / 2),
                (int)round(((vertex1.y / vertex1.w) + 1) *
                    window_height / 2 -
                    window_height / 2));

            traingle[1] = glm::vec2((int)round(((vertex2.x / vertex2.w) + 1) *
                window_width / 2 -
                window_width / 2),
                (int)round(((vertex2.y / vertex2.w) + 1) *
                    window_height / 2 -
                    window_height / 2));

            traingle[2] = glm::vec2((int)round(((vertex3.x / vertex3.w) + 1) *
                window_width / 2 -
                window_width / 2),
                (int)round(((vertex3.y / vertex3.w) + 1) *
                    window_height / 2 -
                    window_height / 2));

            auto color = glm::vec3(1.0f, 1.0f, 1.0f);
            auto lightPos = glm::vec3(5,5,5);
            auto lightDir = glm::normalize(glm::vec3(vertex1) - lightPos);
        
            float intensity = glm::max(glm::dot(lightDir , normal),0.0f);
            color = color * intensity;
            
            sort(traingle.begin(), traingle.end(), sortcol);
            if (traingle[0].y == traingle[1].y) // natural flat top
            {
                // sorting top vertices by x
                if (traingle[1].x < traingle[0].x)
                    std::swap(traingle[0], traingle[1]);

                fillTopFlatTriangle(traingle[0], traingle[1], traingle[2],
                    color);
            }
            else if (traingle[1].y ==
                traingle[2].y) // natural flat bottom
            {
                // sorting bottom vertices by x
                if (traingle[2].x < traingle[1].x)
                    std::swap(traingle[1], traingle[2]);

                fillBottomFlatTriangle(traingle[0], traingle[1],
                    traingle[2], color);
            }
            else // general triangle
            {
                // find splitting vertex interpolant
                const float alphaSplit = (traingle[1].y - traingle[0].y) /
                    (traingle[2].y - traingle[0].y);
                const auto vi = interpolate(traingle[0],
                    traingle[2], alphaSplit);

                if (traingle[1].x < vi.x) // major right
                {
                    fillBottomFlatTriangle(traingle[0], traingle[1], vi,
                         color);
                    fillTopFlatTriangle(traingle[1], vi, traingle[2],
                         color);
                }
                else // major leftzValue,
                {
                    fillBottomFlatTriangle(traingle[0], vi, traingle[1],
                         color);
                    fillTopFlatTriangle(vi, traingle[1], traingle[2],
                         color);
                }
            }
        }
    }
}

void rasterizeByTriangle(std::vector<faces> trianglefaces)
{
    for (int i = 0; i < trianglefaces.size(); i++)
    {
        int filter = 1;
        if (filter)
        {

            /* camera vanda paxadi paro vane aile lai puraai traingle nai
             * display nagarne */
            if (trianglefaces[i].vertex1.w < 0.1 || trianglefaces[i].vertex2.w < 0.1 || trianglefaces[i].vertex3.w < 0.1) {
                continue;
            }

            glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(trianglefaces[i].vertex2 - trianglefaces[i].vertex1), glm::vec3(trianglefaces[i].vertex3 - trianglefaces[i].vertex1)));

            /* calculate the normal here and if the normal and camera
             * direction dot product gives positive the trangle should not
             * be drawn */
            auto temp = glm::dot(normal, glm::vec3(trianglefaces[i].vertex1));
            if (temp <= 0) {
                continue;
            }


            assert(trianglefaces[i].vertex1.w != 0 and trianglefaces[i].vertex2.w != 0 and trianglefaces[i].vertex3.w != 0);

            float window_width = 800;
            float window_height = 800;

            std::vector<glm::vec2> traingle = std::vector<glm::vec2>(3);
            traingle[0] = glm::vec2((int)round(((trianglefaces[i].vertex1.x / trianglefaces[i].vertex1.w) + 1) *
                window_width / 2 -
                window_width / 2),
                (int)round(((trianglefaces[i].vertex1.y / trianglefaces[i].vertex1.w) + 1) *
                    window_height / 2 -
                    window_height / 2));

            traingle[1] = glm::vec2((int)round(((trianglefaces[i].vertex2.x / trianglefaces[i].vertex2.w) + 1) *
                window_width / 2 -
                window_width / 2),
                (int)round(((trianglefaces[i].vertex2.y / trianglefaces[i].vertex2.w) + 1) *
                    window_height / 2 -
                    window_height / 2));

            traingle[2] = glm::vec2((int)round(((trianglefaces[i].vertex3.x / trianglefaces[i].vertex3.w) + 1) *
                window_width / 2 -
                window_width / 2),
                (int)round(((trianglefaces[i].vertex3.y / trianglefaces[i].vertex3.w) + 1) *
                    window_height / 2 -
                    window_height / 2));

            auto color = glm::vec3(0.0f, 1.0f, 1.0f);
            auto lightPos = glm::vec3(0, 0, 0);
            auto lightDir = glm::normalize(glm::vec3(trianglefaces[i].vertex1) - lightPos);

            float intensity = glm::max(glm::dot(lightDir, normal), 0.0f);
            color = color * intensity;

            sort(traingle.begin(), traingle.end(), sortcol);
            if (traingle[0].y == traingle[1].y) // natural flat top
            {
                // sorting top vertices by x
                if (traingle[1].x < traingle[0].x)
                    std::swap(traingle[0], traingle[1]);

                fillTopFlatTriangle(traingle[0], traingle[1], traingle[2],
                    color);
            }
            else if (traingle[1].y ==
                traingle[2].y) // natural flat bottom
            {
                // sorting bottom vertices by x
                if (traingle[2].x < traingle[1].x)
                    std::swap(traingle[1], traingle[2]);

                fillBottomFlatTriangle(traingle[0], traingle[1],
                    traingle[2], color);
            }
            else // general triangle
            {
                // find splitting vertex interpolant
                const float alphaSplit = (traingle[1].y - traingle[0].y) /
                    (traingle[2].y - traingle[0].y);
                const auto vi = interpolate(traingle[0],
                    traingle[2], alphaSplit);

                if (traingle[1].x < vi.x) // major right
                {
                    fillBottomFlatTriangle(traingle[0], traingle[1], vi,
                        color);
                    fillTopFlatTriangle(traingle[1], vi, traingle[2],
                        color);
                }
                else // major leftzValue,
                {
                    fillBottomFlatTriangle(traingle[0], vi, traingle[1],
                        color);
                    fillTopFlatTriangle(vi, traingle[1], traingle[2],
                        color);
                }
            }
        }
    }
}