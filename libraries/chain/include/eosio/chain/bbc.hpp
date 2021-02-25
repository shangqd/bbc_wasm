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
  class sha256 
{
  public:
    sha256()
    {

    }
    explicit sha256( const std::string& hex_str )
    {

    }
    explicit sha256( const char *data, size_t size )
    {

    }

    std::string str()const
    {
       return std::string("abc");
    }
    
    char* data()const
    {
       static char s[3];
       return s;
    }

    size_t data_size()const { return 256 / 8; }

    static sha256 hash( const char* d, uint32_t dlen );
    static sha256 hash( const std::string& );
    static sha256 hash( const sha256& );

    template<typename T>
    static sha256 hash( const T& t ) 
    { 
      sha256::encoder e; 
      //fc::raw::pack(e,t);
      return e.result(); 
    } 

    class encoder 
    {
      public:
        encoder();
        ~encoder();

        void write( const char* d, uint32_t dlen );
        void put( char c ) { write( &c, 1 ); }
        void reset();
        sha256 result();

      private:
        struct      impl;
        //fc::fwd<impl,112> my;
    };

    template<typename T>
    inline friend T& operator<<( T& ds, const sha256& ep ) {
      ds.write( ep.data(), sizeof(ep) );
      return ds;
    }

    template<typename T>
    inline friend T& operator>>( T& ds, sha256& ep ) {
      ds.read( ep.data(), sizeof(ep) );
      return ds;
    }
    friend sha256 operator << ( const sha256& h1, uint32_t i       );
    friend sha256 operator >> ( const sha256& h1, uint32_t i       );
    friend bool   operator == ( const sha256& h1, const sha256& h2 );
    friend bool   operator != ( const sha256& h1, const sha256& h2 );
    friend sha256 operator ^  ( const sha256& h1, const sha256& h2 );
    friend bool   operator >= ( const sha256& h1, const sha256& h2 );
    friend bool   operator >  ( const sha256& h1, const sha256& h2 ); 
    friend bool   operator <  ( const sha256& h1, const sha256& h2 ); 

    uint32_t pop_count()const
    {
       return (uint32_t)(__builtin_popcountll(_hash[0]) +
                         __builtin_popcountll(_hash[1]) +
                         __builtin_popcountll(_hash[2]) +
                         __builtin_popcountll(_hash[3])); 
    }

    /**
     * Count leading zero bits
     */
    uint16_t clz()const;

    /**
     * Approximate (log_2(x) + 1) * 2**24.
     *
     * Detailed specs:
     * - Return 0 when x == 0.
     * - High 8 bits of result simply counts nonzero bits.
     * - Low 24 bits of result are the 24 bits of input immediately after the most significant 1 in the input.
     * - If above would require reading beyond the end of the input, zeros are used instead.
     */
    uint32_t approx_log_32()const;

    void set_to_inverse_approx_log_32( uint32_t x );
    static double inverse_approx_log_32_double( uint32_t x );

    uint64_t _hash[4];
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
