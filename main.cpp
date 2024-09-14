#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <sstream>

const int NUM_CLOUDS = 5;
const float ANIMATION_SPEED = 0.006f; // Slower speed of animation
const float LINE_WIDTH = 2.0f;
const float DOT_RADIUS = 0.02f; // Radius of the moving dot

// Utility structure for handling 2D vectors (positions, velocities, etc.)
struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
};

// Structure to represent a network node (router, server, computer)
struct Node {
    Vec2 position;
    float radius;
    Vec2 color;
    Node(float x, float y, float r, Vec2 c) : position(x, y), radius(r), color(c) {}
};

// Structure to represent a cloud
struct Cloud {
    Vec2 position;
    float speed;
    Cloud(float x, float y, float s) : position(x, y), speed(s) {}
};

// List of nodes and clouds
std::vector<Node> nodes;
std::vector<Cloud> clouds;
float dotPosition = 0.0f; // Position of the moving dot on the line

// Draw a node
void drawNode(const Node& node) {
    if (node.color.x == 1.0f && node.color.y == 1.0f) {
        // Server: Draw a white rectangle
        glColor3f(node.color.x, node.color.y, node.color.y);
        glBegin(GL_QUADS);
        glVertex2f(node.position.x - node.radius, node.position.y - node.radius * 0.5);
        glVertex2f(node.position.x + node.radius, node.position.y - node.radius * 0.5);
        glVertex2f(node.position.x + node.radius, node.position.y + node.radius * 0.5);
        glVertex2f(node.position.x - node.radius, node.position.y + node.radius * 0.5);
        glEnd();
    } else if (node.color.x == 0.0f && node.color.y == 1.0f) {
        // Router: Draw a green triangle
        glColor3f(node.color.x, node.color.y, 0.0f); // Green for router
        glBegin(GL_TRIANGLES);
        glVertex2f(node.position.x, node.position.y + node.radius); // Top
        glVertex2f(node.position.x - node.radius, node.position.y - node.radius); // Bottom-left
        glVertex2f(node.position.x + node.radius, node.position.y - node.radius); // Bottom-right
        glEnd();
    } else if (node.color.x == 1.0f && node.color.y == 0.0f) {
        // Computer: Draw a yellow circle
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for computer
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(node.position.x, node.position.y); // Center
        int num_segments = 360; // Number of segments to approximate the circle
        for (int i = 0; i <= num_segments; ++i) {
            float theta = i * 2.0f * M_PI / num_segments;
            float dx = node.radius * cos(theta);
            float dy = node.radius * sin(theta);
            glVertex2f(node.position.x + dx, node.position.y + dy);
        }
        glEnd();
    }
}

// Initialize nodes, keeping the topology centered
void initializeNodes() {
    float yOffset = 0.0f;  // Centralize topology vertically
    float xOffset = 0.0f;  // Centralize horizontally

    // Server in the top center
    nodes.push_back(Node(xOffset - 0.5f, yOffset + 0.5f, 0.1f, Vec2(1.0f, 1.0f))); // Server (white)

    // Routers
    nodes.push_back(Node(xOffset - 0.3f, yOffset + 0.2f, 0.1f, Vec2(0.0f, 1.0f))); // Router 1 (green)
    nodes.push_back(Node(xOffset + 0.3f, yOffset + 0.2f, 0.1f, Vec2(0.0f, 1.0f))); // Router 2 (green)

    // Computers connected to Router 1
    nodes.push_back(Node(xOffset - 0.5f, yOffset - 0.2f, 0.07f, Vec2(1.0f, 0.0f))); // Computer 1 (yellow)
    nodes.push_back(Node(xOffset - 0.2f, yOffset - 0.2f, 0.07f, Vec2(1.0f, 0.0f))); // Computer 2 (yellow)
    nodes.push_back(Node(xOffset - 0.35f, yOffset - 0.4f, 0.07f, Vec2(1.0f, 0.0f))); // Computer 3 (yellow)

    // Computers connected to Router 2
    nodes.push_back(Node(xOffset + 0.1f, yOffset - 0.2f, 0.07f, Vec2(1.0f, 0.0f))); // Computer 4 (yellow)
    nodes.push_back(Node(xOffset + 0.4f, yOffset - 0.2f, 0.07f, Vec2(1.0f, 0.0f))); // Computer 5 (yellow)
    nodes.push_back(Node(xOffset + 0.25f, yOffset - 0.4f, 0.07f, Vec2(1.0f, 0.0f))); // Computer 6 (yellow)
}

