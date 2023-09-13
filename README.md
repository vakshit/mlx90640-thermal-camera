# mlx90640_thermal_camera

This is a ROS-noetic driver package for the MLX90640 sensor tested on Raspberry Pi4.

Download the debian package from the [Releases](https://github.com/vakshit/mlx90640_thermal_camera/releases/tag/v1.0.0) page.

## Installation

```bash
sudo apt-get install ros-noetic-mlx90640_thermal_camera_1.0.0-0focal_arm64.deb
```

`NOTE:` Install the amd64 package if you are using an x86_64 machine.

## Usage

```bash
sudo su -
roslaunch mlx90640_thermal_camera thermal_camera.launch
```

`NOTE:` You need to be the root user to access the I2C device.
