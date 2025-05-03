// TODO:
// Add more xmb options
// Button_Highlighting
// Input controls (Mouse + Keyboard , Controllers)
// Work on process management and framebuffer calls
#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
//ma#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLT_IMPLEMENTATION

#include "/home/gamer/xmb_dev/external/glad/include/glad/glad.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <vector>
#include <cstring>  
#include <GLFW/glfw3.h> 
//#include "gltext.h"
//#include <GLFW/glfw3.h> 
#include <thread>  
#include <chrono> 
#include "text.h"
#include "xmb_games.h"
GLuint fontTexture;
#include <thread>
#include <mutex>
#include <queue>

std::mutex textRenderMutex;
std::queue<std::string> textQueue; 

// Function to check OpenGL errors
void prepareText(const std::string& text) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate some work
    std::lock_guard<std::mutex> lock(textRenderMutex);
    textQueue.push(text);
}
void checkOpenGLError(const char* statement) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after " << statement << ": " << error << std::endl;
    }
} 

int print(std::string text){
    std::cout << "LOG: " << text << " ";
    return 0;
}
struct xmb_Loading {
    float xmb_Loading_progress = 0.0f;
};
#include <vector>

struct XMBItem {
    std::string name;      
    std::string iconPath; 
    std::string dataPath;  
};

struct XMBCategory {
    std::string title;              // The title of the category (e.g., "Games")
    std::vector<XMBItem> items;     // List of items in the category (buttons, options, etc.)

    void addItem(const std::string& name, const std::string& iconPath, const std::string& dataPath) {
        items.push_back({name, iconPath, dataPath});
    }
};

struct XMB {
    XMBCategory games;    
    XMBCategory settings; 
    XMBCategory apps;    
};
xmb_Loading loading_properties;

bool isPNG(const char* filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return false;

    unsigned char signature[8];
    file.read(reinterpret_cast<char*>(signature), 8);

    // PNG signature: 89 50 4E 47 0D 0A 1A 0A
    const unsigned char png_signature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

    return std::equal(std::begin(png_signature), std::end(png_signature), std::begin(signature));
}
const float replacementColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
GLuint loadTexture(const char* filepath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4); // Force RGBA
    if (!data) {
        std::cerr << "Failed to load image: " << filepath << "\n";
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the modified image data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return texture;
}
#include <cmath>

// Draws a rectangle with rounded corners
void Draw_Rounded_Rectangle(float x, float y, float width, float height, float radius) {
    // Make sure the radius is not larger than half of the width or height
    radius = std::min(radius, std::min(width, height) / 2.0f);

    // Draw the four rounded corners
    const int segments = 30;  // Number of segments in the circle (higher = smoother)

    // Top-left corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius); // Center of the arc
    for (int i = 0; i <= segments; ++i) {
        float angle = (i / (float)segments) * M_PI_2;
        glVertex2f(x + radius + cos(angle) * radius, y + radius + sin(angle) * radius);
    }
    glEnd();

    // Top-right corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + width - radius, y + radius); // Center of the arc
    for (int i = 0; i <= segments; ++i) {
        float angle = (i / (float)segments) * M_PI_2;
        glVertex2f(x + width - radius + cos(angle) * radius, y + radius + sin(angle) * radius);
    }
    glEnd();

    // Bottom-left corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + height - radius); // Center of the arc
    for (int i = 0; i <= segments; ++i) {
        float angle = (i / (float)segments) * M_PI_2;
        glVertex2f(x + radius + cos(angle) * radius, y + height - radius + sin(angle) * radius);
    }
    glEnd();

    // Bottom-right corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + width - radius, y + height - radius); // Center of the arc
    for (int i = 0; i <= segments; ++i) {
        float angle = (i / (float)segments) * M_PI_2;
        glVertex2f(x + width - radius + cos(angle) * radius, y + height - radius + sin(angle) * radius);
    }
    glEnd();

    // Draw the four sides of the rectangle
    glBegin(GL_QUADS);
    // Top side
    glVertex2f(x + radius, y); 
    glVertex2f(x + width - radius, y); 
    glVertex2f(x + width - radius, y + radius); 
    glVertex2f(x + radius, y + radius); 

    // Bottom side
    glVertex2f(x + radius, y + height - radius);
    glVertex2f(x + width - radius, y + height - radius);
    glVertex2f(x + width - radius, y + height);
    glVertex2f(x + radius, y + height);

    // Left side
    glVertex2f(x, y + radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x + radius, y + height - radius);
    glVertex2f(x, y + height - radius);

    // Right side
    glVertex2f(x + width - radius, y + radius);
    glVertex2f(x + width, y + radius);
    glVertex2f(x + width, y + height - radius);
    glVertex2f(x + width - radius, y + height - radius);
    glEnd();
}

