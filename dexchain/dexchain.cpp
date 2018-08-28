/**
 *  @file
 *  @copyright defined in ../LICENSE
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include "dexchain.hpp"
#include <string>

namespace dex {
#define NONE_ACCOUNT    0
#define APPROVE_ACCOUNT _self
#define STAKE_ACCOUNT   _self
#define DELEGATE_ACCOUNT _self

#define SYSTEM_CONTRACT N(eosio)
#define EOS_CONTRACT N(eosio.token)
#define EOS_SYM S(4,EOS)
#define DEXCHAIN_EOS_SYM S(4,XEOS)
#define NONE_SYMBOL S(0,NONE)

#define FUND_PERMISSION_DELEGATE 0x1
#define FUND_PERMISSION_VOTE     0x2
#define FUND_PERMISSION_WITHDRAW 0x4
#define FUND_PERMISSION_FULL     0xff

#define FUND_USAGE_TYPE_DELEGATEBW 1
#define FUND_USAGE_TYPE_VOTE       2
#define FUND_USAGE_TYPE_WITHDRAW   3

using namespace eosio;

dexchain::dexchain( account_name self ):contract(self), _dexmarket(self, self), _fundmarket(self, self) {
    auto xeos = eosio::symbol_type(DEXCHAIN_EOS_SYM);
    stats statstable( _self, xeos.name() );
    auto existing = statstable.find( xeos.name());
    if ( existing == statstable.end()) {
        print("Create EOS with stake asset");
        auto maximum_supply = asset(100000000000000ll, xeos);
        statstable.emplace( _self, [&]( auto& s ) {
            s.supply.symbol  = maximum_supply.symbol;
            s.max_supply     = maximum_supply;
            s.issuer         = self;
            s.stake_symbol   = EOS_SYM;
            s.stake_contract = EOS_CONTRACT;
        });
    } else {
        print("EOS with stake already created");
    }
}

void dexchain::create( account_name issuer,
                    asset        maximum_supply)
{
    require_auth( issuer );

    auto sym = maximum_supply.symbol;
    //TODO:Should inner token begin with "Y"?
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( maximum_supply.is_valid(), "invalid supply");
    eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.name() );
    auto existing = statstable.find( sym.name() );
    eosio_assert( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( issuer, [&]( auto& s ) {
       s.supply.symbol  = maximum_supply.symbol;
       s.max_supply     = maximum_supply;
       s.issuer         = issuer;
       s.stake_symbol   = NONE_SYMBOL;
       s.stake_contract = NONE_ACCOUNT;
    });
}

void dexchain::createbystake(
                    asset           maximum_supply,
                    asset           stake,
                    account_name    stake_contract)
{
    require_auth( stake_contract );

    auto sym = maximum_supply.symbol;
    //TODO:Should token with stake asset begin with "X"?
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( maximum_supply.is_valid(), "invalid supply");
    eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");
    eosio_assert( stake.amount >= 0, "stake amount must >= 0");

    stats statstable( _self, sym.name() );
    auto existing = statstable.find( sym.name() );
    eosio_assert( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol  = maximum_supply.symbol;
       s.max_supply     = maximum_supply;
       s.issuer         = STAKE_ACCOUNT;

       s.stake_symbol   = stake.symbol;
       s.stake_contract = stake_contract;
    });

    if (stake.amount > 0) {
        auto quantity = asset(stake.amount, sym);
        action(
            permission_level{ stake_contract, N(active) }, //Must assign eosio.code of creator to this contract
            _self, N(stake),
            std::make_tuple(stake_contract, stake_contract, quantity, stake, stake_contract, std::string("stake outer token to dex token"))
        ).send();
    }
}

void dexchain::issue( account_name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    auto sym_name = sym.name();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    require_auth( st.issuer );
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must issue positive quantity" );

    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, 0, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
        action(
                permission_level{ st.issuer, N(active) },
                _self, N(transfer),
                std::make_tuple(st.issuer, to, quantity, memo)
        ).send();
    }
}

void dexchain::approve( account_name account,
                      asset        quantity,
                      string       memo )
{
    require_auth( account );
    auto sym = quantity.symbol.name();
    stats statstable( _self, sym );
    const auto& st = statstable.get( sym );
    require_recipient( account );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    //disapprove may cost ram, so sub must after add
    add_balance2( account, quantity, account );
    sub_balance( account, quantity );
}

void dexchain::disapprove( account_name account,
                      asset        quantity,
                      string       memo )
{
    require_auth( APPROVE_ACCOUNT );
    auto sym = quantity.symbol.name();
    stats statstable( _self, sym );
    const auto& st = statstable.get( sym );
    require_recipient( account );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    sub_balance2( account, quantity );
    //Approve add
    add_balance( account, quantity, account );
}

void dexchain::transfer( account_name from,
                      account_name to,
                      asset        quantity,
                      string       memo )
{
    eosio_assert( from != to, "cannot transfer to self" );
    require_auth( from );
    eosio_assert( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.name();
    stats statstable( _self, sym );
    const auto& st = statstable.get( sym );
    require_recipient( from );
    require_recipient( to );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    sub_balance( from, quantity );
    add_balance( to, quantity, from );
}

void dexchain::burn( account_name owner, asset quantity, string memo )
{
    require_auth( owner );
    sub_balance( owner, quantity );
    auto sym_name = quantity.symbol.name();
    stats statstable( _self, sym_name );
    auto& st = statstable.get( sym_name );
    statstable.modify( st, 0, [&]( auto& s ) {
       s.supply -= quantity;
    });
}

void dexchain::sub_balance( account_name owner, asset value) {
   accounts from_acnts( _self, owner );

   const auto& from = from_acnts.get( value.symbol.name(), "no balance object found" );
   eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


   if( from.balance.amount == value.amount ) {
      approves acnts( _self, owner );
      const auto existing = acnts.find( value.symbol.name());
      if (existing == acnts.end()) {
        from_acnts.erase( from );
      } else {
        from_acnts.modify( from, owner, [&]( auto& a ) {
            a.balance -= value;
        });
      }
   } else {
      from_acnts.modify( from, owner, [&]( auto& a ) {
          a.balance -= value;
      });
   }
}

void dexchain::add_balance( account_name owner, asset value, account_name ram_payer )
{
   accounts to_acnts( _self, owner );
   auto to = to_acnts.find( value.symbol.name() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, 0, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

void dexchain::sub_balance2( account_name owner, asset value ) {
   approves from_acnts( _self, owner );

   const auto& from = from_acnts.get( value.symbol.name(), "no balance object found" );
   eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );


   if( from.balance.amount == value.amount ) {
      from_acnts.erase( from );
   } else {
      from_acnts.modify( from, owner, [&]( auto& a ) {
          a.balance -= value;
      });
   }
}

void dexchain::add_balance2( account_name owner, asset value, account_name ram_payer )
{
   approves to_acnts( _self, owner );
   auto to = to_acnts.find( value.symbol.name() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, 0, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

void dexchain::assertAsset( asset quantity ) {
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );

    auto sym_name = sym.name();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist" );
}

void dexchain::stake(
            account_name player,
            account_name receiver,
            symbol_type token_symbol,
            asset stake,
            account_name stakecontract,
            string memo )
{
    require_auth( player );

    auto sym = token_symbol;
    auto sym_name = sym.name();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before link" );
    const auto& st = *existing;
    eosio_assert( st.stake_contract == stakecontract, "invalid stake contract" );
    eosio_assert( st.stake_symbol == stake.symbol, "invalid stake symbol" );

    action(
        permission_level{ player, N(active) }, //Must assign eosio.code of creator to this contract
        stakecontract, N(transfer),
        std::make_tuple(player, STAKE_ACCOUNT, stake, std::string("Send stacked outer token to stakeaccount"))
    ).send();

    asset quantity = asset(stake.amount, token_symbol);
    action(
        permission_level{ STAKE_ACCOUNT, N(active) }, //Must assign eosio.code of creator to this contract
        _self, N(issue),
        std::make_tuple(STAKE_ACCOUNT, quantity, std::string("Issue inner token for staked outer stoken"))
    ).send();
    // add_balance(STAKE_ACCOUNT, quantity, player);

    action(
        permission_level{ STAKE_ACCOUNT, N(active) }, //Must assign eosio.code of creator to this contract
        _self, N(transfer),
        std::make_tuple(STAKE_ACCOUNT, receiver, quantity, std::string("Send inner token to receiver"))
    ).send();
}

void dexchain::unstake(
            account_name player,
            account_name receiver,
            asset quantity,
            symbol_type stake_symbol,
            account_name stakecontract,
            string memo )
{
    require_auth( player );

    auto sym = quantity.symbol;
    auto sym_name = sym.name();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before link" );
    const auto& st = *existing;
    eosio_assert( st.stake_contract == stakecontract, "invalid stake contract" );
    eosio_assert( st.stake_symbol == stake_symbol, "invalid stake symbol" );

    action(
        permission_level{ player, N(active) }, //Must assign eosio.code of creator to this contract
        _self, N(transfer),
        std::make_tuple(player, STAKE_ACCOUNT, quantity, std::string("Send dex token to stack account"))
    ).send();

    statstable.modify( st, 0, [&]( auto& s ) {
       s.supply.amount -= quantity.amount;
    });

    asset stake = asset(quantity.amount, stake_symbol);
    action(
        permission_level{ STAKE_ACCOUNT, N(active) }, //Must assign eosio.code of creator to this contract
        stakecontract, N(transfer),
        std::make_tuple(STAKE_ACCOUNT, receiver, stake, std::string("Send stacked totoken to receive"))
    ).send();

    action(
        permission_level{ STAKE_ACCOUNT, N(active) }, //Must assign eosio.code of creator to this contract
        stakecontract, N(burn),
        std::make_tuple(STAKE_ACCOUNT, quantity, std::string("Burn inner token due to unstake outer token"))
    ).send();
}

//fee_rate >= 1
void dexchain::createdex(account_name creator, account_name dex_account, asset base, asset quote, uint32_t fee_rate)
    {
        require_auth( creator );
        eosio_assert( fee_rate > 0, "fee_rate should be larger than 0" );
        assertAsset(base);
        assertAsset(quote);

        auto existing = _dexmarket.find( dex_account );

        eosio_assert(existing == _dexmarket.end(), "dex market already existed");

        //Send staked base token to dex account
        action(
                permission_level{ creator, N(active) },
                _self, N(transfer),
                std::make_tuple(creator, dex_account, base, std::string("Send base token to dex account"))
        ).send();

        action(
                permission_level{ dex_account, N(active) }, //Must assign eosio.code of dex_account to this contract
                _self, N(approve),
                std::make_tuple(dex_account, base, std::string("approve token"))
        ).send();

        //Send staked quote token to dex account
        action(
                permission_level{ creator, N(active) },
                _self, N(transfer),
                std::make_tuple(creator, dex_account, quote, std::string("Send quote token to dex account"))
        ).send();

         action(
                permission_level{ dex_account, N(active) },
                _self, N(approve),
                std::make_tuple(dex_account, quote, std::string("approve token"))
        ).send();

        _dexmarket.emplace( _self, [&]( auto& m ) {
            m.dex_account = dex_account;
            m.fee_rate = fee_rate;
            m.supply.amount = 100000000000000ll;
            m.supply.symbol = S(0,DEXCORE);
            m.base.balance.amount = base.amount;
            m.base.balance.symbol = base.symbol;
            m.quote.balance.amount = quote.amount;
            m.quote.balance.symbol = quote.symbol;
        });
    }

void dexchain::closedex(account_name dex_account)
    {
        //TODO:Add a delay action to close a dex
        //延迟关闭去中心化交易对，以给参与者足够的时间交易token
        require_auth( dex_account );

        auto existing = _dexmarket.find( dex_account );

        eosio_assert(existing != _dexmarket.end(), "dex market not existed");

        auto& st = *existing;
        _dexmarket.erase( st );

        auto asset = get_balance2(dex_account, st.base.balance.symbol.name());
        print("base,", asset, "\n");
        action(
                permission_level{ APPROVE_ACCOUNT, N(active) },
                _self, N(disapprove),
                std::make_tuple(dex_account, asset, std::string("disapprove base token"))
        ).send();

        asset = get_balance2(dex_account, st.quote.balance.symbol.name());
        print("quote,", asset, "\n");
        action(
                permission_level{ APPROVE_ACCOUNT, N(active) },
                _self, N(disapprove),
                std::make_tuple(dex_account, asset, std::string("disapprove quote token"))
        ).send();
    }

void dexchain::buytoken(account_name owner, account_name receiver, account_name dex_account, asset quant) {
        require_auth( owner );

        auto existing = _dexmarket.find( dex_account );
        eosio_assert(existing != _dexmarket.end(), "dex market not existed");

        const auto& dex = *existing;

        eosio_assert( quant.amount > 0, "must purchase a positive amount" );
        auto fee = quant;
        fee.amount = ( fee.amount + dex.fee_rate - 1 ) / dex.fee_rate; /// fee_rate=200 when .5% fee
        auto quant_after_fee = quant;
        quant_after_fee.amount -= fee.amount;

        eosio_assert( quant_after_fee.amount > 0, "quant_after_fee must a positive amount" );

        //Send staked quote token to dex account
        action(
                permission_level{ owner, N(active) },
                _self, N(transfer),
                std::make_tuple(owner, dex_account, quant, std::string("Send quote token to dex account"))
        ).send();

        action(
                permission_level{ dex_account, N(active) }, //Must assign eosio.code of creator to this contract
                _self, N(approve),
                std::make_tuple(dex_account, quant, std::string("approve quote token"))
        ).send();

        asset tokens_out;
        _dexmarket.modify( existing, 0, [&]( auto& es ) {
            tokens_out = es.convert( quant_after_fee,  dex.base.balance.symbol );
        });
        eosio_assert( tokens_out.amount > 0, "must reserve a positive amount" );
        print("token exchange,", quant_after_fee.amount, ":", tokens_out.amount);

        //从交易所转出
        action(
                permission_level{ APPROVE_ACCOUNT, N(active) },
                _self, N(disapprove),
                std::make_tuple(dex_account, tokens_out, std::string("disapprove base token"))
        ).send();

        action(
                permission_level{ dex_account, N(active) },
                _self, N(transfer),
                std::make_tuple( dex_account, receiver, tokens_out, std::string("Send token to receiver"))
        ).send();
    }

void dexchain::selltoken(account_name owner, account_name receiver, account_name dex_account, asset quant)
    {
        require_auth( owner );

        auto existing = _dexmarket.find( dex_account );
        eosio_assert(existing != _dexmarket.end(), "dex market not existed");

        auto dex = *existing;

        eosio_assert( quant.amount > 0, "must purchase a positive amount" );
        auto fee = quant;
        fee.amount = ( fee.amount + dex.fee_rate - 1 ) / dex.fee_rate; /// fee_rate=200 when .5% fee
        auto quant_after_fee = quant;
        quant_after_fee.amount -= fee.amount;

        eosio_assert( quant_after_fee.amount > 0, "quant_after_fee must a positive amount" );

        action(
                permission_level{ owner, N(active) },
                _self, N(transfer),
                std::make_tuple(owner, dex_account, quant, std::string("Send base token to dex account"))
        ).send();

        action(
                permission_level{ dex_account, N(active) }, //Must assign eosio.code of dex_account to this contract
                _self, N(approve),
                std::make_tuple(dex_account, quant, std::string("approve base token"))
        ).send();

        asset tokens_out;
        _dexmarket.modify( existing, 0, [&]( auto& es ) {
            tokens_out = es.convert( quant_after_fee,  dex.quote.balance.symbol );
        });
        eosio_assert( tokens_out.amount > 0, "must reserve a positive amount" );

        //从交易所转出
        action(
                permission_level{ APPROVE_ACCOUNT, N(active) },
                _self, N(disapprove),
                std::make_tuple(dex_account, tokens_out, std::string("disapprove quote token"))
        ).send();
        action(
                permission_level{ dex_account, N(active) },
                _self, N(transfer),
                std::make_tuple(dex_account, receiver, tokens_out, std::string("Send token to receiver"))
        ).send();
    }


void dexchain::claim(account_name account){
    //TODO
}

void refund(account_name dex_account){
    //TODO
}

void resetDex(account_name dex_account, asset maxBase, asset maxQuote)
{
    //TODO:Increase or descrease volume of dex with price keeping same
}

void dexchain::createfund(account_name creator, account_name fund_account, symbol_name base_symbol, symbol_name fund_symbol, uint8_t permission)
    {
        require_auth( creator );
        asset base(0, base_symbol);
        assertAsset(base);
        asset fund(0, fund_symbol);
        assertAsset(fund);

        auto existing = _fundmarket.find( fund_account );

        eosio_assert(existing == _fundmarket.end(), "fund market already existed");

        _fundmarket.emplace( _self, [&]( auto& m ) {
            m.fund_account = fund_account;
            m.base_balance = base;
            m.fund_balance = fund;
            m.permission = permission;
        });
    }

void dexchain::buyfund(account_name payer, account_name receiver, account_name fund_account, asset quantity)
    {
        require_auth( payer );

        auto existing = _fundmarket.find( fund_account );
        eosio_assert(existing != _fundmarket.end(), "fund market not existed");

        const auto& fund = *existing;

        eosio_assert( quantity.amount > 0, "must purchase a positive amount" );
        eosio_assert( fund.fund_balance.symbol == quantity.symbol, "invalid symbol" );
       //Send fund token to fund account
        action(
                permission_level{ payer, N(active) },//Must assign eosio.code of payer to this contract
                _self, N(transfer),
                std::make_tuple(payer, fund_account, quantity, std::string("Send fund token to dex account"))
        ).send();

        action(
                permission_level{ fund_account, N(active) },
                _self, N(approve),
                std::make_tuple(fund_account, quantity, std::string("approve fund token"))
        ).send();

        asset tokens_out;
        _fundmarket.modify( existing, 0, [&]( auto& es ) {
            tokens_out = es.buy( quantity );
        });
        eosio_assert( tokens_out.amount > 0, "must reserve a positive amount" );
        print("fund price,", quantity.amount, ":", tokens_out.amount);

        //从基金转出
        action(
            permission_level{ fund_account, N(active) },
            _self, N(issue),
            std::make_tuple(fund_account, receiver, tokens_out, std::string("Issue base token"))
        ).send();
    }

void dexchain::sellfund(account_name account, account_name fund_account, asset quantity)
    {
        require_auth( account );

        auto existing = _fundmarket.find( fund_account );
        eosio_assert(existing != _fundmarket.end(), "fund market not existed");

        const auto& fund = *existing;

        eosio_assert( quantity.amount > 0, "must sell a positive amount" );
        eosio_assert( fund.fund_balance.symbol == quantity.symbol, "invalid symbol" );
       //Send fund token to fund account
        action(
                permission_level{ account, N(active) },
                _self, N(transfer),
                std::make_tuple(account, fund_account, quantity, std::string("Send fund token to dex account"))
        ).send();
        sub_balance(account, quantity);

        asset tokens_out;
        _fundmarket.modify( existing, 0, [&]( auto& es ) {
            tokens_out = es.sell( quantity );
        });
        eosio_assert( tokens_out.amount > 0, "must reserve a positive amount" );
        print("fund price,", quantity.amount, ":", tokens_out.amount);

        action(
                permission_level{ fund_account, N(active) }, //Must assign eosio.code of creator to this contract
                _self, N(disapprove),
                std::make_tuple(fund_account, tokens_out, std::string("disapprove fund token"))
        ).send();
        //从基金转出
        action(
            permission_level{ fund_account, N(active) }, //Must assign eosio.code of creator to this contract
            _self, N(transfer),
            std::make_tuple(fund_account, account, tokens_out, std::string("refund token"))
        ).send();
    }

void dexchain::addfund(account_name account, account_name fund_account, asset quantity)
    {
        require_auth( account );

        auto existing = _fundmarket.find( fund_account );
        eosio_assert(existing != _fundmarket.end(), "fund market not existed");

        const auto& fund = *existing;

        action(
                permission_level{ account, N(active) },
                _self, N(transfer),
                std::make_tuple(account, fund_account, quantity, std::string("Send fund token to fund account"))
        ).send();

        action(
                permission_level{ fund_account, N(active) }, //Must assign eosio.code of creator to this contract
                _self, N(approve),
                std::make_tuple(fund_account, quantity, std::string("approve fund token"))
        ).send();

        _fundmarket.modify( existing, 0, [&]( auto& es ) {
            es.addfund( quantity );
        });
    }

void dexchain::delegatebw(account_name fund_account, account_name receiver, asset net, asset cpu)
    {
        require_auth( fund_account );

        auto existing = _fundmarket.find( fund_account );
        eosio_assert(existing != _fundmarket.end(), "fund market not existed");

        const auto& fund = *existing;
        eosio_assert(fund.fund_balance.symbol == DEXCHAIN_EOS_SYM, "only xeos can be rent");
                eosio_assert((fund.permission & FUND_PERMISSION_DELEGATE) == 1, "no delegate permission");

        asset all_amount = net + cpu;

        action(
                permission_level{ fund_account, N(active) },
                _self, N(disapprove),
                std::make_tuple(fund_account, all_amount, std::string("disapprove fund token"))
        ).send();

        action(
                permission_level{ fund_account, N(active) },
                _self, N(transfer),
                std::make_tuple(fund_account, DELEGATE_ACCOUNT, all_amount, std::string("transfer token to delegate account to delegate"))
        ).send();


        fundusages _fundusages( _self, fund_account );
        auto id = _fundusages.available_primary_key();
        _fundusages.emplace( _self, [&]( auto& m ) {
            m.id = id;
            m.amount = net;
            m.amount2 = cpu;
            m.fund_account = fund_account;
            m.type = FUND_USAGE_TYPE_DELEGATEBW;
            m.create_time = now();
        });

        //Delegatebw to receiver
        action(
                permission_level{ DELEGATE_ACCOUNT, N(active) },
                SYSTEM_CONTRACT, N(delegatebw),
                std::make_tuple(DELEGATE_ACCOUNT, receiver, net, cpu, false)
        ).send();


        //TODO:add delay action to undelegate cpu after one day


        _fundmarket.modify( existing, 0, [&]( auto& es ) {
            es.used_balance.amount += all_amount.amount;
        });
    }

    void dexchain::updateusage( account_name fund_account ) {
        ///TODO:遍历fund_usages table,找出到期的delegate
    }

    //TODO:
    void dexchain::voteproducer( account_name fund_account, const account_name voter, const account_name proxy, const std::vector<account_name>& producers ) {

    }

    //TODO:
    void dexchain::withdrawfund( account_name fund_account, const account_name voter, const account_name proxy, const std::vector<account_name>& producers ) {

    }

}/// namespace dex

EOSIO_ABI( dex::dexchain, (create)(createbystake)(issue)(transfer)(approve)(disapprove)(stake)(unstake)(createdex)(closedex)(buytoken)(selltoken)(createfund)(buyfund)(sellfund)(addfund)
 )
