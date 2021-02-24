#pragma once
#include <stdint.h>
#include <string>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/subseq.hpp>
#include <boost/preprocessor/seq/remove.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/stringize.hpp>

namespace eosio { namespace chain {
    using namespace std;
    class controller
    {
        public:
            bool contracts_console() const
            {
                return true;
            }
    };
    class apply_context
    {
        public:
            controller&                   control;
    };
    class transaction_context 
    {
    };

    struct name 
    {
      uint64_t value = 0;
      bool empty()const { return 0 == value; }
      bool good()const  { return !empty();   }

      name( const char* str )   { set(str);           } 
      name( const string& str ) { set( str.c_str() ); }

      void set( const char* str );

      template<typename T>
      name( T v ):value(v){}
      name(){}

      explicit operator string()const;

      string to_string() const { return string(*this); }

      name& operator=( uint64_t v ) {
         value = v;
         return *this;
      }

      name& operator=( const string& n ) {
         value = name(n).value;
         return *this;
      }
      name& operator=( const char* n ) {
         value = name(n).value;
         return *this;
      }

      friend std::ostream& operator << ( std::ostream& out, const name& n ) {
         return out << string(n);
      }

      friend bool operator < ( const name& a, const name& b ) { return a.value < b.value; }
      friend bool operator <= ( const name& a, const name& b ) { return a.value <= b.value; }
      friend bool operator > ( const name& a, const name& b ) { return a.value > b.value; }
      friend bool operator >=( const name& a, const name& b ) { return a.value >= b.value; }
      friend bool operator == ( const name& a, const name& b ) { return a.value == b.value; }

      friend bool operator == ( const name& a, uint64_t b ) { return a.value == b; }
      friend bool operator != ( const name& a, uint64_t b ) { return a.value != b; }

      friend bool operator != ( const name& a, const name& b ) { return a.value != b.value; }

      operator bool()const            { return value; }
      operator uint64_t()const        { return value; }
      operator unsigned __int128()const       { return value; }
   };

#define _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)\
    _REGISTER_WAVM_INTRINSIC(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)\
    _REGISTER_WABT_INTRINSIC(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)

#define _REGISTER_INTRINSIC4(CLS, MOD, METHOD, WASM_SIG, NAME, SIG)\
   _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, NAME, SIG )

#define _REGISTER_INTRINSIC3(CLS, MOD, METHOD, WASM_SIG, NAME)\
   _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, NAME, decltype(&CLS::METHOD) )

#define _REGISTER_INTRINSIC2(CLS, MOD, METHOD, WASM_SIG)\
   _REGISTER_INTRINSIC_EXPLICIT(CLS, MOD, METHOD, WASM_SIG, BOOST_PP_STRINGIZE(METHOD), decltype(&CLS::METHOD) )

#define _REGISTER_INTRINSIC1(CLS, MOD, METHOD)\
   static_assert(false, "Cannot register " BOOST_PP_STRINGIZE(CLS) ":" BOOST_PP_STRINGIZE(METHOD) " without a signature");

#define _REGISTER_INTRINSIC0(CLS, MOD, METHOD)\
   static_assert(false, "Cannot register " BOOST_PP_STRINGIZE(CLS) ":<unknown> without a method name and signature");

#define _UNWRAP_SEQ(...) __VA_ARGS__

#define _EXPAND_ARGS(CLS, MOD, INFO)\
   ( CLS, MOD, _UNWRAP_SEQ INFO )

#define _REGISTER_INTRINSIC(R, CLS, INFO)\
   BOOST_PP_CAT(BOOST_PP_OVERLOAD(_REGISTER_INTRINSIC, _UNWRAP_SEQ INFO) _EXPAND_ARGS(CLS, "env", INFO), BOOST_PP_EMPTY())

#define REGISTER_INTRINSICS(CLS, MEMBERS)\
   BOOST_PP_SEQ_FOR_EACH(_REGISTER_INTRINSIC, CLS, _WRAPPED_SEQ(MEMBERS))

