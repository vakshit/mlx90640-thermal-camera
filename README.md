# mlx90640-thermal-camera

This is a ROS-noetic driver package for the MLX90640 sensor tested on Raspberry Pi4.

Download the debian package from the [Releases](https://github.com/vakshit/mlx90640-thermal-camera/releases/tag/v1.0.0) page.

## Installation

```bash
sudo apt-get install ros-noetic-mlx9064-thermal-camera.deb
```

## Usage

```bash
sudo su -
roslaunch mlx90640-thermal-camera thermal_camera.launch
```

`NOTE:` You need to be the root user to access the I2C device.
