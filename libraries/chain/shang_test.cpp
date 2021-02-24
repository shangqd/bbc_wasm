
#include <eosio/chain/bbc.hpp>
#include <eosio/chain/webassembly/wabt.hpp>
#include <eosio/chain/webassembly/wavm.hpp>


#include "Runtime/Intrinsics.h"

namespace eosio { namespace chain { namespace webassembly { namespace wavm {
running_instance_context the_running_instance_context;
}}}}


class context_aware_api {
   public:
      context_aware_api()
      {
      }

      void checktime() {
      }
};

class context_free_api : public context_aware_api {
    public:
        context_free_api(eosio::chain::apply_context& ctx)
        {
        }

        int get_context_free_data(int a,int b) const
        {
            return a + b;
        }
};

REGISTER_INTRINSICS(context_free_api,
   (get_context_free_data, int(int,int) )
)

void shang()
{
   printf("bbc test OK \n");
}
/*
using namespace eosio::chain::webassembly::wavm;
static Intrinsics::Function __intrinsic_fn0 ( 
    "env.get_context_free_data", 
    wasm_function_type_provider<int(int,int)>::type(), 
    (void *)eosio::chain::webassembly::wavm::intrinsic_function_invoker_wrapper<int(int,int), 
        decltype(&context_free_api::get_context_free_data)>::type::fn<&context_free_api::get_context_free_data>() ); 


using namespace eosio::chain::webassembly::wabt_runtime;
static eosio::chain::webassembly::wabt_runtime::intrinsic_registrator __wabt_intrinsic_fn1 ( 
    "env", 
    "get_context_free_data", 
    wabt_function_type_provider<int(int,int)>::type(), 
    eosio::chain::webassembly::wabt_runtime::intrinsic_function_invoker_wrapper<
        decltype(&context_free_api::get_context_free_data)>::type::fn<&context_free_api::get_context_free_data>() );
*/