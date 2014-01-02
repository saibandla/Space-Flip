#pragma once

#ifndef _SK_ASSERT_H_
#define _SK_ASSERT_H_

#include <assert.h>

#define SK_ASSERT(x) assert(x)
#define SK_ASSERT_TEST(x) SK_ASSERT(x)
#define SK_ASSERT_01_INC(x) SK_ASSERT(x >= 0 && x <= 1)

#define SK_NON_IMPLEMENTED SK_ASSERT(false)
#endif