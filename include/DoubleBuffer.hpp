#ifndef DoubleBuffer_h
#define DoubleBuffer_h


#include <Arduino.h>
#define BUFFER_SIZE 5
#define PING 0
#define PONG 1
#define NOT_READY 0
#define READY 1


class DoubleBuffer{
  private:
    uint32_t ping_buff[BUFFER_SIZE];
    uint32_t pong_buff[BUFFER_SIZE];
    uint32_t idx;
    uint32_t write_flag=PING;
    
    void toggle_write_flag();
  public:
    uint32_t ready_flag=NOT_READY;
    uint32_t write(uint32_t data);
    uint32_t * get_read_buff(void);
    uint32_t * get_write_buff(void);
    void set_ready(uint32_t val){
        ready_flag = val;
    }
    uint32_t get_ready(void){
        return ready_flag;
    }


  DoubleBuffer(){
    write_flag=PING;
    ready_flag=NOT_READY;
    idx=0;
    memset(ping_buff,0,sizeof(uint32_t)*BUFFER_SIZE);
    memset(pong_buff,0,sizeof(uint32_t)*BUFFER_SIZE);
  }

};


#endif



