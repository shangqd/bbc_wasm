
#include <eosio/chain/bbc.hpp>
#include <eosio/chain/webassembly/wabt.hpp>
#include <eosio/chain/webassembly/wavm.hpp>
#include "Runtime/Linker.h"
#include "Runtime/Runtime.h"

#include "Runtime/Intrinsics.h"

namespace eosio { namespace chain { namespace webassembly { namespace wavm {
running_instance_context the_running_instance_context;
}}}}


namespace eosio { namespace chain {
class context_aware_api {
   public:
      context_aware_api(apply_context& ctx, bool context_free = false )
      :context(ctx)
      {
         //std::cout << "context_aware_api OK" << std::endl;
         //if( context.is_context_free() )
         //   EOS_ASSERT( context_free, unaccessible_api, "only context free api's can be used in this context" );
      }

      void checktime() {
         //context.trx_context.checktime();
      }

   protected:
      apply_context&             context;

};


class context_free_api : public context_aware_api {
   public:
      context_free_api( apply_context& ctx )
      :context_aware_api(ctx, true) {
         /* the context_free_data is not available during normal application because it is prunable */
         //EOS_ASSERT( context.is_context_free(), unaccessible_api, "this API may only be called from context_free apply" );
      }

      int get_context_free_data( uint32_t index, array_ptr<char> buffer, size_t buffer_size )const {
         //return context.get_context_free_data( index, buffer, buffer_size );
         return 0;
      }
};

class privileged_api : public context_aware_api {
   public:
      privileged_api( apply_context& ctx )
      :context_aware_api(ctx)
      {
         //EOS_ASSERT( context.is_privileged(), unaccessible_api, "${code} does not have permission to call this API", ("code",context.get_receiver()) );
      }

      /**
       * This should return true if a feature is active and irreversible, false if not.
       *
       * Irreversiblity by fork-database is not consensus safe, therefore, this defines
       * irreversiblity only by block headers not by BFT short-cut.
       */
      int is_feature_active( int64_t feature_name ) {
         return false;
      }

      /**
       *  This should schedule the feature to be activated once the
       *  block that includes this call is irreversible. It should
       *  fail if the feature is already pending.
       *
       *  Feature name should be base32 encoded name.
       */
      void activate_feature( int64_t feature_name ) {
         //EOS_ASSERT( false, unsupported_feature, "Unsupported Hardfork Detected" );
      }

      /**
       *  Pre-activates the specified protocol feature.
       *  Fails if the feature is unrecognized, disabled, or not allowed to be activated at the current time.
       *  Also fails if the feature was already activated or pre-activated.
       */
      void preactivate_feature( const fc::sha256& feature_digest ) {
         //context.control.preactivate_feature( feature_digest );
      }

      /**
       * update the resource limits associated with an account.  Note these new values will not take effect until the
       * next resource "tick" which is currently defined as a cycle boundary inside a block.
       *
       * @param account - the account whose limits are being modified
       * @param ram_bytes - the limit for ram bytes
       * @param net_weight - the weight for determining share of network capacity
       * @param cpu_weight - the weight for determining share of compute capacity
       */
      void set_resource_limits( name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
         //EOS_ASSERT(ram_bytes >= -1, wasm_execution_error, "invalid value for ram resource limit expected [-1,INT64_MAX]");
         //EOS_ASSERT(net_weight >= -1, wasm_execution_error, "invalid value for net resource weight expected [-1,INT64_MAX]");
         //EOS_ASSERT(cpu_weight >= -1, wasm_execution_error, "invalid value for cpu resource weight expected [-1,INT64_MAX]");
         //if( context.control.get_mutable_resource_limits_manager().set_account_limits(account, ram_bytes, net_weight, cpu_weight) ) {
         //   context.trx_context.validate_ram_usage.insert( account );
         //}
      }

      void get_resource_limits(name account, int64_t& ram_bytes, int64_t& net_weight, int64_t& cpu_weight ) {
         //context.control.get_resource_limits_manager().get_account_limits( account, ram_bytes, net_weight, cpu_weight);
      }

      int64_t set_proposed_producers( array_ptr<char> packed_producer_schedule, size_t datalen) {
         //datastream<const char*> ds( packed_producer_schedule, datalen );
         //vector<producer_key> producers;
         //fc::raw::unpack(ds, producers);
         //EOS_ASSERT( producers.size() > 0
         //               || !context.control.is_builtin_activated(
         //                     builtin_protocol_feature_t::disallow_empty_producer_schedule
         //                  ),
         //            wasm_execution_error,
         //            "Producer schedule cannot be empty"
         //);
         //EOS_ASSERT(producers.size() <= config::max_producers, wasm_execution_error, "Producer schedule exceeds the maximum producer count for this chain");
         // check that producers are unique
         //std::set<account_name> unique_producers;
         //for (const auto& p: producers) {
         //   EOS_ASSERT( context.is_account(p.producer_name), wasm_execution_error, "producer schedule includes a nonexisting account" );
         //   EOS_ASSERT( p.block_signing_key.valid(), wasm_execution_error, "producer schedule includes an invalid key" );
         //   unique_producers.insert(p.producer_name);
         //}
         //EOS_ASSERT( producers.size() == unique_producers.size(), wasm_execution_error, "duplicate producer name in producer schedule" );
         //return context.control.set_proposed_producers( std::move(producers) );
         return 0;
      }

