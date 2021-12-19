//
// Created by lennart on 9/19/21.
//
#pragma once



#ifdef __cplusplus
#include <array>
#include <string>
#include <stdlib.h>
//#include <new>
extern "C" {
#endif
#include "common/app/app.h"
#include "common/app/apps.h"
#include "common/svc/svc.h"
#include "common/hal/hal.h"
#include "common/hal/lcd_segments.h"

typedef struct {
    APP_PRIV_COMMON
} priv_t;

#ifdef __cplusplus
}
#endif