/**
 * @class InterruptCallback
 * @brief A callback to be invoked.
 * Depending on the platform different callback types are necessary.
 */
#pragma once

#ifdef ESP_BOARD
#include <functional>
using InterruptCallback = std::function<void()>;
#else
using InterruptCallback = void (*)();
#endif