      uint32_t get_blockchain_parameters_packed( array_ptr<char> packed_blockchain_parameters, size_t buffer_size) {
         //auto& gpo = context.control.get_global_properties();
         //auto s = fc::raw::pack_size( gpo.configuration );
         //if( buffer_size == 0 ) return s;
         //if ( s <= buffer_size ) {
         //   datastream<char*> ds( packed_blockchain_parameters, s );
         //   fc::raw::pack(ds, gpo.configuration);
         //   return s;
         //}
         return 0;
      }

      void set_blockchain_parameters_packed( array_ptr<char> packed_blockchain_parameters, size_t datalen) {
         //datastream<const char*> ds( packed_blockchain_parameters, datalen );
         //chain::chain_config cfg;
         //fc::raw::unpack(ds, cfg);
         //cfg.validate();
         //context.db.modify( context.control.get_global_properties(),
         //   [&]( auto& gprops ) {
         //        gprops.configuration = cfg;
         //});
      }

      bool is_privileged( name n )const {
         //return context.db.get<account_metadata_object, by_name>( n ).is_privileged();
         return true;
      }

      void set_privileged( name n, bool is_priv ) {
         //const auto& a = context.db.get<account_metadata_object, by_name>( n );
         //context.db.modify( a, [&]( auto& ma ){
         //   ma.set_privileged( is_priv );
         //});
      }

};

class transaction_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

      void send_inline( array_ptr<char> data, size_t data_len ) {
         /*
         //TODO: Why is this limit even needed? And why is it not consistently checked on actions in input or deferred transactions
         EOS_ASSERT( data_len < context.control.get_global_properties().configuration.max_inline_action_size, inline_action_too_big,
                    "inline action too big" );
         action act;
         fc::raw::unpack<action>(data, data_len, act);
         context.execute_inline(std::move(act));
         */
      }

      void send_context_free_inline( array_ptr<char> data, size_t data_len ) {
         /*
         //TODO: Why is this limit even needed? And why is it not consistently checked on actions in input or deferred transactions
         EOS_ASSERT( data_len < context.control.get_global_properties().configuration.max_inline_action_size, inline_action_too_big,
                   "inline action too big" );
         action act;
         fc::raw::unpack<action>(data, data_len, act);
         context.execute_context_free_inline(std::move(act));
         */
      }

      void send_deferred( const unsigned __int128& sender_id, name payer, array_ptr<char> data, size_t data_len, uint32_t replace_existing) {
         /*
         transaction trx;
         fc::raw::unpack<transaction>(data, data_len, trx);
         context.schedule_deferred_transaction(sender_id, payer, std::move(trx), replace_existing);
         */
      }

      bool cancel_deferred( const unsigned __int128& val ) {
         /*
         fc::uint128_t sender_id(val>>64, uint64_t(val) );
         return context.cancel_deferred_transaction( (unsigned __int128)sender_id );
         */
        return true;
      }
};

class memory_api : public context_aware_api {
   public:
      memory_api( apply_context& ctx )
      :context_aware_api(ctx,true){}

      char* memcpy( array_ptr<char> dest, array_ptr<const char> src, size_t length) {
         //EOS_ASSERT((size_t)(std::abs((ptrdiff_t)dest.value - (ptrdiff_t)src.value)) >= length,
         //      overlapping_memory_error, "memcpy can only accept non-aliasing pointers");
         return (char *)::memcpy(dest, src, length);
      }

      char* memmove( array_ptr<char> dest, array_ptr<const char> src, size_t length) {
         return (char *)::memmove(dest, src, length);
      }

      int memcmp( array_ptr<const char> dest, array_ptr<const char> src, size_t length) {
         int ret = ::memcmp(dest, src, length);
         if(ret < 0)
            return -1;
         if(ret > 0)
            return 1;
         return 0;
      }

      char* memset( array_ptr<char> dest, int value, size_t length ) {
         return (char *)::memset( dest, value, length );
      }
};

class context_free_transaction_api : public context_aware_api {
   public:
      context_free_transaction_api( apply_context& ctx )
      :context_aware_api(ctx,true){}

      int read_transaction( array_ptr<char> data, size_t buffer_size ) {
         /*
         bytes trx = context.get_packed_transaction();
         auto s = trx.size();
         if( buffer_size == 0) return s;
         auto copy_size = std::min( buffer_size, s );
         memcpy( data, trx.data(), copy_size );
         return copy_size;
         */
        return 0;
      }

      int transaction_size() {
         //return context.get_packed_transaction().size();
         return 0;
      }

      int expiration() {
        //return context.trx_context.trx.expiration.sec_since_epoch();
        return 0;
      }

      int tapos_block_num() {
        //return context.trx_context.trx.ref_block_num;
        return 0;
      }
      int tapos_block_prefix() {
        //return context.trx_context.trx.ref_block_prefix;
        return 0;
      }

      int get_action( uint32_t type, uint32_t index, array_ptr<char> buffer, size_t buffer_size )const {
         //return context.get_action( type, index, buffer, buffer_size );
         return 0;
      }
};

class system_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