// Check if the distance between two clouds is less than a specified minimum distance
bool isTooClose(const Cloud& cloud1, const Cloud& cloud2, float minDistance) {
    float dx = cloud1.position.x - cloud2.position.x;
    float dy = cloud1.position.y - cloud2.position.y;
    return (dx * dx + dy * dy) < (minDistance * minDistance);
}

// Initialize clouds
void initializeClouds() {
    srand(static_cast<unsigned>(time(0)));
    const float MIN_DISTANCE = 0.2f; // Minimum distance between clouds

    clouds.clear(); // Clear any existing clouds

    for (int i = 0; i < NUM_CLOUDS; ++i) {
        Cloud newCloud(0.0f, 0.0f, 0.005f);
        bool positionValid = false;

        while (!positionValid) {
            newCloud.position.x = (rand() % 200 - 100) / 100.0f;
            newCloud.position.y = (rand() % 200 - 100) / 100.0f;
            newCloud.speed = 0.005f; // Slower speed

            // Check if the new cloud is too close to any existing cloud
            positionValid = true;
            for (const auto& cloud : clouds) {
                if (isTooClose(newCloud, cloud, MIN_DISTANCE)) {
                    positionValid = false;
                    break;
                }
            }
        }

        clouds.push_back(newCloud);
    }
}

// Draw a line connecting two nodes
void drawConnection(const Node& from, const Node& to) {
    glColor3f(1.0f, 1.0f, 1.0f); // White for connection lines
    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);
    glVertex2f(from.position.x, from.position.y);
    glVertex2f(to.position.x, to.position.y);
    glEnd();
}

// Draw a dot on the connection line
void drawDotOnConnection(const Node& from, const Node& to, float position) {
    Vec2 direction(to.position.x - from.position.x, to.position.y - from.position.y);
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    // Dot position calculation
    Vec2 dotPos(from.position.x + position * direction.x, from.position.y + position * direction.y);

    // Ensure dot stays within the bounds of the line
    float dotToFromLength = sqrt(pow(dotPos.x - from.position.x, 2) + pow(dotPos.y - from.position.y, 2));
    float dotToToLength = sqrt(pow(dotPos.x - to.position.x, 2) + pow(dotPos.y - to.position.y, 2));
    float lineLength = sqrt(pow(to.position.x - from.position.x, 2) + pow(to.position.y - from.position.y, 2));

    if (dotToFromLength > lineLength) {
        dotPos = from.position;
    } else if (dotToToLength > lineLength) {
        dotPos = to.position;
    }

    glColor3f(1.0f, 0.0f, 0.0f); // Red dot
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(dotPos.x, dotPos.y);
    for (int i = 0; i <= 360; i += 30) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(dotPos.x + DOT_RADIUS * cos(theta), dotPos.y + DOT_RADIUS * sin(theta));
    }
    glEnd();
}

// Draw clouds
void drawCloud(const Cloud& cloud) {
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray for clouds
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cloud.position.x, cloud.position.y);
    for (int i = 0; i <= 360; i += 30) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(cloud.position.x + 0.1f * cos(theta), cloud.position.y + 0.05f * sin(theta));
    }
    glEnd();
}

// Update positions of clouds
void updateClouds() {
    for (auto& cloud : clouds) {
        cloud.position.x += cloud.speed;
        if (cloud.position.x > 1.0f) {
            cloud.position.x = -1.0f;
        }
    }
}

// Update the position of the moving dot along the connection
void updateDotPosition() {
    dotPosition += ANIMATION_SPEED;
    if (dotPosition > 1.0f) {
        dotPosition = 0.0f;
    }
}

// Function to render text at a specific position (supports multi-line)
void renderText(float x, float y, const char* text) {
    glColor3f(1.0f, 1.0f, 1.0f); // Set text color (white)

    // Split the input text by newline characters and render each line separately
    std::string str(text);
    std::stringstream ss(str);
    std::string line;

    float lineSpacing = 0.06f; // Adjust this to set the space between lines

    while (std::getline(ss, line, '\n')) {
        glRasterPos2f(x, y); // Set position for the text
        for (const char* c = line.c_str(); *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Render each character in the line
        }
        y -= lineSpacing; // Move to the next line (decrease y-coordinate)
    }
}

