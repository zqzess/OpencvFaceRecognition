// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#include "opencv/build/include/opencv2/opencv.hpp"
#include "opencv/build/include/opencv2/objdetect/objdetect.hpp"
#include "opencv/build/include/opencv2/highgui/highgui.hpp"
#include "opencv/build/include/opencv2/imgproc/imgproc.hpp"
#include "opencv/build/include/opencv2/imgproc/types_c.h"
#include "opencv/build/include/opencv2/core/core.hpp"
#include "opencv/build/include/opencv2/features2d/features2d.hpp"
#include "opencv/install/include/opencv2/face/facerec.hpp"
#include "ArcFace/inc/arcsoft_face_sdk.h"
#include "ArcFace/inc/merror.h"
#include "ArcFace/inc/asvloffscreen.h"
#include "ArcFace/inc/amcomdef.h"

#endif //PCH_H
