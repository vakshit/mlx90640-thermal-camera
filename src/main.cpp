#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Float32.h>

#include <mlx90640-thermal-camera/mlx90640.h>

int FPS;
int UPSCALE_FACTOR;

float *rawThermalImage;
cv::Mat grayscaleImage(SENSOR_W, SENSOR_H, CV_32FC1);
cv::Mat normalizedImage;
cv::Mat coloredImage;
cv::Mat upscaledImage;
sensor_msgs::ImagePtr normalizedThermalImage;

void constructNormalizedThermalImage(float minTemp, float maxTemp) {
  for (int y = 0; y < 24; y++) {
    for (int x = 0; x < 32; x++) {
      int index = y * 32 + x;
      grayscaleImage.at<float>(y, x) =
          (rawThermalImage[index] - minTemp) / (maxTemp - minTemp) * 255.0f;
    }
  }

  grayscaleImage.convertTo(normalizedImage, CV_8UC1);
  cv::cvtColor(normalizedImage, coloredImage, cv::COLOR_GRAY2BGR);
  cv::resize(coloredImage, upscaledImage,
             cv::Size(SENSOR_H * UPSCALE_FACTOR, SENSOR_W * UPSCALE_FACTOR));

  normalizedThermalImage =
      cv_bridge::CvImage(std_msgs::Header(), "bgr8", upscaledImage)
          .toImageMsg();
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "mlx90640");
  ros::NodeHandle nh("~");

  if (!nh.getParam("fps", FPS)) {
    ROS_ERROR("Could not load param: fps");
    return 1;
  }
  if (!nh.getParam("upscale_factor", UPSCALE_FACTOR)) {
    ROS_ERROR("Could not load param: upscale_factor");
    return 1;
  }

  ros::Publisher minTempPub =
      nh.advertise<std_msgs::Float32>("/mlx90640/temperature/min", FPS);
  ros::Publisher maxTempPub =
      nh.advertise<std_msgs::Float32>("/mlx90640/temperature/max", FPS);
  // ros::Publisher rawThermalImagePub =
  //     nh.advertise<float *>("/mlx90640/image/raw", FPS);
  ros::Publisher normalizedThermalImagePub =
      nh.advertise<sensor_msgs::Image>("/mlx90640/image/normalized", FPS);
  ros::Rate loopRate(FPS);

  MLX90640 thermalCamera = MLX90640(FPS);
  std_msgs::Float32 minTemp, maxTemp;

  while (ros::ok()) {
    ros::spinOnce();

    thermalCamera.see();
    thermalCamera.copyRawImage(rawThermalImage);
    minTemp.data = thermalCamera.getMin();
    maxTemp.data = thermalCamera.getMax();
    constructNormalizedThermalImage(minTemp.data, maxTemp.data);

    minTemp.data = 1.0;
    maxTemp.data = 100.0;
    minTempPub.publish(minTemp);
    maxTempPub.publish(maxTemp);
    // rawThermalImagePub.publish(rawThermalImage);
    normalizedThermalImagePub.publish(normalizedThermalImage);

    loopRate.sleep();
  }

  return 0;
}