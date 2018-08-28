/**
 *  @file
 *  @copyright defined in ../LICENSE
 */

#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>
#include "exchange_state.hpp"

namespace dex {
    using namespace eosio;
    using std::string;
    using eosio::asset;
    using eosio::symbol_type;

   class dexchain : public contract {
      public:
         dexchain( account_name self );

         void create( account_name issuer,
                      asset        maximum_supply);

         void issue( account_name to, asset quantity, string memo );

         void transfer( account_name from,
                        account_name to,
                        asset        quantity,
                        string       memo );

         void approve( account_name account,
                      asset        quantity,
                      string       memo );

         void disapprove( account_name account,
                      asset        quantity,
                      string       memo );

         void burn( account_name owner, asset quantity, string memo );

         void createbystake(
                    asset           maximum_supply,
                    asset           stake,
                    account_name    stake_contract);

         void stake(
            account_name player,
            account_name receiver,
            symbol_type token_symbol,
            asset stake,
            account_name stakecontract,
            string memo );

         void unstake(
            account_name player,
            account_name receiver,
            asset quantity,
            symbol_type stake_symbol,
            account_name stakecontract,
            string memo );

         inline asset get_supply( symbol_name sym )const;

         inline asset get_balance( account_name owner, symbol_name sym )const;

         inline asset get_balance2( account_name owner, symbol_name sym )const;


      // dex functions
         void createdex(account_name creator, account_name dex_account, asset base, asset quote, uint32_t fee_amount);
         void buytoken(account_name owner, account_name receiver, account_name dex_account, asset quat);
         void selltoken(account_name owner, account_name receiver, account_name dex_account, asset quant);
         void closedex(account_name dex_account);

        //TODO
         void claim(account_name account);
         void refund(account_name dex_account);
         void resetDex(account_name dex_account, asset maxBase, asset maxQuote);

     // fund functions
         void createfund(account_name creator, account_name fund_account, symbol_name base, symbol_name fund, uint8_t permission);
         void buyfund(account_name payer, account_name receiver, account_name fund_account, asset quantity);
         void sellfund(account_name account, account_name fund_account, asset quantity);
         void addfund(account_name account, account_name fund_account, asset quantity);
         void rentcpu(account_name account, account_name fund_account, asset quantity);
         //使用基金的资金(XEOS/EOS)为账号抵押资源，该类型基金为保本型基金
         void delegatebw(account_name fund_account, account_name receiver, asset net, asset cpu);

        //TODO:
         //使用基金的资金(XEOS/EOS)投票，该类型基金为保本型基金
         void voteproducer( account_name fund_account, const account_name voter, const account_name proxy, const std::vector<account_name>& producers );
         //提取基金的资金(XEOS/EOS)投资，该类型基金为高风险型基金
         void withdrawfund( account_name fund_account, const account_name voter, const account_name proxy, const std::vector<account_name>& producers );

      private:
         struct account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.name(); }
         };

         struct currency_stats {
            asset          supply;
            asset          max_supply;
            account_name   issuer;

            //Optional
            symbol_type  stake_symbol;
            account_name stake_contract;

            uint64_t primary_key()const { return supply.symbol.name(); }
        };

        struct fund_usage {
            uint64_t       id;
            account_name   fund_account;

            asset          amount;
            asset          amount2;
            uint8_t        type;
            time           create_time;
            uint64_t primary_key()const { return id; }
        };

         typedef eosio::multi_index<N(accounts), account> accounts;
         typedef eosio::multi_index<N(approves), account> approves;
         typedef eosio::multi_index<N(fundusages), fund_usage> fundusages;
         typedef eosio::multi_index<N(stat), currency_stats> stats;
         dexmarkets _dexmarket;
         fundmarkets _fundmarket;


         void sub_balance( account_name owner, asset value );
         void add_balance( account_name owner, asset value, account_name ram_payer );

         void sub_balance2( account_name owner, asset value );
         void add_balance2( account_name owner, asset value, account_name ram_payer );

         void assertAsset( asset quantity );
         void updateusage( account_name fund_account );

      public:
         struct transfer_args {
            account_name  from;
            account_name  to;
            asset         quantity;
            string        memo;
         };
   };

   asset dexchain::get_supply( symbol_name sym )const
   {
      stats statstable( _self, sym );
      const auto& st = statstable.get( sym );
      return st.supply;
   }

   asset dexchain::get_balance( account_name owner, symbol_name sym )const
   {
      accounts accountstable( _self, owner );
      const auto& ac = accountstable.get( sym );
      return ac.balance;
   }

   asset dexchain::get_balance2( account_name owner, symbol_name sym )const
   {
      approves accountstable( _self, owner );
      const auto& ac = accountstable.get( sym );
      return ac.balance;
   }

} /// namespace dex
