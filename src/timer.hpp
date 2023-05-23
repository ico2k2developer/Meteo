//
// Created by ico2k2 on 21/5/2023.
//

#ifndef METEO_TIMER_HPP
#define METEO_TIMER_HPP

#include <cstdint>
#include <functional>
#include <Arduino.h>

#define TIMER_ID_NULL   0

typedef uint8_t timer_id_t;
typedef uint64_t timer_trigger_t;
typedef std::function<uint8_t(timer_id_t,timer_trigger_t)> timer_action_t;

typedef struct
{
    timer_id_t id;
    timer_trigger_t trigger;
    timer_action_t action;
}timer2_t;
typedef timer2_t* timer_p;

timer_p timers;
uint8_t timers_count,timers_size;

uint8_t timer_init(uint8_t count);
uint8_t timer_set_abs(timer_id_t id,timer_trigger_t absolute,timer_action_t action);
uint8_t timer_set_rel(timer_id_t id,timer_trigger_t relative,timer_action_t action);
uint8_t timer_remove(timer_id_t id);
uint8_t timer_trigger(timer_id_t id);
void    timer_release();
uint8_t timer_tick();

#endif //METEO_TIMER_HPP