void Draw_Loading_Bar(float x, float y, float width, float height, const xmb_Loading& properties) {
    // Render the background bar (transparent grey background)
    glDisable(GL_TEXTURE_2D);  // Ensure textures are not used
    glColor4f(0.1f, 0.1f, 0.1f, 0.3f);  // Transparent grey background
    Draw_Rounded_Rectangle(x, y, width, height, 20.0f);  // Rounded corners

    // Render the progress fill
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // White color for the fill

    float filledWidth = width * properties.xmb_Loading_progress;  // <- Here's the difference!
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + filledWidth, y);
    glVertex2f(x + filledWidth, y + height);
    glVertex2f(x, y + height);
    glEnd();

  //  glDisable(GL_BLEND);  // Disable blending after drawing
}
struct Game_Assets{
    char* xmb_os_icon_ico = "assets/os_xmb_icon.png";
    char* xmb_game_ico = "assets/games_xmb_icon.png";
};
static GLuint compileShader(GLenum type, const char* src) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint ok; glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
      char buf[512]; glGetShaderInfoLog(id, 512, nullptr, buf);
      fprintf(stderr,"Shader compile error: %s\n",buf);
    }
    return id;
  }
  int drawTexture(const char* imagePath, float x, float y, float w, float h) {
    static bool        inited    = false;
    static GLuint      prog=0, vao=0, vbo=0, tex=0;
    static GLint       locProj, locOffset, locScale;
    static std::string prevPath;         // remember what we last loaded

    const int SW = 1280, SH = 720;

    // 1) one‑time GLSL + VAO/VBO setup
    if (!inited) {
      // --- compile & link shaders ---
      const char* vsSrc = R"glsl(
        #version 330 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec2 aUV;
        uniform mat4 uProj;
        uniform vec2 uOff;
        uniform vec2 uScl;
        out vec2 vUV;
        void main() {
          vec2 pos = aPos * uScl + uOff;
          gl_Position = uProj * vec4(pos, 0.0, 1.0);
          vUV = aUV;
        }
      )glsl";
      const char* fsSrc = R"glsl(
        #version 330 core
        in vec2 vUV;
        out vec4 oCol;
        uniform sampler2D uTex;
        void main() {
          oCol = texture(uTex, vUV);
        }
      )glsl";

      GLuint vs = compileShader(GL_VERTEX_SHADER,   vsSrc);
      GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
      prog = glCreateProgram();
      glAttachShader(prog, vs);
      glAttachShader(prog, fs);
      glLinkProgram(prog);
      GLint ok; glGetProgramiv(prog, GL_LINK_STATUS, &ok);
      if (!ok) {
        char buf[512]; glGetProgramInfoLog(prog,512,nullptr,buf);
        fprintf(stderr,"Program link error: %s\n",buf);
      }
      glDeleteShader(vs);
      glDeleteShader(fs);

      // get uniform locations
      locProj   = glGetUniformLocation(prog, "uProj");
      locOffset = glGetUniformLocation(prog, "uOff");
      locScale  = glGetUniformLocation(prog, "uScl");

      // --- build VAO/VBO for a unit quad (0,0)-(1,1) ---
      float verts[] = {
        //  x     y      u    v
          0.0f, 1.0f,  0.0f,1.0f,
          0.0f, 0.0f,  0.0f,0.0f,
          1.0f, 0.0f,  1.0f,0.0f,

          0.0f, 1.0f,  0.0f,1.0f,
          1.0f, 0.0f,  1.0f,0.0f,
          1.0f, 1.0f,  1.0f,1.0f
      };
      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
      glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      inited = true;
    }

    // 2) if the path changed, delete the old GL texture so we can reload
    if (prevPath != imagePath) {
      if (tex) {
        glDeleteTextures(1, &tex);
        tex = 0;
      }
      prevPath = imagePath;
    }

    // 3) load (or reload) the texture
    if (tex == 0) {
      tex = loadTexture(imagePath);
      if (tex == 0) {
        std::cerr << "drawTexture: failed to load " << imagePath << "\n";
        return -1;
      }
    }

    // 4) draw
    glUseProgram(prog);
    float proj[16] = {
      2.0f/ SW, 0,        0, 0,
      0,       2.0f/SH,  0, 0,
      0,       0,       -1, 0,
      -1,     -1,        0, 1
    };
    glUniformMatrix4fv(locProj,   1, GL_FALSE, proj);
    glUniform2f(       locOffset,  x,    y);
    glUniform2f(       locScale,    w,    h);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(glGetUniformLocation(prog, "uTex"), 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // restore state
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    return 0;
}

 // Asset_INIT
 int Draw_XMB(){
    Game_Assets Assets;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1280,720,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float iconW   = -64.0f;
    float iconH   = -64.0f;
    float centerX = ((1280 - iconW)/2) + 430;
    float centerY = ((720  - iconH)/2) + 200;

    // draw the icon
    drawTexture("assets/games_xmb_icon.png", centerX, centerY, iconW, iconH);

    // prepare text renderer
    Text textRenderer("/home/gamer/xmb_dev/font/font.ttf", 14);
    textRenderer.setScreenSize(1280, 720);

    // position the text *centered* under the icon, 10px below
    float textX = centerX + iconW + 6.5f;
    float textY = centerY - 68.0f;

    textRenderer.render("Games", textX, textY);
    return 0;
}

