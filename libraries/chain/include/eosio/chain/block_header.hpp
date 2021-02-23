#pragma once
#include <eosio/chain/block_timestamp.hpp>
#include <eosio/chain/producer_schedule.hpp>
//#include <eosio/chain/protocol_feature_activation.hpp>

#include <type_traits>

namespace eosio { namespace chain {
   struct protocol_feature_activation
   {

   };
   namespace detail {
   
      template<typename... Ts>
      struct block_header_extension_types {
         using block_header_extensions_t = fc::static_variant< Ts... >;
         using decompose_t = decompose< Ts... >;
      };
   }

   using block_header_extension_types = detail::block_header_extension_types<
      protocol_feature_activation
   >;

   using block_header_extensions = block_header_extension_types::block_header_extensions_t;

   struct block_header
   {
      block_timestamp_type             timestamp;
      account_name                     producer;

      /**
       *  By signing this block this producer is confirming blocks [block_num() - confirmed, blocknum())
       *  as being the best blocks for that range and that he has not signed any other
       *  statements that would contradict.
       *
       *  No producer should sign a block with overlapping ranges or it is proof of byzantine
       *  behavior. When producing a block a producer is always confirming at least the block he
       *  is building off of.  A producer cannot confirm "this" block, only prior blocks.
       */
      uint16_t                         confirmed = 1;

      block_id_type                    previous;

      checksum256_type                 transaction_mroot; /// mroot of cycles_summary
      checksum256_type                 action_mroot; /// mroot of all delivered action receipts


      /** The producer schedule version that should validate this block, this is used to
       * indicate that the prior block which included new_producers->version has been marked
       * irreversible and that it the new producer schedule takes effect this block.
       */
      uint32_t                          schedule_version = 0;
      optional<producer_schedule_type>  new_producers;
      extensions_type                   header_extensions;


      block_header() = default;

      digest_type       digest()const;
      block_id_type     id() const;
      uint32_t          block_num() const { return num_from_id(previous) + 1; }
      static uint32_t   num_from_id(const block_id_type& id);

   };


   struct signed_block_header : public block_header
   {
      signature_type    producer_signature;
   };

} } /// namespace eosio::chain

FC_REFLECT(eosio::chain::block_header,
           (timestamp)(producer)(confirmed)(previous)
           (transaction_mroot)(action_mroot)
           (schedule_version)(new_producers)(header_extensions))

FC_REFLECT_DERIVED(eosio::chain::signed_block_header, (eosio::chain::block_header), (producer_signature))
