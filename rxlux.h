#include "RandomX/src/randomx.h"

#include <cstring>
#include <mutex>

//! rxlux hashing instance
//! barrystyle 30072020

class rxlux 
{
public:
         std::mutex rxlux_mutex;

         bool is_randomx_init{false};

         randomx_flags flags;
         randomx_vm* rx_vm = nullptr;
         randomx_cache* cache = nullptr;

         char *blkheader; //! ptr for blockheader
         char *seedhash;  //! ptr for seedhash
         char *result;    //! ptr for hashresult

         void setseed(char* thisseed) {
              std::lock_guard<std::mutex> guard(rxlux_mutex);
              if (!seedhash)
                  seedhash = (char*)malloc(32);
              memcpy(seedhash,thisseed,32);
         }

         bool hasseedchanged(char* thisseed) {
              std::lock_guard<std::mutex> guard(rxlux_mutex);
              bool changed = false;
              if (memcmp(seedhash,thisseed,32) != 0) {
                  setseed(thisseed);
                  changed = true;
              }
              return changed;
         }

         void rxinitialize(char* initseed) {
              //! lock provided by caller
              if (!initseed || is_randomx_init) return;
              if (!cache) {
                  flags = randomx_get_flags();
                  cache = randomx_alloc_cache(flags);
                  randomx_init_cache(cache, initseed, 32);
              }
              if (!rx_vm) {
                  result = (char*)malloc(32);
                  rx_vm = randomx_create_vm(flags, cache, nullptr);
              }
              is_randomx_init = true;
         }

         void rxseedrefresh() {
              //! lock provided by caller
              randomx_destroy_vm(rx_vm);
              randomx_release_cache(cache);
              cache = randomx_alloc_cache(flags);
              randomx_init_cache(cache, seedhash, 32);
              rx_vm = randomx_create_vm(flags, cache, nullptr);
         }

         void setheader(char* thisheader) {
              blkheader = thisheader;
         }

         char *getresult() {
              return result;
         }

         void rx_slow_hash(char* seedhash = nullptr) {
              std::lock_guard<std::mutex> guard(rxlux_mutex);
              if (!is_randomx_init)
                  rxinitialize(seedhash);
              if (seedhash && hasseedchanged(seedhash))
                  rxseedrefresh();
              randomx_calculate_hash(rx_vm, blkheader, 144, result);
         }
};
         
