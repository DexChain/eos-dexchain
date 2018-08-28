/**
 *  @file
 *  @copyright defined in ../LICENSE
 */

#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

namespace dex {
   using namespace eosio;
   using eosio::asset;
   using eosio::symbol_type;
   typedef double real_type;


   /**
    *  Uses Bancor math to create a 50/50 relay between two asset types. The state of the
    *  bancor exchange is entirely contained within this struct. There are no external
    *  side effects associated with using this API.
    */
   struct dex_state {
      asset    supply;

      struct connector {
         asset balance;
         double weight = .5;

         EOSLIB_SERIALIZE( connector, (balance)(weight) )
      };

      connector base;
      connector quote;
      account_name dex_account;
      uint32_t fee_rate;

      uint64_t primary_key()const { return dex_account; }
      uint64_t by_base()const {return base.balance.symbol;}
      uint64_t by_quote()const {return quote.balance.symbol;}

      asset convert_to_dex( connector& c, asset in );
      asset convert_from_dex( connector& c, asset in );
      asset convert( asset from, symbol_type to );

      EOSLIB_SERIALIZE( dex_state, (supply)(base)(quote)(dex_account)(fee_rate) )
    };

    struct fund_state {
      asset base_balance;
      asset fund_balance;

      asset used_balance;

      account_name fund_account;
      uint8_t permission;

      uint64_t primary_key()const { return fund_account; }

      asset buy(asset quantity);
      asset sell(asset quantity);
      void  addfund(asset quantity);

      EOSLIB_SERIALIZE( fund_state, (base_balance)(fund_balance)(used_balance)(fund_account)(permission) )
    };

   typedef eosio::multi_index<N(dexmarkets), dex_state,
           indexed_by< N(base), const_mem_fun<dex_state, uint64_t,  &dex_state::by_base> >,
           indexed_by< N(quote), const_mem_fun<dex_state, uint64_t,  &dex_state::by_quote> >
   > dexmarkets;

   typedef eosio::multi_index<N(fundmarkets), fund_state
   > fundmarkets;

} /// namespace dex