      uint64_t current_time() {
         //return static_cast<uint64_t>( context.control.pending_block_time().time_since_epoch().count() );
         return 0;
      }

      uint64_t publication_time() {
         //return static_cast<uint64_t>( context.trx_context.published.time_since_epoch().count() );
         return 0;
      }

      /**
       * Returns true if the specified protocol feature is activated, false if not.
       */
      bool is_feature_activated( const fc::sha256& feature_digest ) {
         //return context.control.is_protocol_feature_activated( feature_digest );
         return true;
      }

      name get_sender() {
         return name{123};// context.get_sender();
      }
};

class permission_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

      bool check_transaction_authorization( array_ptr<char> trx_data,     size_t trx_size,
                                            array_ptr<char> pubkeys_data, size_t pubkeys_size,
                                            array_ptr<char> perms_data,   size_t perms_size
                                          )
      {
         return false;
      }

      bool check_permission_authorization( name account, name permission,
                                           array_ptr<char> pubkeys_data, size_t pubkeys_size,
                                           array_ptr<char> perms_data,   size_t perms_size,
                                           uint64_t delay_us
                                         )
      {
         return false;
      }

      int64_t get_permission_last_used( name account, name permission ) {
         //const auto& am = context.control.get_authorization_manager();
         //return am.get_permission_last_used( am.get_permission({account, permission}) ).time_since_epoch().count();
         return 0;
      };

      int64_t get_account_creation_time( name account ) {
         //auto* acct = context.db.find<account_object, by_name>(account);
         //EOS_ASSERT( acct != nullptr, action_validate_exception,
         //            "account '${account}' does not exist", ("account", account) );
         //return time_point(acct->creation_date).time_since_epoch().count();
         return 0;
      }

};

class database_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

      int db_store_i64( uint64_t scope, uint64_t table, uint64_t payer, uint64_t id, array_ptr<const char> buffer, size_t buffer_size ) {
         //return context.db_store_i64( scope, table, payer, id, buffer, buffer_size );
         return 0;
      }
      void db_update_i64( int itr, uint64_t payer, array_ptr<const char> buffer, size_t buffer_size ) {
         //context.db_update_i64( itr, payer, buffer, buffer_size );
      }
      void db_remove_i64( int itr ) {
         //context.db_remove_i64( itr );
      }
      int db_get_i64( int itr, array_ptr<char> buffer, size_t buffer_size ) {
         //return context.db_get_i64( itr, buffer, buffer_size );
         return 0;
      }
      int db_next_i64( int itr, uint64_t& primary ) {
         //return context.db_next_i64(itr, primary);
         return 0;
      }
      int db_previous_i64( int itr, uint64_t& primary ) {
         //return context.db_previous_i64(itr, primary);
         return 0;
      }
      int db_find_i64( uint64_t code, uint64_t scope, uint64_t table, uint64_t id ) {
         //return context.db_find_i64( code, scope, table, id );
         return 0;
      }
      int db_lowerbound_i64( uint64_t code, uint64_t scope, uint64_t table, uint64_t id ) {
         //return context.db_lowerbound_i64( code, scope, table, id );
         return 0;
      }
      int db_upperbound_i64( uint64_t code, uint64_t scope, uint64_t table, uint64_t id ) {
         //return context.db_upperbound_i64( code, scope, table, id );
         return 0;
      }
      int db_end_i64( uint64_t code, uint64_t scope, uint64_t table ) {
         //return context.db_end_i64( code, scope, table );
         return 0;
      }
};

class compiler_builtins : public context_aware_api {
   public:
      compiler_builtins( apply_context& ctx )
      :context_aware_api(ctx,true){}

