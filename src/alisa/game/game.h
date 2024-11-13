#ifndef ALISA_GAME_GAME_H_
#define ALISA_GAME_GAME_H_

namespace alisa {
namespace game {

class Game {
   public:
    Game();
    virtual ~Game();

    virtual bool init();
    virtual bool destroy();
    virtual bool run();
};

}
}  // namespace alisa

#endif