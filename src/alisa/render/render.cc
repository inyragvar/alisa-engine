#include "render.h"

#include <string>
#include "alisa/logger/logger.h"
#include "alisa/graphics/graphics.h"

#include "lib/SDL2/SDL_image.h"
#include "lib/SDL2/SDL_ttf.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
 

namespace alisa {
namespace render {

Render::~Render() {}

Render::Render() : window_(nullptr) {}

std::string getGLErrorString(GLenum error) {
    switch (error) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        // Add more cases for other possible error codes
        default:
            return "Unknown error";
    }
}

void checkGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        logger::Logger::error("Render: OpenGL error: %s", getGLErrorString(error).c_str());
    }
}

// FIXME: need better place and renaming or maybe using getWindowSize from render - need text
void getWindowSizeG(int* width, int* height) {
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", width, height);  
    *width = 800; 
    *height = 600; 
#else
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
        *width = displayMode.w;
        *height = displayMode.h;
    }
#endif
}

void Render::init() {
    auto& config = utils::Config::get();

    // ----- Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logger::Logger::error("Render: SDL could not initialize\n");
        return;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        logger::Logger::error("Render: SDL could not initialize\n");
        return;
    }

    if (TTF_Init() != 0) {
        logger::Logger::error("Render: TTF_Init could not initialize. Error: %s", TTF_GetError());
        return;
    }
    
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        logger::Logger::error("Render: IMG_Init could not initialize. Error: %s", IMG_GetError());
        return;
    }
    
    // init GL
    //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
  //  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
   // SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    
    // ----- Create window
#if defined(MOBILE_OS) || defined(WEB_GL)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight Portrait PortraitUpsideDown");
    SDL_Rect gScreenRect = {0, 0, 320, 240};

    int windowWidth, windowHeight;
    getWindowSizeG(&windowWidth, &windowHeight);
    gScreenRect.w = windowWidth;
    gScreenRect.h = windowHeight;
    
    Log::info("Render: screen_width: %d, screen_height: %d", gScreenRect.w, gScreenRect.h);
    window_ = SDL_CreateWindow(config.getStr("game_title").c_str(), SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, gScreenRect.w,
                               gScreenRect.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

   /* int window_width, window_height;
    SDL_GetWindowSize(window_, &window_width, &window_height);
    float scale_hdpi = 1.0f;
    float scale_vdpi = 1.0f;
    #ifdef __ANDROID__
        // 
    #elif defined(__IPHONEOS__)
        float ddpi, hdpi, vdpi;

        // Get display DPI
        int res = SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(window_), &ddpi, &hdpi, &vdpi);
        scale_hdpi = hdpi / 160.0;
        scale_vdpi = vdpi / 160.0;
    #endif
    // Calculate actual screen resolution (for Retina displays)
    int real_width = window_width * scale_hdpi;
    int real_height = window_height * scale_vdpi;
    

    Log::info("real w: %d, h: %d", real_width, real_height);*/
#else
    int gl_version_major = config.getInt("gl_version_major");
    int gl_version_minor = config.getInt("gl_version_minor");
    if (gl_version_major == 0) {
        gl_version_major = 3;
        logger::Logger::info("Render: using default GL major version: %d", gl_version_major);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_version_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_version_minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window_ = SDL_CreateWindow(config.getStr("game_title").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                               config.getInt("window_width"), config.getInt("window_height"), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
#endif
    if (window_ == nullptr) {
        logger::Logger::error("Render: window could not be created! SDL Error: %s",
                   SDL_GetError());
        return;
    }

    SDL_DisplayMode display_mode;
    const auto depth = 24;

    SDL_zero(display_mode);
    switch (depth) {
        case 8:
            display_mode.format = SDL_PIXELFORMAT_INDEX8;
            break;
        case 15:
            display_mode.format = SDL_PIXELFORMAT_RGB555;
            break;
        case 16:
            display_mode.format = SDL_PIXELFORMAT_RGB565;
            break;
        case 24:
            display_mode.format = SDL_PIXELFORMAT_RGB24;
            break;
        default:
            display_mode.format = SDL_PIXELFORMAT_RGB888;
            break;
    }

    if (SDL_SetWindowDisplayMode(window_, &display_mode) < 0) {
        logger::Logger::error("Render: error set up fullscreen display mode: %s",
                   SDL_GetError());
        return;
    }
    context_ = SDL_GL_CreateContext(window_);

    if (!context_) {
        logger::Logger::error("Render: error create SDL GL context\n");
        return;
    }
   
   SDL_GL_MakeCurrent(window_, context_);
   checkGLError();

#if !defined(MOBILE_OS) && !defined(WEB_GL)
    // Log::info("PC version");
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        logger::Logger::error("Render: error initializing GLEW: %s", glewGetErrorString(res));
        return;
    }
#endif
    int viewport_width;
    int viewport_height;
    
    SDL_GL_GetDrawableSize(window_, &viewport_width, &viewport_height);
    logger::Logger::info("Render: created OpenGL context with viewport size: %d x %d", viewport_width, viewport_height);
    checkGLError();
    
   // config.set("window_width", viewport_width);
   // config.set("window_height", viewport_height);

    SDL_WarpMouseInWindow(window_, viewport_width / 2, viewport_height / 2);

   // system::AdaptScreenSize();
    
   // SDL_GL_SetSwapInterval(1);
   // this parameter by default limiting frame rate == monitor's refresh rate 
    SDL_GL_SetSwapInterval(0);
    
    logger::Logger::info("Render: GL version: %s\n", glGetString(GL_VERSION));

    GLint texture_units = 0;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    checkGLError();
    logger::Logger::info("Render: GL max texture image units: %d\n", texture_units);

    int max_attributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);

    //config.set("gl_max_vertex_atrributes", max_attributes);
    logger::Logger::info("Render: GL max vertex attributes: %d\n", max_attributes);
}

void Render::destroy() {}

void Render::draw(screen::Screen* screen) {
    screen->draw();
    
    SDL_GL_SwapWindow(window_);
}

void Render::getWindowSize(int* width, int* height) {
    bool is_before_change = *width > *height;
    
    SDL_GL_GetDrawableSize(window_, width, height);
}

}
}
