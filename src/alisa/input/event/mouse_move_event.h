#ifndef ALISA_INPUT_EVENT_MOUSE_MOVE_EVENT_H_
#define ALISA_INPUT_EVENT_MOUSE_MOVE_EVENT_H_

#include <functional>

namespace alisa {
namespace input {
    
struct MouseMoveData {
    float x;
    float y;

    float offset_x;
    float offset_y;

    bool is_pressed;
};

typedef std::function<void(MouseMoveData)> mouse_move_event_lambda;

class MouseMoveEvent {
public:
    MouseMoveEvent(const mouse_move_event_lambda lambda);

    void call(MouseMoveData data);

private:
    mouse_move_event_lambda lambda_;
};

}
}

#endif