#define _REGISTER_INJECTED_INTRINSIC(R, CLS, INFO)\
   BOOST_PP_CAT(BOOST_PP_OVERLOAD(_REGISTER_INTRINSIC, _UNWRAP_SEQ INFO) _EXPAND_ARGS(CLS, EOSIO_INJECTED_MODULE_NAME, INFO), BOOST_PP_EMPTY())

#define REGISTER_INJECTED_INTRINSICS(CLS, MEMBERS)\
   BOOST_PP_SEQ_FOR_EACH(_REGISTER_INJECTED_INTRINSIC, CLS, _WRAPPED_SEQ(MEMBERS))


}};


namespace fc {
  class microseconds {
    public:
        constexpr explicit microseconds( int64_t c = 0) :_count(c){}
        static constexpr microseconds maximum() { return microseconds(0x7fffffffffffffffll); }
        friend constexpr microseconds operator + (const  microseconds& l, const microseconds& r ) { return microseconds(l._count+r._count); }
        friend constexpr microseconds operator - (const  microseconds& l, const microseconds& r ) { return microseconds(l._count-r._count); }

        constexpr bool operator==(const microseconds& c)const { return _count == c._count; }
        constexpr bool operator!=(const microseconds& c)const { return _count != c._count; }
        friend constexpr bool operator>(const microseconds& a, const microseconds& b){ return a._count > b._count; }
        friend constexpr bool operator>=(const microseconds& a, const microseconds& b){ return a._count >= b._count; }
        constexpr friend bool operator<(const microseconds& a, const microseconds& b){ return a._count < b._count; }
        constexpr friend bool operator<=(const microseconds& a, const microseconds& b){ return a._count <= b._count; }
        constexpr microseconds& operator+=(const microseconds& c) { _count += c._count; return *this; }
        constexpr microseconds& operator-=(const microseconds& c) { _count -= c._count; return *this; }
        constexpr int64_t count()const { return _count; }
        constexpr int64_t to_seconds()const { return _count/1000000; }
    private:
        friend class time_point;
        int64_t      _count;
  };
  inline constexpr microseconds seconds( int64_t s ) { return microseconds( s * 1000000 ); }
  inline constexpr microseconds milliseconds( int64_t s ) { return microseconds( s * 1000 ); }
  inline constexpr microseconds minutes(int64_t m) { return seconds(60*m); }
  inline constexpr microseconds hours(int64_t h) { return minutes(60*h); }
  inline constexpr microseconds days(int64_t d) { return hours(24*d); }

  //class variant;
  //void to_variant( const fc::microseconds&,  fc::variant&  );
  //void from_variant( const fc::variant& , fc::microseconds& );

  class time_point {
    public:
        constexpr explicit time_point( microseconds e = microseconds() ) :elapsed(e){}
        static time_point now();
        static constexpr time_point maximum() { return time_point( microseconds::maximum() ); }
        static constexpr time_point min() { return time_point();                      }

        operator std::string()const;
        static time_point from_iso_string( const std::string& s );

        constexpr const microseconds& time_since_epoch()const { return elapsed; }
        constexpr uint32_t            sec_since_epoch()const  { return elapsed.count() / 1000000; }
        constexpr bool   operator > ( const time_point& t )const                              { return elapsed._count > t.elapsed._count; }
        constexpr bool   operator >=( const time_point& t )const                              { return elapsed._count >=t.elapsed._count; }
        constexpr bool   operator < ( const time_point& t )const                              { return elapsed._count < t.elapsed._count; }
        constexpr bool   operator <=( const time_point& t )const                              { return elapsed._count <=t.elapsed._count; }
        constexpr bool   operator ==( const time_point& t )const                              { return elapsed._count ==t.elapsed._count; }
        constexpr bool   operator !=( const time_point& t )const                              { return elapsed._count !=t.elapsed._count; }
        constexpr time_point&  operator += ( const microseconds& m)                           { elapsed+=m; return *this;                 }
        constexpr time_point&  operator -= ( const microseconds& m)                           { elapsed-=m; return *this;                 }
        constexpr time_point   operator + (const microseconds& m) const { return time_point(elapsed+m); }
        constexpr time_point   operator - (const microseconds& m) const { return time_point(elapsed-m); }
       constexpr microseconds operator - (const time_point& m) const { return microseconds(elapsed.count() - m.elapsed.count()); }
    private:
        microseconds elapsed;
  };

