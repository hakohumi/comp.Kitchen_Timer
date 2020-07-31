/*
 * File:   BuzzerClass.h
 * Author: fuminori.hakoishi
 *
 * Created on 2020/07/31, 9:21
 */

#ifndef BUZZERCLASS_H
#define BUZZERCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "common.h"

void UpdateBuzzer(void);

extern uint8_t BuzzerTimingCount;
extern bool BuzzerTimingFlg;

#ifdef __cplusplus
}
#endif

#endif /* BUZZERCLASS_H */
