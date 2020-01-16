#pragma once

#include <queue>

#define MY_NAMESPACE_BEGIN namespace XAB {
#define MY_NAMESPACE_END }


MY_NAMESPACE_BEGIN

extern const int OcrPeriod; // Ocr 识别周期 (单位: 秒)
extern const std::string OutRootDir; // 识别后的文本输出跟目录

MY_NAMESPACE_END