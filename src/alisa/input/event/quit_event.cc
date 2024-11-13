#include "quit_event.h"

namespace alisa {
namespace input {

QuitEvent::QuitEvent(const quit_event_lambda lambda)
    : lambda_(std::move(lambda)) {}

void QuitEvent::call() { lambda_(); }

}
} 