int Draw_Loading_Menu() {
    Game_Assets Assets;
    
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glOrtho(0, 1280, 720, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float iconWidth = -256;
    float iconHeight = -256;
    float centerX = ((1280 - iconWidth) / 2);
    float centerY = ((720 - iconHeight) / 2) - 15;
    drawTexture(Assets.xmb_os_icon_ico, centerX, centerY, iconWidth, iconHeight);
 
    // Y position for the loading bar
    float width = 400.0f;
    float height = 30.0f;
    float x = ((1280 - 400.0f) / 2);
    float y = ((720 - 30.0f) / 2) + 120;

    Draw_Loading_Bar(x, y, width, height, loading_properties);



    return 0;
}



int main(){
    
    print("INIT_XMB_LOADING_GRAPHICS!");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    } // Loads SDL and init it
    SDL_Window* window = SDL_CreateWindow(
        "XMB",              // Window title
        SDL_WINDOWPOS_CENTERED,        // X position
        SDL_WINDOWPOS_CENTERED,        // Y position
        1280, 720,                     // Width and height
        SDL_WINDOW_OPENGL              // Using OpenGL
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    gladLoadGL();
   
 
    //Frame_Management
    SDL_GL_SetSwapInterval(1);  // For V-Sync
    bool running = true;
    SDL_Event event;
    while (running) {
       
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                
            }
        }
    
    Draw_Loading_Menu();
   // RenderText("Hello World!", 100, 100, 1.0f);


    XMB xmb;
    std::string filepath = "/home/gamer/xmb_dev/font/font.ttf";


    // Games category
    xmb.games.title = "Games";
 //   xmb.games.addItem("Game 1", "assets/icons/game1.png", "/Gamer_OS/Games/Game1/GAMEDAT.gi");
  //  xmb.games.addItem("Game 2", "assets/icons/game2.png", "/Gamer_OS/Games/Game2/GAMEDAT.gi");

    // Settings category
    xmb.settings.title = "Settings";
  //  xmb.settings.addItem("Display", "assets/icons/display.png", "settings/display.ini");
  //  xmb.settings.addItem("Sound", "assets/icons/sound.png", "settings/sound.ini");
 //   xmb.settings.addItem("System Updates", "assets/icons/updates.png", "settings/updates.ini");

    // Apps category
    xmb.apps.title = "Apps";
    GLuint tex = 0; 
   // xmb.apps.addItem("Firefox", "assets/icons/firefox.png", "/Gamer_OS/apps/firefox/APP.ai");
    ::loading_properties.xmb_Loading_progress += 0.005f;
    if (::loading_properties.xmb_Loading_progress > 1.0f) {
      ::loading_properties.xmb_Loading_progress = 1.0f;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (::loading_properties.xmb_Loading_progress = 1.0f){
        glDeleteTextures(1, &tex);   
        Draw_XMB();
    }
    }
    

    // Draw_XMB();
    // Swap window
    SDL_GL_SwapWindow(window);  
    }


    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}