#pragma once

#ifdef _WIN64
#	define X64
#endif
#ifndef _DEBUG
#	define NDEBUG
#	define _SECURE_SCL 0
#	define _HAS_ITERATOR_DEBUGGING 0
#endif

#ifdef _DEBUG
#	include <vld.h>
#endif

#include <algorithm>
#include <cassert>
#include <vector>
#include <string>
#include <set>
#include <DirectXMath.h>

using std::vector;
using std::string;
using std::min;
using std::max;