  /**
   *  A lower resolution time_point accurate only to seconds from 1970
   */
  class time_point_sec
  {
    public:
        constexpr time_point_sec()
        :utc_seconds(0){}

        constexpr explicit time_point_sec(uint32_t seconds )
        :utc_seconds(seconds){}

        constexpr time_point_sec( const time_point& t )
        :utc_seconds( t.time_since_epoch().count() / 1000000ll ){}

        static constexpr time_point_sec maximum() { return time_point_sec(0xffffffff); }
        static constexpr time_point_sec min() { return time_point_sec(0); }

        constexpr operator time_point()const { return time_point( fc::seconds( utc_seconds) ); }
        constexpr uint32_t sec_since_epoch()const { return utc_seconds; }

        constexpr time_point_sec operator = ( const fc::time_point& t )
        {
          utc_seconds = t.time_since_epoch().count() / 1000000ll;
          return *this;
        }
        constexpr friend bool      operator < ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds < b.utc_seconds; }
        constexpr friend bool      operator > ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds > b.utc_seconds; }
        constexpr friend bool      operator <= ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds <= b.utc_seconds; }
        constexpr friend bool      operator >= ( const time_point_sec& a, const time_point_sec& b )  { return a.utc_seconds >= b.utc_seconds; }
        constexpr friend bool      operator == ( const time_point_sec& a, const time_point_sec& b ) { return a.utc_seconds == b.utc_seconds; }
        constexpr friend bool      operator != ( const time_point_sec& a, const time_point_sec& b ) { return a.utc_seconds != b.utc_seconds; }
        constexpr time_point_sec&  operator += ( uint32_t m ) { utc_seconds+=m; return *this; }
        constexpr time_point_sec&  operator += ( microseconds m ) { utc_seconds+=m.to_seconds(); return *this; }
        constexpr time_point_sec&  operator -= ( uint32_t m ) { utc_seconds-=m; return *this; }
        constexpr time_point_sec&  operator -= ( microseconds m ) { utc_seconds-=m.to_seconds(); return *this; }
        constexpr time_point_sec   operator +( uint32_t offset )const { return time_point_sec(utc_seconds + offset); }
        constexpr time_point_sec   operator -( uint32_t offset )const { return time_point_sec(utc_seconds - offset); }

        friend constexpr time_point   operator + ( const time_point_sec& t, const microseconds& m )   { return time_point(t) + m;             }
        friend constexpr time_point   operator - ( const time_point_sec& t, const microseconds& m )   { return time_point(t) - m;             }
        friend constexpr microseconds operator - ( const time_point_sec& t, const time_point_sec& m ) { return time_point(t) - time_point(m); }
        friend constexpr microseconds operator - ( const time_point& t, const time_point_sec& m ) { return time_point(t) - time_point(m); }

        //std::string to_non_delimited_iso_string()const;
        //std::string to_iso_string()const;

        //operator std::string()const;
        //static time_point_sec from_iso_string( const std::string& s );

    private:
        uint32_t utc_seconds;
  };
}



//#include <eosio/chain/webassembly/wavm.hpp>
//#include <eosio/chain/webassembly/wabt.hpp>
//#include <eosio/chain/wasm_interface.hpp>
//#include <eosio/chain/wasm_eosio_injection.hpp>
//#include <eosio/chain/webassembly/runtime_interface.hpp>
//#include <eosio/chain/wasm_eosio_constraints.hpp>
//#include <eosio/chain/types.hpp>
//#include "Runtime/Runtime.h"
//#include "IR/Types.h"
//#include "IR/Module.h"
//#include "Platform/Platform.h"
//#include "WAST/WAST.h"
//#include "IR/Operators.h"
//#include "IR/Validate.h"
//#include "Runtime/Linker.h"
//#include "Runtime/Intrinsics.h"
//#include <softfloat.hpp>

//eosio::chain::apply_context