// Utility function to render text at the center of the node
void renderTextInShape(float x, float y, const char* text) {
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the text to contrast the node colors
    glRasterPos2f(x, y); // Set position for the text
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Render each character
    }
}

// Draw a node and its label
void drawNodeWithLabel(const Node& node, const char* label) {
    // Draw the node based on its type
    drawNode(node);

    // Calculate the appropriate position for the text inside the shape
    if (node.color.x == 1.0f && node.color.y == 1.0f) {
        // Server: Draw label inside the white rectangle
        renderTextInShape(node.position.x - 0.07f, node.position.y - 0.02f, label);
    } else if (node.color.x == 0.0f && node.color.y == 1.0f) {
        // Router: Draw label inside the green triangle
        renderTextInShape(node.position.x - 0.04f, node.position.y - 0.03f, label);
    } else if (node.color.x == 1.0f && node.color.y == 0.0f) {
        // Computer: Draw label inside the yellow circle
        renderTextInShape(node.position.x - 0.03f, node.position.y - 0.02f, label);
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clouds
    for (const auto& cloud : clouds) {
        drawCloud(cloud);
    }

    // Draw connections between server, routers, and computers
    drawConnection(nodes[0], nodes[1]); // Server to Router 1
    drawDotOnConnection(nodes[0], nodes[1], dotPosition);

    drawConnection(nodes[0], nodes[2]); // Server to Router 2
    drawDotOnConnection(nodes[0], nodes[2], dotPosition);

    drawConnection(nodes[1], nodes[3]); // Router 1 to Computer 1
    drawDotOnConnection(nodes[1], nodes[3], dotPosition);

    drawConnection(nodes[1], nodes[4]); // Router 1 to Computer 2
    drawDotOnConnection(nodes[1], nodes[4], dotPosition);

    drawConnection(nodes[1], nodes[5]); // Router 1 to Computer 3
    drawDotOnConnection(nodes[1], nodes[5], dotPosition);

    drawConnection(nodes[2], nodes[6]); // Router 2 to Computer 4
    drawDotOnConnection(nodes[2], nodes[6], dotPosition);

    drawConnection(nodes[2], nodes[7]); // Router 2 to Computer 5
    drawDotOnConnection(nodes[2], nodes[7], dotPosition);

    drawConnection(nodes[2], nodes[8]); // Router 2 to Computer 6
    drawDotOnConnection(nodes[2], nodes[8], dotPosition);

    // Draw nodes
    for (const auto& node : nodes) {
        drawNode(node);
    }

    // Draw nodes with labels
    drawNodeWithLabel(nodes[0], "Server0"); // Server
    drawNodeWithLabel(nodes[1], "Router0"); // Router 1
    drawNodeWithLabel(nodes[2], "Router1"); // Router 2

    // Computers
    drawNodeWithLabel(nodes[3], "PC0"); // Computer 1
    drawNodeWithLabel(nodes[4], "PC1"); // Computer 2
    drawNodeWithLabel(nodes[5], "PC2"); // Computer 3
    drawNodeWithLabel(nodes[6], "PC3"); // Computer 4
    drawNodeWithLabel(nodes[7], "PC4"); // Computer 5
    drawNodeWithLabel(nodes[8], "PC5"); // Computer 6

    // Render some text on the screen
    renderText(-0.9f, 0.9f, "Welcome to Animated Network Topology\nCISCO Look-alike System\nComputer Graphics Project | CSE 322\nSamina Rahman - 221006012\nAbtahi Md. Mahib Uddin - 221004112");

    glutSwapBuffers();
}

// Timer function for animation
void timer(int value) {
    updateClouds();
    updateDotPosition();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Call timer function every 16ms (~60 FPS)
}

// Initialize OpenGL settings
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input handler
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0); // Exit the program
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // glutInitWindowSize(800, 600);

    printf("Welcome to Animated Network Topology\nCISCO Look-alike System\n\nComputer Graphics Project | CSE 322\n\nSamina Rahman - 221006012\nAbtahi Md. Mahib Uddin - 221004112\n\n");

    glutCreateWindow("Animated Network Topology");

    initializeNodes();
    initializeClouds();

    initOpenGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0); // Initialize the timer

    // Set full screen mode
    glutFullScreen();

    glutMainLoop();
    return 0;
}