      void __ashlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
         //fc::uint128_t i(high, low);
         //i <<= shift;
         ret = 0;// (unsigned __int128)i;
      }

      void __ashrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
         // retain the signedness
         ret = high;
         ret <<= 64;
         ret |= low;
         ret >>= shift;
      }

      void __lshlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
         //fc::uint128_t i(high, low);
         //i <<= shift;
         ret = 0;//(unsigned __int128)i;
      }

      void __lshrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
         //fc::uint128_t i(high, low);
         //i >>= shift;
         ret =0;// (unsigned __int128)i;
      }

      void __divti3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
         __int128 lhs = ha;
         __int128 rhs = hb;

         lhs <<= 64;
         lhs |=  la;

         rhs <<= 64;
         rhs |=  lb;

         //EOS_ASSERT(rhs != 0, arithmetic_exception, "divide by zero");

         lhs /= rhs;

         ret = lhs;
      }

      void __udivti3(unsigned __int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
         unsigned __int128 lhs = ha;
         unsigned __int128 rhs = hb;

         lhs <<= 64;
         lhs |=  la;

         rhs <<= 64;
         rhs |=  lb;

         //EOS_ASSERT(rhs != 0, arithmetic_exception, "divide by zero");

         lhs /= rhs;
         ret = lhs;
      }

      void __multi3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
         __int128 lhs = ha;
         __int128 rhs = hb;

         lhs <<= 64;
         lhs |=  la;

         rhs <<= 64;
         rhs |=  lb;

         lhs *= rhs;
         ret = lhs;
      }

      void __modti3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
         __int128 lhs = ha;
         __int128 rhs = hb;

         lhs <<= 64;
         lhs |=  la;

         rhs <<= 64;
         rhs |=  lb;

         //EOS_ASSERT(rhs != 0, arithmetic_exception, "divide by zero");

         lhs %= rhs;
         ret = lhs;
      }

      void __umodti3(unsigned __int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
         unsigned __int128 lhs = ha;
         unsigned __int128 rhs = hb;

         lhs <<= 64;
         lhs |=  la;

         rhs <<= 64;
         rhs |=  lb;

         //EOS_ASSERT(rhs != 0, arithmetic_exception, "divide by zero");

         lhs %= rhs;
         ret = lhs;
      }

      // arithmetic long double
      void __addtf3( float128_t& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         float128_t a = {{ la, ha }};
         float128_t b = {{ lb, hb }};
         ret = f128_add( a, b );
      }
      void __subtf3( float128_t& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         float128_t a = {{ la, ha }};
         float128_t b = {{ lb, hb }};
         ret = f128_sub( a, b );
      }
      void __multf3( float128_t& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         float128_t a = {{ la, ha }};
         float128_t b = {{ lb, hb }};
         ret = f128_mul( a, b );
      }
      void __divtf3( float128_t& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         float128_t a = {{ la, ha }};
         float128_t b = {{ lb, hb }};
         ret = f128_div( a, b );
      }
      void __negtf2( float128_t& ret, uint64_t la, uint64_t ha ) {
         ret = {{ la, (ha ^ (uint64_t)1 << 63) }};
      }

      // conversion long double
      void __extendsftf2( float128_t& ret, float f ) {
         ret = f32_to_f128( to_softfloat32(f) );
      }
      void __extenddftf2( float128_t& ret, double d ) {
         ret = f64_to_f128( to_softfloat64(d) );
      }
      double __trunctfdf2( uint64_t l, uint64_t h ) {
         float128_t f = {{ l, h }};
         return from_softfloat64(f128_to_f64( f ));
      }
      float __trunctfsf2( uint64_t l, uint64_t h ) {
         float128_t f = {{ l, h }};
         return from_softfloat32(f128_to_f32( f ));
      }
      int32_t __fixtfsi( uint64_t l, uint64_t h ) {
         float128_t f = {{ l, h }};
         return f128_to_i32( f, 0, false );
      }
      int64_t __fixtfdi( uint64_t l, uint64_t h ) {
         float128_t f = {{ l, h }};
         return f128_to_i64( f, 0, false );
      }
      void __fixtfti( __int128& ret, uint64_t l, uint64_t h ) {
         //float128_t f = {{ l, h }};
         ret = 0;// ___fixtfti( f );
      }
      uint32_t __fixunstfsi( uint64_t l, uint64_t h ) {
         float128_t f = {{ l, h }};
         return f128_to_ui32( f, 0, false );
      }
      uint64_t __fixunstfdi( uint64_t l, uint64_t h ) {
         float128_t f = {{ l, h }};
         return f128_to_ui64( f, 0, false );
      }
      void __fixunstfti( unsigned __int128& ret, uint64_t l, uint64_t h ) {
         //float128_t f = {{ l, h }};
         ret = 0;//___fixunstfti( f );
      }
      void __fixsfti( __int128& ret, float a ) {
         ret = 0;//___fixsfti( to_softfloat32(a).v );
      }
      void __fixdfti( __int128& ret, double a ) {
         ret = 0;//___fixdfti( to_softfloat64(a).v );
      }
      void __fixunssfti( unsigned __int128& ret, float a ) {
         ret = 0;//___fixunssfti( to_softfloat32(a).v );
      }
      void __fixunsdfti( unsigned __int128& ret, double a ) {
         ret = 0;// ___fixunsdfti( to_softfloat64(a).v );
      }
      double __floatsidf( int32_t i ) {
         return from_softfloat64(i32_to_f64(i));
      }
      void __floatsitf( float128_t& ret, int32_t i ) {
         ret = i32_to_f128(i);
      }
      void __floatditf( float128_t& ret, uint64_t a ) {
         ret = i64_to_f128( a );
      }
      void __floatunsitf( float128_t& ret, uint32_t i ) {
         ret = ui32_to_f128(i);
      }
      void __floatunditf( float128_t& ret, uint64_t a ) {
         ret = ui64_to_f128( a );
      }
      double __floattidf( uint64_t l, uint64_t h ) {
         return 0.0;
      }
      double __floatuntidf( uint64_t l, uint64_t h ) {
         return 0.0;
      }
      int ___cmptf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb, int return_value_if_nan ) {
         float128_t a = {{ la, ha }};
         float128_t b = {{ lb, hb }};
         if ( __unordtf2(la, ha, lb, hb) )
            return return_value_if_nan;
         if ( f128_lt( a, b ) )
            return -1;
         if ( f128_eq( a, b ) )
            return 0;
         return 1;
      }
      int __eqtf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, 1);
      }
      int __netf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, 1);
      }
      int __getf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, -1);
      }
      int __gttf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, 0);
      }
      int __letf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, 1);
      }
      int __lttf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, 0);
      }
      int __cmptf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         return ___cmptf2(la, ha, lb, hb, 1);
      }
      int __unordtf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ) {
         //float128_t a = {{ la, ha }};
         //float128_t b = {{ lb, hb }};
         //if ( softfloat_api::is_nan(a) || softfloat_api::is_nan(b) )
         //   return 1;
         return 0;
      }

      static constexpr uint32_t SHIFT_WIDTH = (sizeof(uint64_t)*8)-1;
};

