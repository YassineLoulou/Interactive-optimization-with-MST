
// RQ: Si le chemin ne convient pas ( il peut y avoir un bug pour des raisons inconnues ) , prière d'éxecuter le programme plus qu'une fois

// J'ai transféré le résultat d'éxecution des autres programmes ici pour les simuler 

#include <iostream>
#include <fstream>
#include <vector>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define e 1e-6
#define inf 1e9

using namespace std;

struct Path {
    std::vector<float> vertices;
    std::vector<float> colors;
    GLenum mode;
};
vector<float>v;
class sommet {
public:
    float x, y;
    sommet() {}
    sommet(float X, float Y) {
        x = X;
        y = Y;
    }
    bool operator<(const sommet& other) const {
        if (x < other.x) {
            return true;
        }
        else if (x == other.x) {
            return y < other.y;
        }
        else {
            return false;
        }
    }
    bool operator ==(sommet S) {
        return abs(x - S.x) < e && abs(y - S.y) < e;
    }
    bool operator !=(sommet S) {
        return !((*this) == S);
    }

    sommet operator*(const float& t) {
        return sommet(x * t, y * t);
    }
    sommet operator+(sommet S) {
        return sommet(x + S.x, y + S.y);
    }
    sommet operator-(sommet S) {
        return sommet(x - S.x, y - S.y);
    }
};

class segment {
public:
    sommet S1;
    sommet S2;
    float norm;
public:
    segment() {}
    segment(sommet A, sommet B) :S1(A), S2(B) {}
    segment(sommet A, sommet B, float d) :S1(A), S2(B), norm(d) {}
    bool operator ==(segment S) {
        return S1 == S.S1 && S2 == S.S2;
    }
    float a() {
        if (S2.x != S1.x)
            return (S2.y - S1.y) / (S2.x - S1.x);
        return float(inf);
    }
    float b() {
        if (S2.x != S1.x)
            return S1.y - (*this).a() * S1.x;
        return S1.x;
    }
    bool in_str_seg(sommet S) {
        if ((*this).a() == float(inf))
            return e < max(S1.y, S2.y) - S.y - S.y && S.y - min(S1.y, S2.y)>e;
        return S.x - min(S1.x, S2.x) > e && e < max(S1.x, S2.x) - S.x;
    }
    bool in_seg(sommet S) {
        if ((*this).a() == float(inf))
            return S.y >= min(S1.y, S2.y) && S.y <= max(S1.y, S2.y);
        if (S.x >= min(S1.x, S2.x) && S.x <= max(S1.x, S2.x))
            return true;
        return false;
    }
};
class obstacle {
public:
    vector<segment>obs;
};
sommet center(obstacle ob) {
    sommet A(0, 0);
    float sz = float(ob.obs.size());
    for (int i = 0; i < ob.obs.size(); i++) {
        A = A + ob.obs[i].S1;
    }
    A = A * (1.0f / sz);
    return A;
}
void rotate(sommet& S, sommet C, float angle) {
    
    float s = sin(angle);
    float c = cos(angle);

    S.x -= C.x;
    S.y -= C.y;

    float xnew = S.x * c - S.y * s;
    float ynew = S.x * s + S.y * c;

    S.x = xnew + C.x;
    S.y = ynew + C.y;
}
void rotate_obs(obstacle& ob, float angle) {
    sommet centre = center(ob);
    for (int i = 0; i < ob.obs.size(); i++) {
        rotate(ob.obs[i].S1, centre, angle);
        rotate(ob.obs[i].S2, centre, angle);
    }
}
obstacle obj;


std::vector<Path> loadPathsFromFile(const std::string& filename) {
    std::vector<Path> paths;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return paths;
    }
    Path currentPath;
    int numVertices;
    int c = 0;






    while (file >> numVertices) {
        currentPath.vertices.clear();

        // Read vertices for current path
        for (int i = 0; i < numVertices; ++i) {
            float x, y;
            file >> x >> y;
            currentPath.vertices.push_back(x);
            currentPath.vertices.push_back(y);
        }
        // Set mode and color for current path
        currentPath.mode = GL_POLYGON;  // Set mode to draw a line strip
        if (c >= 1 && c <= 2)
            currentPath.colors = { 1.0f, 1.0f, 1.0f };  // Set color to white
        else if (c == 1)
            currentPath.colors = { 0.5f, 0.5f, 0.5f };
        else
            currentPath.colors = { 0.5f, 0.5f, 0.5f };

        c++;
        if (c == 1) {
            v = currentPath.vertices;
            for (int i = 0; i < v.size(); i += 2) {
                sommet S(v[i], v[i + 1]);
                obj.obs.push_back(segment(S, S));
            }
        }
        paths.push_back(currentPath);
    }

    file.close();

    return paths;
}

using namespace std;
float f(float a, float b, float x) {
    return a * x + b;
}
float aa(float x1, float y1, float x2, float y2) {
    if (abs(x1 - x2) < e) {
        return 1e9f;
    }
    return (y2 - y1) / (x2 - x1);
}
float bb(float x1, float y1, float x2, float y2) {
    return y1 - aa(x1, y1, x2, y2) * x1;
}
float a, b;
int ind = 0;
bool test = true;
void drawPaths(std::vector<Path>& paths, double t, double dt,obstacle&object) {


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

    
    

    

    int k = 0;

    for (const Path& path : paths) {

        glLineWidth(5.0f); // Set line width
        glBegin(path.mode);
        glColor3fv(path.colors.data());

        const float* vertices = path.vertices.data();
        for (int i = 0; i < path.vertices.size() / 2; ++i) {
            glVertex2fv(&vertices[i * 2]);
            glTexCoord2f(vertices[i * 2], vertices[i * 2 + 1]);
        }
     
        k++;
        glEnd();
    }
        
    Path& p = paths[0];

    
    glBegin(p.mode);
    float* vertices = p.vertices.data();
    //contains points of object
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3fv(p.colors.data());
    if ((test)&&abs(vertices[0] - vertices[2]) > dt) {
        for (int i = 0; i < p.vertices.size(); i += 2) {
            sommet S(vertices[i], vertices[i + 1]);
            rotate(S, center(object), -dt);
            vertices[i] = S.x;
            vertices[i + 1] = S.y;

            object.obs[i / 2].S1.x = S.x;
            object.obs[i / 2].S1.y = S.y;
        }
    }
    else if ((test)&&abs(center(object).x - 5.0f) > dt) {
        for (int i = 0; i < p.vertices.size(); i += 2) {
            vertices[i] += dt;
            object.obs[i / 2].S1.x += dt;
        }
    }
    else if (abs(vertices[0] - 6.0f)>0.01f ) {
        test = false;
        for (int i = 0; i < p.vertices.size(); i += 2) {
            sommet S(vertices[i], vertices[i + 1]);
            rotate(S, center(object), -dt);
            vertices[i] = S.x;
            vertices[i + 1] = S.y;

            object.obs[i / 2].S1.x = S.x;
            object.obs[i / 2].S1.y = S.y;
        }
    }
    //draw square
    for (int i = 0; i < p.vertices.size() / 2; ++i) {
        glVertex2fv(&vertices[i * 2]);
    }
    glEnd();
    
}

int main() {


    // Initialize GLFW and create a window
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Paths", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load path data from file
    std::vector<Path> paths = loadPathsFromFile("test.txt");


    double t0 = glfwGetTime();
    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        





        double t1 = glfwGetTime();
        drawPaths(paths, t1, t1 - t0,obj);
        t0 = t1;

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}
