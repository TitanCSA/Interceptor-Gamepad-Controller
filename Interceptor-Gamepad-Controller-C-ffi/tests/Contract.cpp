#include "InterceptorBridge.h"
#include <windows.h>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <iostream>
extern "C" __declspec(dllimport) void interceptor_gamepad_controller_test_mode(unsigned);
extern "C" __declspec(dllimport) long long interceptor_gamepad_controller_test_seen(unsigned,unsigned);
void Check(bool value) {if(!value)throw std::runtime_error("Product C ABI contract failed");}
bool Allowed(unsigned kind) {return kind<=2 ? (TEST_INPUT_MASK&(1u<<(kind-1)))!=0 : (TEST_CONTROLLER_MASK&(1u<<(kind-3)))!=0;}
int main() {
 try {
  Check(!std::strcmp(interceptor_gamepad_controller_product_name(),TEST_PRODUCT));
  Check(interceptor_gamepad_controller_input_mask()==TEST_INPUT_MASK && interceptor_gamepad_controller_controller_mask()==TEST_CONTROLLER_MASK);
  Check(interceptor_gamepad_controller_version()==1 && interceptor_gamepad_controller_sizeof(1)==sizeof(interceptor_gamepad_controller_field) && interceptor_gamepad_controller_sizeof(6)==sizeof(interceptor_gamepad_controller_info));
  unsigned expected=0;for(unsigned k=1;k<=6;++k)if(Allowed(k))++expected;
  interceptor_gamepad_controller_error error{};uint64_t session=0,other=0;
  Check(!interceptor_gamepad_controller_open(&session,&error));Check(interceptor_gamepad_controller_open(&other,&error)==ERROR_BUSY && !other);
  interceptor_gamepad_controller_init init{};Check(!interceptor_gamepad_controller_initialize(session,&init,&error)&&init.ready);
  interceptor_gamepad_controller_info info{};Check(!interceptor_gamepad_controller_info_get(session,&info,&error));
  Check(info.input_mask==TEST_INPUT_MASK && info.controller_mask==TEST_CONTROLLER_MASK);
  const interceptor_gamepad_controller_devices* devices=nullptr;Check(!interceptor_gamepad_controller_devices_get(session,&devices,&error)&&devices->count==expected);
  for(unsigned n=0;n<devices->count;++n)Check(Allowed(devices->items[n].kind));
  const interceptor_gamepad_controller_batch* batch=nullptr;Check(!interceptor_gamepad_controller_receive(session,0,&batch,&error)&&batch->count==expected);
  auto ticket=batch->id;const interceptor_gamepad_controller_batch* unused=nullptr;
  Check(interceptor_gamepad_controller_receive(session,0,&unused,&error)==ERROR_BUSY && !unused);
  for(unsigned n=0;n<batch->count;++n) {
   auto& event=batch->events[n];Check(Allowed(event.kind));Check(event.sequence>0x7fffffffffffffffull);
   for(unsigned f=0;f<event.field_count;++f) {
    auto& field=event.fields[f];
    if(event.kind==1 && !std::strcmp(field.name,"scan_code"))field.value=0x30;
    if(event.kind==2 && !std::strcmp(field.name,"y"))field.value=-7;
    if((event.kind==3 || event.kind==5) && field.group==INTERCEPTOR_GAMEPAD_CONTROLLER_AXIS && field.index==0){field.value=50;field.normalized=.5;}
    if(event.kind==4 && !std::strcmp(field.name,"left_x"))field.value=-100;
   }
  }
  Check(!interceptor_gamepad_controller_send(session,ticket,&error)&&error.accepted==expected);
  if(Allowed(1))Check(interceptor_gamepad_controller_test_seen(1,1)==0x30);
  if(Allowed(2))Check(interceptor_gamepad_controller_test_seen(2,1)==-7);
  if(Allowed(3))Check(interceptor_gamepad_controller_test_seen(3,1)==50 && interceptor_gamepad_controller_test_seen(3,2)==500);
  Check(!interceptor_gamepad_controller_send(session,ticket,&error)&&!error.accepted);
  Check(!interceptor_gamepad_controller_receive(session,0,&batch,&error));Check(interceptor_gamepad_controller_send(session,ticket,&error)==ERROR_INVALID_HANDLE);
  batch->events[0].blocked=1;Check(!interceptor_gamepad_controller_send(session,batch->id,&error)&&error.accepted==expected-1);
  Check(!interceptor_gamepad_controller_receive(session,0,&batch,&error));interceptor_gamepad_controller_test_mode(2);
  Check(interceptor_gamepad_controller_send(session,batch->id,&error)==ERROR_BUSY && error.accepted==1);
  Check(!interceptor_gamepad_controller_send(session,batch->id,&error)&&error.accepted==expected-1);interceptor_gamepad_controller_test_mode(0);
  Check(!interceptor_gamepad_controller_receive(session,0,&batch,&error));Check(!interceptor_gamepad_controller_discard(session,batch->id,&error));
  for(unsigned kind=1;kind<=6;++kind)if(!Allowed(kind)) {
   interceptor_gamepad_controller_event input{};input.kind=kind;Check(interceptor_gamepad_controller_inject(session,&input,1,&error)==ERROR_NOT_SUPPORTED);
  }
  if(Allowed(2)) {
   interceptor_gamepad_controller_field x{};x.name="x";x.value=123;interceptor_gamepad_controller_event input{};input.kind=2;input.fields=&x;input.field_count=1;
   Check(!interceptor_gamepad_controller_inject(session,&input,1,&error)&&error.accepted==1);
  }
  interceptor_gamepad_controller_blob blob{};Check(!interceptor_gamepad_controller_descriptor(session,devices->items[0].token,1,&blob,&error));
  interceptor_gamepad_controller_test_mode(1);uint32_t result=999;
  std::thread reader([&]{interceptor_gamepad_controller_error e{};const interceptor_gamepad_controller_batch* empty=nullptr;result=interceptor_gamepad_controller_receive(session,5000,&empty,&e);});
  Sleep(30);Check(!interceptor_gamepad_controller_cancel(session,&error));reader.join();Check(!result);
  Check(!interceptor_gamepad_controller_close(session,&error));Check(interceptor_gamepad_controller_close(session,&error)==ERROR_INVALID_HANDLE);
  interceptor_gamepad_controller_test_mode(0);Check(!interceptor_gamepad_controller_open(&session,&error));Check(!interceptor_gamepad_controller_close(session,&error));
  std::cout<<TEST_PRODUCT<<": C ABI, product masks, edits, blocking, retries, uint64, injection and cancellation passed\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}
}