class console_api : public context_aware_api {
   public:
      console_api( apply_context& ctx )
      : context_aware_api(ctx,true)
      , ignore(false) {}
      //, ignore(!ctx.control.contracts_console()) {}

      // Kept as intrinsic rather than implementing on WASM side (using prints_l and strlen) because strlen is faster on native side.
      void prints(null_terminated_ptr str) {
         if ( !ignore ) {
            //context.console_append( static_cast<const char*>(str) );
            printf("%s\n", static_cast<const char*>(str));
         }
      }

      void prints_l(array_ptr<const char> str, size_t str_len ) {
         if ( !ignore ) {
            //context.console_append(string(str, str_len));
         }
      }

      void printi(int64_t val) {
         if ( !ignore ) {
            //std::ostringstream oss;
            //oss << val;
            //context.console_append( oss.str() );
         }
      }

      void printui(uint64_t val) {
         if ( !ignore ) {
            //std::ostringstream oss;
            //oss << val;
            //context.console_append( oss.str() );
         }
      }

      void printi128(const __int128& val) {
         if ( !ignore ) {
            /*
            bool is_negative = (val < 0);
            unsigned __int128 val_magnitude;
            if( is_negative )
               val_magnitude = static_cast<unsigned __int128>(-val); // Works even if val is at the lowest possible value of a int128_t
            else
               val_magnitude = static_cast<unsigned __int128>(val);
            fc::uint128_t v(val_magnitude>>64, static_cast<uint64_t>(val_magnitude) );
            string s;
            if( is_negative ) {
               s += '-';
            }
            s += fc::variant(v).get_string();
            context.console_append( s );
            */
         }
      }

      void printui128(const unsigned __int128& val) {
         if ( !ignore ) {
            //fc::uint128_t v(val>>64, static_cast<uint64_t>(val) );
            //context.console_append(fc::variant(v).get_string());
         }
      }

      void printsf( float val ) {
         if ( !ignore ) {
            // Assumes float representation on native side is the same as on the WASM side
            //std::ostringstream oss;
            //oss.setf( std::ios::scientific, std::ios::floatfield );
            //oss.precision( std::numeric_limits<float>::digits10 );
            //oss << val;
            //context.console_append( oss.str() );
         }
      }

      void printdf( double val ) {
         if ( !ignore ) {
            // Assumes double representation on native side is the same as on the WASM side
            //std::ostringstream oss;
            //oss.setf( std::ios::scientific, std::ios::floatfield );
            //oss.precision( std::numeric_limits<double>::digits10 );
            //oss << val;
            //context.console_append( oss.str() );
         }
      }

      void printqf( const float128_t& val ) {
         /*
          * Native-side long double uses an 80-bit extended-precision floating-point number.
          * The easiest solution for now was to use the Berkeley softfloat library to round the 128-bit
          * quadruple-precision floating-point number to an 80-bit extended-precision floating-point number
          * (losing precision) which then allows us to simply cast it into a long double for printing purposes.
          *
          * Later we might find a better solution to print the full quadruple-precision floating-point number.
          * Maybe with some compilation flag that turns long double into a quadruple-precision floating-point number,
          * or maybe with some library that allows us to print out quadruple-precision floating-point numbers without
          * having to deal with long doubles at all.
          */

         if ( !ignore ) {
            //std::ostringstream oss;
            //oss.setf( std::ios::scientific, std::ios::floatfield );
/*
#ifdef __x86_64__
            oss.precision( std::numeric_limits<long double>::digits10 );
            extFloat80_t val_approx;
            f128M_to_extF80M(&val, &val_approx);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
            oss << *(long double*)(&val_approx);
#pragma GCC diagnostic pop
#else
            oss.precision( std::numeric_limits<double>::digits10 );
            double val_approx = from_softfloat64( f128M_to_f64(&val) );
            oss << val_approx;
#endif
            context.console_append( oss.str() );*/
         }
      }

      void printn(const name& value) {
         if ( !ignore ) {
            //context.console_append(value.to_string());
         }
      }

      void printhex(array_ptr<const char> data, size_t data_len ) {
         if ( !ignore ) {
            //context.console_append(fc::to_hex(data, data_len));
         }
      }

   private:
      bool ignore;
};

class authorization_api : public context_aware_api {
   public:
      using context_aware_api::context_aware_api;

   void require_authorization( const name& account ) {
      //context.require_authorization( account );
   }

   bool has_authorization( const name& account )const {
      //return context.has_authorization( account );
      return true;
   }

   void require_authorization(const name& account,
                                                 const name& permission) {
      //context.require_authorization( account, permission );
   }

   void require_recipient( name recipient ) {
      //context.require_recipient( recipient );
   }

   bool is_account( const name& account )const {
      //return context.is_account( account );
      return true;
   }

};

class context_free_system_api :  public context_aware_api {
public:
   explicit context_free_system_api( apply_context& ctx )
   :context_aware_api(ctx,true){}

