#include "DoubleBuffer.hpp"

void DoubleBuffer::toggle_write_flag(){
    if(write_flag==PING){
      write_flag=PONG;
    }else{
      write_flag=PING;
    }
}
uint32_t * DoubleBuffer::get_read_buff(void){
  if(write_flag==PING){
    return pong_buff;
  }
  return ping_buff;
}
uint32_t * DoubleBuffer::get_write_buff(void){
  if(write_flag==PING){
    return ping_buff;
  }
  return pong_buff;
}
uint32_t DoubleBuffer::write(uint32_t data){
    uint32_t *pwrite = get_write_buff();
    pwrite[idx]=data;
    idx++;
    if(idx==BUFFER_SIZE){
      idx=0;
      toggle_write_flag();
      set_ready(READY);
    }
    return ready_flag;
}
