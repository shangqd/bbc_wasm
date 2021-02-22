/*
#include <eosio/chain/wasm_interface.hpp>
#include <eosio/chain/apply_context.hpp>
#include <eosio/chain/controller.hpp>
#include <eosio/chain/transaction_context.hpp>
#include <eosio/chain/producer_schedule.hpp>
#include <eosio/chain/exceptions.hpp>
#include <boost/core/ignore_unused.hpp>
#include <eosio/chain/authorization_manager.hpp>
#include <eosio/chain/resource_limits.hpp>
#include <eosio/chain/wasm_interface_private.hpp>
#include <eosio/chain/wasm_eosio_validation.hpp>
#include <eosio/chain/wasm_eosio_injection.hpp>
#include <eosio/chain/global_property_object.hpp>
#include <eosio/chain/protocol_state_object.hpp>
#include <eosio/chain/account_object.hpp>
#include <fc/exception/exception.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/sha1.hpp>
#include <fc/io/raw.hpp>

#include <softfloat.hpp>
#include <compiler_builtins.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <string.h>

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


using namespace eosio::chain::webassembly::wavm;


template<typename T>
struct wasm_to_rvalue_type_shang;

template<>
struct wasm_to_rvalue_type_shang<F32> {
   static constexpr auto value = ResultType::f32;
};
template<>
struct wasm_to_rvalue_type_shang<F64> {
   static constexpr auto value = ResultType::f64;
};
template<>
struct wasm_to_rvalue_type_shang<I32> {
   static constexpr auto value = ResultType::i32;
};
template<>
struct wasm_to_rvalue_type_shang<I64> {
   static constexpr auto value = ResultType::i64;
};
template<>
struct wasm_to_rvalue_type_shang<void> {
   static constexpr auto value = ResultType::none;
};

template<>
struct wasm_to_rvalue_type_shang<char*> {
   static constexpr auto value = ResultType::i32;
};

template<typename T>
constexpr auto wasm_to_rvalue_type_v_shang = wasm_to_rvalue_type_shang<T>::value;


template<typename T>
struct wasm_to_value_type_shang;

template<>
struct wasm_to_value_type_shang<F32> {
   static constexpr auto value = ValueType::f32;
};

template<>
struct wasm_to_value_type_shang<F64> {
   static constexpr auto value = ValueType::f64;
};
template<>
struct wasm_to_value_type_shang<I32> {
   static constexpr auto value = ValueType::i32;
};
template<>
struct wasm_to_value_type_shang<I64> {
   static constexpr auto value = ValueType::i64;
};


template<typename T>
constexpr auto wasm_to_value_type_v_shang = wasm_to_value_type_shang<T>::value;


template<typename>
struct wasm_function_type_provider_shang;

template<typename Ret, typename ...Args>
struct wasm_function_type_provider_shang<Ret(Args...)> {
   static const FunctionType *type() {
      return FunctionType::get(wasm_to_rvalue_type_v_shang<Ret>, {wasm_to_value_type_v_shang<Args> ...});
   }
};


using namespace eosio::chain::webassembly::wavm;


template<typename WasmSig, typename Ret, typename MethodSig, typename Cls, typename... Params>
struct intrinsic_function_invoker_shang {
    
   using impl = intrinsic_invoker_impl<Ret, std::tuple<Params...>, std::tuple<>>;

   template<MethodSig Method>
   static Ret wrapper(running_instance_context& ctx, Params... params) {
        eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).checktime();
      return (eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).*Method)(params...);
   }

   template<MethodSig Method>
   static const WasmSig *fn() {
      auto fn = impl::template fn<wrapper<Method>>();
      static_assert(std::is_same<WasmSig *, decltype(fn)>::value,
                    "Intrinsic function signature does not match the ABI");
      return fn;
   }
};

template<typename WasmSig, typename MethodSig, typename Cls, typename... Params>
struct intrinsic_function_invoker_shang<WasmSig, void, MethodSig, Cls, Params...> {
   using impl = intrinsic_invoker_impl<void_type, std::tuple<Params...>, std::tuple<>>;

   template<MethodSig Method>
   static void_type wrapper(running_instance_context& ctx, Params... params) {
      eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).checktime();
      (eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).*Method)(params...);
      return void_type();
   }

   template<MethodSig Method>
   static const WasmSig *fn() {
      auto fn = impl::template fn<wrapper<Method>>();
      static_assert(std::is_same<WasmSig *, decltype(fn)>::value,
                    "Intrinsic function signature does not match the ABI");
      return fn;
   }
};


template<typename, typename>
struct intrinsic_function_invoker_wrapper_shang;

template<typename WasmSig, typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_shang<WasmSig, Ret (Cls::*)(Params...)> {
   using type = intrinsic_function_invoker_shang<WasmSig, Ret, Ret (Cls::*)(Params...), Cls, Params...>;
};

template<typename WasmSig, typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_shang<WasmSig, Ret (Cls::*)(Params...) const> {
   using type = intrinsic_function_invoker_shang<WasmSig, Ret, Ret (Cls::*)(Params...) const, Cls, Params...>;
};

template<typename WasmSig, typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_shang<WasmSig, Ret (Cls::*)(Params...) volatile> {
   using type = intrinsic_function_invoker_shang<WasmSig, Ret, Ret (Cls::*)(Params...) volatile, Cls, Params...>;
};

template<typename WasmSig, typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_shang<WasmSig, Ret (Cls::*)(Params...) const volatile> {
   using type = intrinsic_function_invoker_shang<WasmSig, Ret, Ret (Cls::*)(Params...) const volatile, Cls, Params...>;
};


static Intrinsics::Function __intrinsic_fn0 ( 
    "env.get_context_free_data", 
    wasm_function_type_provider_shang<int(int,int)>::type(), 
    (void *)intrinsic_function_invoker_wrapper_shang<int(int,int), 
        decltype(&context_free_api::get_context_free_data)>::type::fn<&context_free_api::get_context_free_data>() ); 



// ================================================================================

using namespace eosio::chain::webassembly::wabt_runtime;


template<typename T>
struct wabt_to_value_type_shang;

template<>
struct wabt_to_value_type_shang<F32> {
   static constexpr auto value = Type::F32;
};

template<>
struct wabt_to_value_type_shang<F64> {
   static constexpr auto value = Type::F64;
};
template<>
struct wabt_to_value_type_shang<I32> {
   static constexpr auto value = Type::I32;
};
template<>
struct wabt_to_value_type_shang<I64> {
   static constexpr auto value = Type::I64;
};

template<typename T>
constexpr auto wabt_to_value_type_v_shang = wabt_to_value_type_shang<T>::value;


template<typename T>
struct wabt_to_rvalue_type_shang;
template<>
struct wabt_to_rvalue_type_shang<F32> {
   static constexpr auto value = Type::F32;
};
template<>
struct wabt_to_rvalue_type_shang<F64> {
   static constexpr auto value = Type::F64;
};
template<>
struct wabt_to_rvalue_type_shang<I32> {
   static constexpr auto value = Type::I32;
};
template<>
struct wabt_to_rvalue_type_shang<I64> {
   static constexpr auto value = Type::I64;
};

template<>
struct wabt_to_rvalue_type_shang<char*> {
   static constexpr auto value = Type::I32;
};

template<typename T>
constexpr auto wabt_to_rvalue_type_v_shang = wabt_to_rvalue_type_shang<T>::value;

template<typename>
struct wabt_function_type_provider_shang;

template<typename Ret, typename ...Args>
struct wabt_function_type_provider_shang<Ret(Args...)> {
   static FuncSignature type() {
      return FuncSignature({wabt_to_value_type_v_shang<Args> ...}, {wabt_to_rvalue_type_v_shang<Ret>});
   }
};
template<typename ...Args>
struct wabt_function_type_provider_shang<void(Args...)> {
   static FuncSignature type() {
      return FuncSignature({wabt_to_value_type_v_shang<Args> ...}, {});
   }
};



using namespace eosio::chain::webassembly::wabt_runtime;

template<typename WasmSig, typename Ret, typename MethodSig, typename Cls, typename... Params>
struct intrinsic_function_invoker_qing {
   using impl = eosio::chain::webassembly::wavm::intrinsic_invoker_impl<Ret, std::tuple<Params...>, std::tuple<>>;

   template<MethodSig Method>
   static Ret wrapper(running_instance_context& ctx, Params... params) {
      eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).checktime();
      return (eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).*Method)(params...);
   }

   template<MethodSig Method>
   static const WasmSig *fn() {
      auto fn = impl::template fn<wrapper<Method>>();
      static_assert(std::is_same<WasmSig *, decltype(fn)>::value,
                    "Intrinsic function signature does not match the ABI");
      return fn;
   }
};


template<typename WasmSig, typename MethodSig, typename Cls, typename... Params>
struct intrinsic_function_invoker_qing<WasmSig, void, MethodSig, Cls, Params...> {
   using impl = eosio::chain::webassembly::wavm::intrinsic_invoker_impl<eosio::chain::webassembly::wavm::void_type, std::tuple<Params...>, std::tuple<>>;

   template<MethodSig Method>
   static eosio::chain::webassembly::wavm::void_type wrapper(running_instance_context& ctx, Params... params) {
      eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).checktime();
      (eosio::chain::class_from_wasm<Cls>::value(*ctx.apply_ctx).*Method)(params...);
      return eosio::chain::webassembly::wavm::void_type();
   }

   template<MethodSig Method>
   static const WasmSig *fn() {
      auto fn = impl::template fn<wrapper<Method>>();
      static_assert(std::is_same<WasmSig *, decltype(fn)>::value,
                    "Intrinsic function signature does not match the ABI");
      return fn;
   }
};


template<typename>
struct intrinsic_function_invoker_wrapper_qing;

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_qing<Ret (Cls::*)(Params...)> {
   using type = intrinsic_function_invoker_qing<Ret, Ret (Cls::*)(Params...), Cls, Params...>;
};

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_qing<Ret (Cls::*)(Params...) const> {
   using type = intrinsic_function_invoker_qing<Ret, Ret (Cls::*)(Params...) const, Cls, Params...>;
};

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_qing<Ret (Cls::*)(Params...) volatile> {
   using type = intrinsic_function_invoker_qing<Ret, Ret (Cls::*)(Params...) volatile, Cls, Params...>;
};

template<typename Cls, typename Ret, typename... Params>
struct intrinsic_function_invoker_wrapper_qing<Ret (Cls::*)(Params...) const volatile> {
   using type = intrinsic_function_invoker_qing<Ret, Ret (Cls::*)(Params...) const volatile, Cls, Params...>;
};


using namespace eosio::chain::webassembly::wabt_runtime;

static eosio::chain::webassembly::wabt_runtime::intrinsic_registrator __wabt_intrinsic_fn1 ( 
    "env", 
    "get_context_free_data", 
    wabt_function_type_provider_shang<int(int,int)>::type(), 
    eosio::chain::webassembly::wabt_runtime::intrinsic_function_invoker_wrapper<
        decltype(&context_free_api::get_context_free_data)>::type::fn<&context_free_api::get_context_free_data>() );
*/