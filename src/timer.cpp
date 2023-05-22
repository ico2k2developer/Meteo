//
// Created by ico2k2 on 21/5/2023.
//

#include <timer.hpp>
#include <utility>

uint8_t timer_init(uint8_t count)
{
    timers = (timer_p)malloc(count * sizeof(timer_t));
    if(timers)
    {
        timers_count = 0;
        timers_size = count;
        return 0;
    }
    return -1;
}

uint8_t timer_set_abs(timer_id_t id,timer_trigger_t absolute,timer_action_t action)
{
    if(!timers)
        return -1;
    timer_id_t i;
    for(i = 0; i < timers_count; i++)
    {
        if(timers[i].id == 0 || timers[i].id == id)
            break;
    }
    if(i < timers_size)
    {
        if(i == timers_count)
            timers_count++;
        timers[i].id = id;
        timers[i].trigger = absolute;
        timers[i].action = std::move(action);
        return timers_count;
    }
    return -2;
}

uint8_t timer_set_rel(timer_id_t id,timer_trigger_t relative,timer_action_t action)
{
    if(micros64() < relative)
        return timer_set_abs(id,relative - micros64(),std::move(action));
    else
        return -3;
}

uint8_t timer_remove(timer_id_t id)
{
    if(!timers)
        return -1;
    timer_id_t i;
    for(i = 0; i < timers_count; i++)
    {
        if(id == timers[i].id)
        {
            timers[i].id = 0;
            return 0;
        }
    }
    return -2;
}

void timer_release()
{
    free(timers);
    timers_count = timers_size = 0;
}

uint8_t timer_tick()
{
    uint8_t result = 0;
    timer_id_t i;
    timer_trigger_t trigger;
    for(i = 0; i < timers_count; i++)
    {
        if(timers[i].id && timers[i].trigger <= (trigger = micros64()))
        {
            result = 1;
            if(!timers[i].action(trigger))
                timers[i].id = 0;
        }
    }
    return result;
}