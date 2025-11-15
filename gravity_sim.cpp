#include <iostream>
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>

const float G = 20000; // gravitational constant
GLFWwindow* StartGLFW();

class Object {
    public:
    std::vector<float> position;
    std::vector<float> velocity;
    float radius;
    float mass;
    
    Object(std::vector<float> position, std::vector<float> velocity, float radius = 25.0f, float mass = 10.0f) {
        this->position = position;
        this->velocity = velocity;
        this->radius = radius;
        this->mass = mass;
    }
    
    void accelerate(float x, float y){
        this->velocity[0] += x;
        this->velocity[1] += y;
    }
    
    void updatePos(float dt){
        this->position[0] += this->velocity[0] * dt;
        this->position[1] += this->velocity[1] * dt;
    }
    
    void DrawCircle(float cx, float cy, float r, int segments) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * M_PI * (static_cast<float>(i) / segments);
            float x = cx + cos(angle) * r;
            float y = cy + sin(angle) * r;
            glVertex2f(x, y);
        }
        glEnd();
    }
};

float screenWidth = 1000.0f;
float screenHeight = 1000.0f;

int main() {
    GLFWwindow * window = StartGLFW();
    if (!window) return -1;
    
    glfwMakeContextCurrent(window);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    std::vector<Object> objects = {
        Object(std::vector<float>{300.0f, 800.0f}, std::vector<float>{900.0f, 0.0f}),
        Object(std::vector<float>{800.0f, 400.0f}, std::vector<float>{-900.0f, 0.0f}),
    };
    
    int segments = 100;
    double lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {    
        // Calculate delta time
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        
        
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        for(auto& obj : objects) {
            
            for(auto& obj2: objects) {
                if (&obj == &obj2) {continue;};
                float dx = obj2.position[0] - obj.position[0];
                float dy = obj2.position[1] - obj.position[1];
                float distance = sqrt(dx * dx + dy * dy);
                std::vector<float> direction = {dx/distance, dy/distance};

                float Gforce = (G * obj.mass * obj2.mass) / (distance * distance);
                float acc1 = Gforce / obj.mass;

                std::vector<float> acc = {acc1 * direction[0] ,acc1 * direction[1] };
                obj.accelerate(acc[0], acc[1]);
            }
            obj.updatePos(dt);
            
            obj.DrawCircle(obj.position[0], obj.position[1], obj.radius, segments);
            
            // if(obj.position[1] < 0 || obj.position[1] > screenHeight) {
            //     obj.velocity[1] *= -1;
            // }
            // if(obj.position[0] < 0 || obj.position[0] > screenWidth) {
            //     obj.velocity[0] *= -1;
            // }
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cerr << "failed to initialize glfw\n";
        return nullptr;
    }
    return glfwCreateWindow(1000, 1000, "gravity_simulation", NULL, NULL);
}
