#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Struct to represent a circle
struct Circle {
    float x, y, radius;
    float speedX, speedY;
};

// Function to calculate the distance between two circles
float distance(const Circle& c1, const Circle& c2) {
    return std::sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y));
}

// Function to handle circle collision response
void handleCollision(Circle& c1, Circle& c2) {
    float m1 = c1.radius; // Assume mass is proportional to radius
    float m2 = c2.radius;

    float new_v1x = (c1.speedX * (m1 - m2) + 2 * m2 * c2.speedX) / (m1 + m2);
    float new_v1y = (c1.speedY * (m1 - m2) + 2 * m2 * c2.speedY) / (m1 + m2);
    float new_v2x = (c2.speedX * (m2 - m1) + 2 * m1 * c1.speedX) / (m1 + m2);
    float new_v2y = (c2.speedY * (m2 - m1) + 2 * m1 * c1.speedY) / (m1 + m2);

    c1.speedX = new_v1x;
    c1.speedY = new_v1y;
    c2.speedX = new_v2x;
    c2.speedY = new_v2y;
}

// Key callback function to close the window when Escape is pressed
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "Bouncing Circles with Gravity", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);

    // Initialize two circles
    Circle c1 = {-0.5f, 0.5f, 0.1f, 0.02f, 0.0f};
    Circle c2 = {0.5f, 0.5f, 0.1f, -0.01f, 0.0f};

    const float gravity = -0.0005f; // Gravity constant

    while (!glfwWindowShouldClose(window)) {
        // Apply gravity
        c1.speedY += gravity;
        c2.speedY += gravity;

        // Update positions
        c1.x += c1.speedX;
        c1.y += c1.speedY;
        c2.x += c2.speedX;
        c2.y += c2.speedY;

        // Check for wall collisions
        if (c1.x + c1.radius > 1.0f || c1.x - c1.radius < -1.0f) c1.speedX = -c1.speedX;
        if (c1.y - c1.radius < -1.0f) { // Bottom collision
            c1.speedY = -c1.speedY * 0.9f; // Add damping to simulate energy loss
            c1.y = -1.0f + c1.radius; // Prevent sinking below the floor
        }
        if (c1.y + c1.radius > 1.0f) c1.speedY = -c1.speedY;

        if (c2.x + c2.radius > 1.0f || c2.x - c2.radius < -1.0f) c2.speedX = -c2.speedX;
        if (c2.y - c2.radius < -1.0f) { // Bottom collision
            c2.speedY = -c2.speedY * 0.9f; // Add damping to simulate energy loss
            c2.y = -1.0f + c2.radius; // Prevent sinking below the floor
        }
        if (c2.y + c2.radius > 1.0f) c2.speedY = -c2.speedY;

        // Check for circle collisions
        if (distance(c1, c2) < c1.radius + c2.radius) {
            handleCollision(c1, c2);
        }

        // Render circles
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.0f, 0.0f); // Red for circle 1
        for (int i = 0; i < 100; ++i) {
            float angle = 2.0f * 3.1415926f * i / 100;
            float x = c1.radius * cos(angle);
            float y = c1.radius * sin(angle);
            glVertex2f(c1.x + x, c1.y + y);
        }
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue for circle 2
        for (int i = 0; i < 100; ++i) {
            float angle = 2.0f * 3.1415926f * i / 100;
            float x = c2.radius * cos(angle);
            float y = c2.radius * sin(angle);
            glVertex2f(c2.x + x, c2.y + y);
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
