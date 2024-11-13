#include "swipe_event.h"

namespace alisa {
namespace input {

SwipeEvent::SwipeEvent(const swipe_event_lambda lambda)
    : lambda_(std::move(lambda)) {}

void SwipeEvent::call(SwipeData data) { lambda_(data); } 

}
}