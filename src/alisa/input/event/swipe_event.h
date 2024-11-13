#ifndef ALISA_INPUT_EVENT_SWIPE_EVENT_H_
#define ALISA_INPUT_EVENT_SWIPE_EVENT_H_

#include <functional>

#include "alisa/utils/math/vector_2f.h"

namespace alisa {
namespace input {

struct SwipeData {
    utils::math::Vector2f direction;
};

typedef std::function<void(SwipeData)> swipe_event_lambda;

class SwipeEvent {
public:
    SwipeEvent(const swipe_event_lambda lambda);

    void call(SwipeData data);

private:
    swipe_event_lambda lambda_;
};

}
}

#endif