   void abort() {
      //EOS_ASSERT( false, abort_called, "abort() called");
   }

   // Kept as intrinsic rather than implementing on WASM side (using eosio_assert_message and strlen) because strlen is faster on native side.
   void eosio_assert( bool condition, null_terminated_ptr msg ) {
      /*
      if( BOOST_UNLIKELY( !condition ) ) {
         const size_t sz = strnlen( msg, max_assert_message );
         std::string message( msg, sz );
         EOS_THROW( eosio_assert_message_exception, "assertion failure with message: ${s}", ("s",message) );
      }*/
   }

   void eosio_assert_message( bool condition, array_ptr<const char> msg, size_t msg_len ) {
      /*
      if( BOOST_UNLIKELY( !condition ) ) {
         const size_t sz = msg_len > max_assert_message ? max_assert_message : msg_len;
         std::string message( msg, sz );
         EOS_THROW( eosio_assert_message_exception, "assertion failure with message: ${s}", ("s",message) );
      }*/
   }

   void eosio_assert_code( bool condition, uint64_t error_code ) {
      /*
      if( BOOST_UNLIKELY( !condition ) ) {
         if( error_code >= static_cast<uint64_t>(system_error_code::generic_system_error) ) {
            restricted_error_code_exception e( FC_LOG_MESSAGE(
                                                   error,
                                                   "eosio_assert_code called with reserved error code: ${error_code}",
                                                   ("error_code", error_code)
            ) );
            e.error_code = static_cast<uint64_t>(system_error_code::contract_restricted_error_code);
            throw e;
         } else {
            eosio_assert_code_exception e( FC_LOG_MESSAGE(
                                             error,
                                             "assertion failure with error code: ${error_code}",
                                             ("error_code", error_code)
            ) );
            e.error_code = error_code;
            throw e;
         }
      }*/
   }

   void eosio_exit(int32_t code) {
      //context.control.get_wasm_interface().exit();
   }

};

class action_api : public context_aware_api {
   public:
   action_api( apply_context& ctx )
      :context_aware_api(ctx,true){}

      int read_action_data(array_ptr<char> memory, size_t buffer_size) {
         /*
         auto s = context.get_action().data.size();
         if( buffer_size == 0 ) return s;
         auto copy_size = std::min( buffer_size, s );
         memcpy( memory, context.get_action().data.data(), copy_size );
         return copy_size;
         */
         return 24;
      }

      int action_data_size() {
         return 24;
         //return context.get_action().data.size();
      }

      name current_receiver() {
         return name{123};
         //return context.get_receiver();
      }
};

class call_depth_api : public context_aware_api {
   public:
      call_depth_api( apply_context& ctx )
      :context_aware_api(ctx,true){}
      void call_depth_assert() {
         //FC_THROW_EXCEPTION(wasm_execution_error, "Exceeded call depth maximum");
      }
};

REGISTER_INJECTED_INTRINSICS(call_depth_api,
   (call_depth_assert,  void()               )
);

REGISTER_INTRINSICS(compiler_builtins,
   (__ashlti3,     void(int, int64_t, int64_t, int)               )
   (__ashrti3,     void(int, int64_t, int64_t, int)               )
   (__lshlti3,     void(int, int64_t, int64_t, int)               )
   (__lshrti3,     void(int, int64_t, int64_t, int)               )
   (__divti3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__udivti3,     void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__modti3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__umodti3,     void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__multi3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__addtf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__subtf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__multf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__divtf3,      void(int, int64_t, int64_t, int64_t, int64_t)  )
   (__eqtf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__netf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__getf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__gttf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__lttf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__letf2,       int(int64_t, int64_t, int64_t, int64_t)        )
   (__cmptf2,      int(int64_t, int64_t, int64_t, int64_t)        )
   (__unordtf2,    int(int64_t, int64_t, int64_t, int64_t)        )
   (__negtf2,      void (int, int64_t, int64_t)                   )
   (__floatsitf,   void (int, int)                                )
   (__floatunsitf, void (int, int)                                )
   (__floatditf,   void (int, int64_t)                            )
   (__floatunditf, void (int, int64_t)                            )
   (__floattidf,   double (int64_t, int64_t)                      )
   (__floatuntidf, double (int64_t, int64_t)                      )
   (__floatsidf,   double(int)                                    )
   (__extendsftf2, void(int, float)                               )
   (__extenddftf2, void(int, double)                              )
   (__fixtfti,     void(int, int64_t, int64_t)                    )
   (__fixtfdi,     int64_t(int64_t, int64_t)                      )
   (__fixtfsi,     int(int64_t, int64_t)                          )
   (__fixunstfti,  void(int, int64_t, int64_t)                    )
   (__fixunstfdi,  int64_t(int64_t, int64_t)                      )
   (__fixunstfsi,  int(int64_t, int64_t)                          )
   (__fixsfti,     void(int, float)                               )
   (__fixdfti,     void(int, double)                              )
   (__fixunssfti,  void(int, float)                               )
   (__fixunsdfti,  void(int, double)                              )
   (__trunctfdf2,  double(int64_t, int64_t)                       )
   (__trunctfsf2,  float(int64_t, int64_t)                        )
);

