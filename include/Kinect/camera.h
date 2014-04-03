#ifndef _CAMERA_
#define _CAMERA_

#include "libfreenect.hpp"
#include <vector>
//#include <pthread.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


class ExtendedFreenectDevice : public Freenect::FreenectDevice {
public:
  ExtendedFreenectDevice(freenect_context *_ctx, int _index);
  void VideoCallback(void* _rgb, uint32_t timestamp);
  void DepthCallback(void* _depth, uint32_t timestamp);
  bool getVideo(cv::Mat& output);
  bool getDepth(cv::Mat& output);

private:
  std::vector<uint8_t> m_buffer_depth;
  std::vector<uint8_t> m_buffer_rgb;
  std::vector<uint16_t> m_gamma;
  cv::Mat depthMat;
  cv::Mat rgbMat;
  cv::Mat ownMat;
  cv::Mutex m_rgb_mutex;
  cv::Mutex m_depth_mutex;
  bool m_new_rgb_frame;
  bool m_new_depth_frame;
};

class Camera{
 public:
  Camera();
  ~Camera();
  bool getVideo(cv::Mat& output);
  bool getDepth(cv::Mat& output);

 private:
  Freenect::Freenect freenect;
  ExtendedFreenectDevice& extendedFreenectDevice;
};

#endif /* _CAMERA_ */
