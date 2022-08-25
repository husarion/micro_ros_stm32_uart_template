#include <mbed.h>
#include <micro_ros_utilities/string_utilities.h>
#include <micro_ros_utilities/type_utilities.h>
#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <std_msgs/msg/string.h>
#include <stdio.h>

#include "microros_transport/mbed_serial_transport.hpp"

static rcl_publisher_t publisher;
static rcl_subscription_t subscriber;
static std_msgs__msg__String msg;
static rclc_executor_t executor;
static rclc_support_t support;
static rcl_allocator_t rcl_allocator;
static rcl_node_t node;
static rcl_timer_t timer;

constexpr uint64_t SERIAL_BAUDRATE = 576000;
constexpr uint64_t FREQUENCY = 10;

static UARTSerial uros_serial(PA_9, PA_10);

#define RCCHECK(fn)                    \
    {                                  \
        rcl_ret_t temp_rc = fn;        \
        if ((temp_rc != RCL_RET_OK)) { \
            error_loop();              \
        }                              \
    }
#define RCSOFTCHECK(fn)                \
    {                                  \
        rcl_ret_t temp_rc = fn;        \
        if ((temp_rc != RCL_RET_OK)) { \
        }                              \
    }

void error_loop() {
    while (1) {
        // error
    }
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
    RCLC_UNUSED(last_call_time);
    if (timer != NULL) {
        RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    }
}

void subscription_callback(const void *msgin) {
    const std_msgs__msg__String *inmsg = (const std_msgs__msg__String *)msgin;
    UNUSED(inmsg);
}

static void setup() {
    // set baudrate and initialize transport
    uros_serial.set_baud(SERIAL_BAUDRATE);
    set_microros_serial_transports(&uros_serial);

    rcl_allocator = rcl_get_default_allocator();

    // create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &rcl_allocator));

    // create node
    RCCHECK(rclc_node_init_default(&node, "micro_ros_chatter", "", &support));

    // create publisher
    RCCHECK(rclc_publisher_init_default(
        &publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        "chatter"));

    // create subscriber
    RCCHECK(rclc_subscription_init_default(
        &subscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        "chatter"));

    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &rcl_allocator));
    RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg,
                                           &subscription_callback, ON_NEW_DATA));

    // create timer,
    const unsigned int timer_timeout = 1.0 / FREQUENCY * 1e9;
    RCCHECK(rclc_timer_init_default(
        &timer,
        &support,
        timer_timeout,
        timer_callback));

    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &rcl_allocator));
    RCCHECK(rclc_executor_add_timer(&executor, &timer));

    micro_ros_utilities_create_message_memory(
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        &msg,
        (micro_ros_utilities_memory_conf_t){});
}

void loop() {
    RCSOFTCHECK(rclc_executor_spin(&executor));
}

void handle_string() {
    int i = 0;
    while (true) {
        char buffer[50];
        sprintf(buffer, "%d", i);
        msg.data = micro_ros_string_utilities_set(msg.data, buffer);
        ThisThread::sleep_for(1000);
        ++i;
    }
}
int main() {
    setup();
    Thread thread;
    thread.start(handle_string);

    while (true) {
        loop();
    }
}