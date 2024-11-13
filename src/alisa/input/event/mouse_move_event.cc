#include "mouse_move_event.h"

namespace alisa {
namespace input {

MouseMoveEvent::MouseMoveEvent(const mouse_move_event_lambda lambda)
    : lambda_(std::move(lambda)) {}

void MouseMoveEvent::call(MouseMoveData data) { lambda_(data); }

}
}