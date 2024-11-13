#include "mouse_event.h"

namespace alisa {
namespace input {

MouseEvent::MouseEvent(const mouse_event_lambda lambda)
    : lambda_(std::move(lambda)) {}

void MouseEvent::call(MouseData data) { lambda_(data); }

}
}