REGISTER_INTRINSICS(privileged_api,
   (is_feature_active,                int(int64_t)                          )
   (activate_feature,                 void(int64_t)                         )
   (get_resource_limits,              void(int64_t,int,int,int)             )
   (set_resource_limits,              void(int64_t,int64_t,int64_t,int64_t) )
   (set_proposed_producers,           int64_t(int,int)                      )
   (get_blockchain_parameters_packed, int(int, int)                         )
   (set_blockchain_parameters_packed, void(int,int)                         )
   (is_privileged,                    int(int64_t)                          )
   (set_privileged,                   void(int64_t, int)                    )
   (preactivate_feature,              void(int)                             )
);


REGISTER_INTRINSICS( database_api,
   (db_store_i64,        int(int64_t,int64_t,int64_t,int64_t,int,int))
   (db_update_i64,       void(int,int64_t,int,int))
   (db_remove_i64,       void(int))
   (db_get_i64,          int(int, int, int))
   (db_next_i64,         int(int, int))
   (db_previous_i64,     int(int, int))
   (db_find_i64,         int(int64_t,int64_t,int64_t,int64_t))
   (db_lowerbound_i64,   int(int64_t,int64_t,int64_t,int64_t))
   (db_upperbound_i64,   int(int64_t,int64_t,int64_t,int64_t))
   (db_end_i64,          int(int64_t,int64_t,int64_t))
);


REGISTER_INTRINSICS(permission_api,
   (check_transaction_authorization, int(int, int, int, int, int, int)                  )
   (check_permission_authorization,  int(int64_t, int64_t, int, int, int, int, int64_t) )
   (get_permission_last_used,        int64_t(int64_t, int64_t) )
   (get_account_creation_time,       int64_t(int64_t) )
);


REGISTER_INTRINSICS(system_api,
   (current_time,          int64_t() )
   (publication_time,      int64_t() )
   (is_feature_activated,  int(int)  )
   (get_sender,            int64_t() )
);

REGISTER_INTRINSICS(context_free_system_api,
   (abort,                void()              )
   (eosio_assert,         void(int, int)      )
   (eosio_assert_message, void(int, int, int) )
   (eosio_assert_code,    void(int, int64_t)  )
   (eosio_exit,           void(int)           )
);

REGISTER_INTRINSICS(action_api,
   (read_action_data,       int(int, int)  )
   (action_data_size,       int()          )
   (current_receiver,   int64_t()          )
);

REGISTER_INTRINSICS(authorization_api,
   (require_recipient,     void(int64_t)          )
   (require_authorization, void(int64_t), "require_auth", void(authorization_api::*)(const name&) )
   (require_authorization, void(int64_t, int64_t), "require_auth2", void(authorization_api::*)(const name&, const name& permission) )
   (has_authorization,     int(int64_t), "has_auth", bool(authorization_api::*)(const name&)const )
   (is_account,            int(int64_t)           )
);


REGISTER_INTRINSICS(console_api,
   (prints,                void(int)      )
   (prints_l,              void(int, int) )
   (printi,                void(int64_t)  )
   (printui,               void(int64_t)  )
   (printi128,             void(int)      )
   (printui128,            void(int)      )
   (printsf,               void(float)    )
   (printdf,               void(double)   )
   (printqf,               void(int)      )
   (printn,                void(int64_t)  )
   (printhex,              void(int, int) )
);


REGISTER_INTRINSICS(context_free_transaction_api,
   (read_transaction,       int(int, int)            )
   (transaction_size,       int()                    )
   (expiration,             int()                    )
   (tapos_block_prefix,     int()                    )
   (tapos_block_num,        int()                    )
   (get_action,             int (int, int, int, int) )
);

REGISTER_INTRINSICS(transaction_api,
   (send_inline,               void(int, int)               )
   (send_context_free_inline,  void(int, int)               )
   (send_deferred,             void(int, int64_t, int, int, int32_t) )
   (cancel_deferred,           int(int)                     )
);

 
REGISTER_INTRINSICS(context_free_api,
   (get_context_free_data, int(int, int, int) )
)


REGISTER_INTRINSICS(memory_api,
   (memcpy,                 int(int, int, int)  )
   (memmove,                int(int, int, int)  )
   (memcmp,                 int(int, int, int)  )
   (memset,                 int(int, int, int)  )
);


}}

void shang()
{
   printf("bbc test OK \n");
}

#include <fstream>
#include <string.h>
#include <sys/stat.h>

#include <IR/Module.h>
#include "IR/Operators.h"
#include "Inline/Serialization.h"
#include <softfloat.hpp>
#include <compiler_builtins.hpp>
#include "WASM/WASM.h"
#include <src/binary-reader.h>



#include "IR/Module.h"
#include "Runtime/Intrinsics.h"
#include "Platform/Platform.h"
#include "WAST/WAST.h"
#include "IR/Validate.h"

#include <src/interp.h>
#include <src/binary-reader-interp.h>
#include <src/error-formatter.h>
#include <src/binary-reader.h>

static eosio::chain::apply_context *c;
static eosio::chain::webassembly::wabt_runtime::wabt_apply_instance_vars* static_wabt_vars;


using namespace wabt;
using namespace wabt::interp;

