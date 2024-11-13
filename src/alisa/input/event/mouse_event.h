#ifndef ALISA_INPUT_EVENT_MOUSE_EVENT_H_
#define ALISA_INPUT_EVENT_MOUSE_EVENT_H_

#include <functional>

namespace alisa {
namespace input {
    
struct MouseData {
    float x;
    float y;

    bool is_pressed;
};

typedef std::function<void(MouseData)> mouse_event_lambda;

class MouseEvent {
public:
    MouseEvent(const mouse_event_lambda lambda);

    void call(MouseData data);

private:
    mouse_event_lambda lambda_;
};

}
}

#endif