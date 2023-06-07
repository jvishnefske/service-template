//
// Created by cnh on 6/6/23.
//

#include "test.h"
#include "backend.h"
#include <gtest/gtest.h>

TEST(backendTests, createInstance){
    backend b;
    ASSERT_DOUBLE_EQ(0.0, b.value);
}


