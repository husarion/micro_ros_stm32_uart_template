FROM ros:humble-ros-core

RUN apt-get update && apt-get install -y \
    ros-${ROS_DISTRO}-demo-nodes-cpp && \
    apt-get autoremove -y && \
    rm -rf /var/lib/apt/lists/*