void shang_test2(const char* code_bytes, size_t code_size, std::vector<uint8_t> initial_memory)
{
   using namespace eosio::chain::webassembly::wabt_runtime;
   using namespace eosio::chain;
   std::unique_ptr<wabt::interp::Environment> env = std::make_unique<wabt::interp::Environment>();
   for(auto it = intrinsic_registrator::get_map().begin() ; it != intrinsic_registrator::get_map().end(); ++it) 
   {
      interp::HostModule* host_module = env->AppendHostModule(it->first);
      for(auto itf = it->second.begin(); itf != it->second.end(); ++itf) 
      {
         host_module->AppendFuncExport(itf->first, itf->second.sig, [fn=itf->second.func](const auto* f, const auto* fs, const auto& args, auto& res) {
            interp::TypedValue ret = fn(*static_wabt_vars, args);
            if(ret.type != Type::Void)
               res[0] = ret;
            return interp::Result::Ok;});
      }
   }
   
   interp::DefinedModule* instantiated_module = nullptr;
   wabt::Errors errors;
   wabt::ReadBinaryOptions read_binary_options;

   ReadBinaryInterp(env.get(), code_bytes, code_size, read_binary_options, &errors, &instantiated_module);
      
   std::vector<uint8_t> _initial_memory = initial_memory;
   DefinedModule*   _instatiated_module = instantiated_module;
   Executor _executor(env.get(), nullptr, Thread::Options(64*1024,  wasm_constraints::maximum_call_depth+2));
   std::vector<std::pair<Global*, TypedValue>>       _initial_globals;
   Limits                                            _initial_memory_configuration;
   
   for (Index i = 0; i < env->GetGlobalCount(); ++i) 
   {
      if(env->GetGlobal(i)->mutable_ == false)
         continue;
      _initial_globals.emplace_back(env->GetGlobal(i), env->GetGlobal(i)->typed_value);
   }
   
   if (env->GetMemoryCount())
   {
      _initial_memory_configuration = env->GetMemory(0)->page_limits;
   }

   // 开始调用
   for(const auto& mg : _initial_globals)
   {
      mg.first->typed_value = mg.second;
   }
   wabt_apply_instance_vars this_run_vars{nullptr, *c};
   static_wabt_vars = &this_run_vars;

   //reset memory to inital size & copy back in initial data
   if (env->GetMemoryCount()) 
   {
      Memory* memory = this_run_vars.memory = env->GetMemory(0);
      memory->page_limits = _initial_memory_configuration;
      memory->data.resize(_initial_memory_configuration.initial * WABT_PAGE_SIZE);
      memset(memory->data.data(), 0, memory->data.size());
      memcpy(memory->data.data(), _initial_memory.data(), _initial_memory.size());
   }
   TypedValues   _params{3, TypedValue(Type::I64)};

   _params[0].set_i64(uint64_t(6138663591592764928));
   _params[1].set_i64(uint64_t(6138663591592764928));
   _params[2].set_i64(uint64_t(5031766152489992192));

   auto res1 = _executor.RunStartFunction(_instatiated_module);

   res1 = _executor.RunExportByName(_instatiated_module, "apply", _params);
   if (res1.result == interp::Result::Ok)
   {
      printf("OK\n");
   }
   else
   {
      printf("Err\n");
   }
}

std::vector<uint8_t> parse_initial_memory(const IR::Module& module) {
         std::vector<uint8_t> mem_image;

         for(const IR::DataSegment& data_segment : module.dataSegments) {
            //EOS_ASSERT(data_segment.baseOffset.type == InitializerExpression::Type::i32_const, wasm_exception, "");
            //EOS_ASSERT(module.memories.defs.size(), wasm_exception, "");
            const U32 base_offset = data_segment.baseOffset.i32;
            const Uptr memory_size = (module.memories.defs[0].type.size.min << IR::numBytesPerPageLog2);
            if(base_offset >= memory_size || base_offset + data_segment.data.size() > memory_size)
            {
               //FC_THROW_EXCEPTION(wasm_execution_error, "WASM data segment outside of valid memory range");
            }
            if(base_offset + data_segment.data.size() > mem_image.size())
               mem_image.resize(base_offset + data_segment.data.size(), 0x00);
            memcpy(mem_image.data() + base_offset, data_segment.data.data(), data_segment.data.size());
         }
         return mem_image;
      }

void shang_test1()
{
   char *p = new char[1088];
   c = (eosio::chain::apply_context *)p;
   using namespace wabt;
   using namespace wabt::interp;
   using namespace IR;
   using namespace Runtime;

   IR::Module module;
   const char *f = "/home/shang/shangqd/eosio.contracts/build/contracts/eosio.token/eosio.token.wasm";
   struct stat statbuf;
   stat(f,&statbuf);
   std::ifstream inFile(f,std::ios::in|std::ios::binary);
   int n = statbuf.st_size;
   uint8_t *byte = new uint8_t[n];
   inFile.read((char*)byte, n);
   inFile.close();

   Serialization::MemoryInputStream stream((const U8*)byte, n);
   WASM::serialize(stream, module);
   module.userSections.clear();
    
   std::vector<U8> bytes;
   Serialization::ArrayOutputStream outstream;
   WASM::serialize(outstream, module);
   bytes = outstream.getBytes();
   shang_test2((const char*)bytes.data(), bytes.size(), parse_initial_memory(module));
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