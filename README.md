# Animated Network Topology

## Project Overview
This project is an animated network topology visualization created using OpenGL and GLUT. The visualization showcases a network consisting of a server, routers, and computers, connected with lines. It includes animated elements such as moving clouds and a dot traveling along the connections. This project is designed as a computer graphics demonstration, resembling a CISCO-like network topology.

## Features
Network Topology Visualization: Displays a network with nodes representing a server, routers, and computers.

Animated Clouds: Clouds move across the screen at a slower speed.
Moving Dot: A dot moves along the network connections to represent data flow.

Dynamic Rendering: Real-time animation and rendering of the network and clouds.

Text Rendering: Displays a welcome message and project details on the screen.
## Requirements
OpenGL: A cross-language API for rendering 2D and 3D vector graphics.

GLUT: A toolkit for managing windows and input for OpenGL applications.

Codeblocks: IDE - Best for C++ Projects
## Demo Video

[![Animated Network Topology Demo Video](https://img.youtube.com/vi/nRyuX68z6-0/0.jpg)](https://youtu.be/nRyuX68z6-0)

## Code Explanation
### Utility Structures

```
// Utility structure for handling 2D vectors (positions, velocities, etc.)
struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
};
```
### Vec2: A simple 2D vector class used for positions and velocities.

```
// Structure to represent a network node (router, server, computer)
struct Node {
    Vec2 position;
    float radius;
    Vec2 color;
    Node(float x, float y, float r, Vec2 c) : position(x, y), radius(r), color(c) {}
};

```
### Node: Represents a network node with position, size, and color.
```
// Structure to represent a cloud
struct Cloud {
    Vec2 position;
    float speed;
    Cloud(float x, float y, float s) : position(x, y), speed(s) {}
};
```
### Cloud: Represents a cloud with position and speed.
```
// Initialize nodes
void initializeNodes() {
    // Define server, routers, and computers with their positions, sizes, and colors.
}

// Initialize clouds
void initializeClouds() {
    // Create clouds with random positions and ensure they are not too close to each other.
}
```
```initializeNodes()```: Initializes nodes with their positions, sizes, and colors.

```initializeClouds()```: Initializes clouds with random positions and speed, ensuring they are spaced apart.

### Drawing Functions
```
// Draw a node
void drawNode(const Node& node) {
    // Draws a node based on its type (server, router, or computer) using different shapes.
}

// Draw a line connecting two nodes
void drawConnection(const Node& from, const Node& to) {
    // Draws a line between two nodes.
}

// Draw a dot on the connection line
void drawDotOnConnection(const Node& from, const Node& to, float position) {
    // Draws a dot moving along the line connecting two nodes.
}

// Draw clouds
void drawCloud(const Cloud& cloud) {
    // Draws a cloud using a triangle fan.
}

```
```drawNode()```: Draws a node based on its type and color.

```drawDotOnConnection()```: Draws a moving dot along the connection line.

```drawConnection()```: Draws a line connecting two nodes.

```drawCloud()```: Draws a cloud using a series of triangles.


### Update Functions

```
// Update positions of clouds
void updateClouds() {
    // Moves clouds and wraps them around the screen.
}

// Update the position of the moving dot along the connection
void updateDotPosition() {
    // Moves the dot along the connection line.
}
```
```updateClouds()```: Updates the position of clouds and wraps them around the screen.
```updateDotPosition()```: Updates the dot's position along the connection.

### Display Function
```
// Display function
void display() {
    // Clears the screen, draws clouds, nodes, connections, and renders text.
}
```
```display()```: Clears the screen and draws all visual elements.

### Timer Function
```
// Timer function for animation
void timer(int value) {
    updateClouds();
    updateDotPosition();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // Call timer function every 16ms (~60 FPS)
}
```
```timer()```: Updates animations and schedules the next frame.

### Initialization and Input Handling

```
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
```

``` initOpenGL()``` : Sets up the OpenGL environment.

``` keyboard() ``` : Handles keyboard input, specifically the ESC key to exit the application.

## Conclusion
This project provides a visualization of a network topology with animated elements using OpenGL and GLUT. It demonstrates basic rendering techniques, animation, and real-time updates in a graphical application.