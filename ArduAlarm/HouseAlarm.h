#ifndef HouseAlarm_h
#define HouseAlarm_h

#define MAX_CODE_LEN   10

class HouseAlarm
{
    public:
        HouseAlarm(int);
        void begin();
        void run();
        void activate(char *);
        void ring();
        bool is_ringing();
        bool deactivate(char *);

        int pin_buzzer;

    private:
        enum  t_states
        {
            PRE_ACTIVATED,
            ACTIVATED,
            PRE_RINGING,
            RINGING,
            DEACTIVATED
        };
        t_states state;

        unsigned long startTime = 0;
        unsigned long secondsCounter = 0;

        void pip();

};

#endif