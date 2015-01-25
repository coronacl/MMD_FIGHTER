#pragma once

#include	<windows.h>
#include	<locale.h>

#include	<gl/glut.h>
#include	<stdio.h>

#include	"PMDModel.h"
#include	"VMDMotion.h"
#include	<tchar.h>
#include	"PlayerManage.h"
#include	<math.h>

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"opencv_core249d.lib")
#pragma comment(lib,"opencv_imgproc249d.lib")
#pragma comment(lib,"opencv_highgui249d.lib")
//#pragma comment(lib,"opencv_objdetect249d.lib")
//#pragma comment(lib,"opencv_contrib249d.lib")
//#pragma comment(lib,"opencv_features2d249d.lib")
//#pragma comment(lib,"opencv_flann249d.lib")
//#pragma comment(lib,"opencv_gpu249d.lib")
//#pragma comment(lib,"opencv_haartraining_engined.lib")
//#pragma comment(lib,"opencv_legacy249d.lib")
//#pragma comment(lib,"opencv_ts249d.lib")
//#pragma comment(lib,"opencv_video249d.lib")
#else
//Releaseモードの場合
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
//#pragma comment(lib,"opencv_objdetect249.lib")
//#pragma comment(lib,"opencv_contrib249.lib")
//#pragma comment(lib,"opencv_features2d249.lib")
//#pragma comment(lib,"opencv_flann249.lib")
//#pragma comment(lib,"opencv_gpu249.lib")
//#pragma comment(lib,"opencv_haartraining_engined.lib")
//#pragma comment(lib,"opencv_legacy249.lib")
//#pragma comment(lib,"opencv_ts249.lib")
//#pragma comment(lib,"opencv_video249.lib")
#endif

using namespace cv;