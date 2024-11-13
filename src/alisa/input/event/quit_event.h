#ifndef ALISA_INPUT_EVENT_QUIT_EVENT_H_
#define ALISA_INPUT_EVENT_QUIT_EVENT_H_

#include <functional>

namespace alisa {
namespace input {

typedef std::function<void(void)> quit_event_lambda;

class QuitEvent {
   public:
    QuitEvent(const quit_event_lambda lambda);

   public:
    void call();

   private:
    quit_event_lambda lambda_;
};

}
}

#endif