#ifndef ALISA_RENDER_RENDER_H_
#define ALISA_RENDER_RENDER_H_

#include <mutex>

#include "alisa/utils/singleton.h"
#include "alisa/utils/config.h"
#include "alisa/screen/screen.h"

#define SDL_MAIN_HANDLED
#include "lib/SDL2/SDL.h"

namespace alisa {
namespace render {

class Render : public utils::Singleton<Render> {
    friend class utils::Singleton<Render>;

public:
    ~Render();

    void init();
    void destroy();

    void draw(screen::Screen* screen);
    
    void getWindowSize(int* width, int* height);

protected:
    Render();
    
private:
    SDL_Window* window_;
    SDL_GLContext context_;
    SDL_GLContext thread_context_;
};

}
}

#endif
