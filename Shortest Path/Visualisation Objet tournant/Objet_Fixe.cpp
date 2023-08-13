#include <iostream>
#include <fstream>
#include <vector>
#include <GLFW/glfw3.h>
#define e 1e-6
using namespace std;
struct Path {
    std::vector<float> vertices;
    std::vector<float> colors;
    GLenum mode;
};
std::vector<float>v;
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
        if (c >= 2 && c <= 4)
            currentPath.colors = { 1.0f, 1.0f, 1.0f };  // Set color to white
        else if (c == 1)
            currentPath.colors = { 0.5f, 0.5f, 0.5f };
        else
            currentPath.colors = { 0.5f, 0.5f, 0.5f };

        c++;
        if (c == 1) {
            v = currentPath.vertices;
        }
        else paths.push_back(currentPath);
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
void drawPaths(std::vector<Path>& paths, double t, double dt) {


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 10.0, -1.0, 10.0, -1.0, 1.0);


    for (const Path& path : paths) {



        glLineWidth(5.0f); // Set line width
        glBegin(path.mode);
        glColor3fv(path.colors.data());

        const float* vertices = path.vertices.data();
        for (int i = 0; i < path.vertices.size() / 2; ++i) {
            glVertex2fv(&vertices[i * 2]);
        }
        glEnd();
    }

    Path& p = paths[0];
    glBegin(p.mode);
    float* vertices = p.vertices.data();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3fv(p.colors.data());
    if (2 * ind <= v.size() - 2) {
        if (abs(vertices[0] - 0.5 - v[2 * ind]) < dt && abs(vertices[1] - v[2 * ind + 1]) < dt)
        {
            a = aa(v[2 * ind], v[2 * ind + 1], v[2 * (ind + 1)], v[2 * (ind + 1) + 1]);
            b = bb(v[2 * ind], v[2 * ind + 1], v[2 * (ind + 1)], v[2 * (ind + 1) + 1]);
            cout << a << " " << b << endl;
            ind++;
        }
        //change losange position

        for (int i = 0; i < p.vertices.size(); i += 2) {

            if (abs(a - 1e9f) > e) {
                vertices[i] += dt;
                vertices[i + 1] = a * dt + vertices[i + 1];
            }
            else
                vertices[i + 1] -= dt;
        }
        cout << endl << endl;
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
        drawPaths(paths, t1, t1 - t0);
        t0 = t1;

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}
