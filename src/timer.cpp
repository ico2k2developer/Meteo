//
// Created by ico2k2 on 21/5/2023.
//

#include <timer.hpp>

timer_p timers;
uint8_t timers_count,timers_size;

uint8_t timer_init(uint8_t count)
{
    Serial.printf("Allocating %hhu timers...\n",count);
    timers = (timer_p)malloc(count * sizeof(timer2_t));
    Serial.printf("Allocated %hhu timers (%zu bytes)\n",count,count * sizeof(timer2_t));
    if(timers)
    {
        timer_id_t i;
        timers_count = 0;
        timers_size = count;
        for(i = 0; i < count; i++)
        {
            Serial.printf("Initializing element %hhu...\n",i);
            timers[i].id = TIMER_ID_NULL;
            Serial.printf("Assigned id %d\n",timers[i].id);
            timers[i].action = nullptr;
            Serial.println("Assigned callback");
        }
        return 0;
    }
    return -1;
}

uint8_t timer_set_abs(timer_id_t id,timer_trigger_t absolute,timer_action_t action)
{
    Serial.printf("Timer set request, current count %d, id %d, time %llu:\n\t",timers_count,id,absolute);
    if(!timers)
        return -1;
    timer_id_t i;
    for(i = 0; i < timers_count; i++)
    {
        if(timers[i].id == TIMER_ID_NULL || timers[i].id == id)
            break;
    }
    Serial.printf("proposed position is %d, ",i);
    if(i < timers_size)
    {
        Serial.print("accepted.");
        if(i == timers_count)
        {
            timers_count++;
            Serial.printf(" Count set to %d.",timers_count);
        }
        timers[i].id = id;
        timers[i].trigger = absolute;
        if(action)
            timers[i].action = action;
        return timers_count;
    }
    Serial.println();
    return -2;
}

uint8_t timer_set_rel(timer_id_t id,timer_trigger_t relative,timer_action_t action)
{
    if(micros64() < relative)
        return timer_set_abs(id,relative - micros64(),action);
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
            timers[i].id = TIMER_ID_NULL;
            return 0;
        }
    }
    return -2;
}

void timer_triggered(timer_id_t index,timer_trigger_t trigger)
{
    if(timers[index].action)
        if(!(timers[index].action)(index,trigger))
            timers[index].id = TIMER_ID_NULL;
}

uint8_t timer_trigger(timer_id_t id)
{
    if(!timers)
        return -1;
    timer_id_t i;
    for(i = 0; i < timers_count; i++)
    {
        if(id == timers[i].id)
        {
            timer_triggered(i,micros64());
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
        if(timers[i].id != TIMER_ID_NULL)
        {
            trigger = micros64();
            if(timers[i].trigger <= trigger)
            {
                result = 1;
                timer_triggered(i,trigger);
            }
        }
    }
    Serial.println();
    return result;
}