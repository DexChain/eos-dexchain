#!/bin/bash
LOCAL=true
if [ $LOCAL = true ];then
ROOT_ACCOUNT="eosio"
CLEOS="cleos -u http://127.0.0.1:8888"
else
ROOT_ACCOUNT="itleakstoken"
CLEOS="cleos -u xxxxx"
echo "no local"
fi

echoCmd() {
	echo -e "\033[31;46m"$1"\033[0m"
}

WORK_DIR="$( dirname "$0"  )"
cd $WORK_DIR
echo $WORK_DIR

EOSIO_PRIVATE_KEY=5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
EOSIO_PUBLIC_KEY=EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

#Replace below keys to your keys
KEY_PRIVATE_2=5JsYXh93SPC4qTJRiK6xxxxxxxxxxxxxxx
KEY_PUB_2=EOS5zv6z6r1fmh5Arzzh71iWkxxxxxxxxxxxxxx

$CLEOS wallet create -n test
echo "import keys...."
$CLEOS wallet import -n test $EOSIO_PRIVATE_KEY
$CLEOS wallet import -n test $KEY_PRIVATE_2
sleep 2

echo "Some operation may throw some errors"

if [ $LOCAL = true ];then
$CLEOS create account $ROOT_ACCOUNT dexchaincode $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT eosfavoreeth $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT useruseridx1 $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT useruseridx2 $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT useruseridx3 $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT useruseridx4 $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT useruseridx5 $KEY_PUB_2
$CLEOS create account $ROOT_ACCOUNT onefamousdex $KEY_PUB_2
else
echo "no local"
fi

echoCmd '$CLEOS set contract dexchaincode ../dexchain/ -p dexchaincode'
$CLEOS set contract dexchaincode ../dexchain/ -p dexchaincode

#dexchaincode as stake account, need authorize eosio.code permission to contract dexchaincode
$CLEOS set account permission dexchaincode active '{"threshold": 1,"keys": [{"key":"'$KEY_PUB_2'","weight":1}],"accounts": [{"permission":{"actor":"dexchaincode","permission":"eosio.code"},"weight":1}]}' owner -p dexchaincode@owner

$CLEOS set contract eosfavoreeth ../dexchain/ -p eosfavoreeth

$CLEOS push action eosfavoreeth create '["useruseridx1", "100000000.0000 EETH"]' -p useruseridx1
$CLEOS get table eosfavoreeth EETH stat
$CLEOS push action eosfavoreeth issue '["useruseridx2", "10000.0000 EETH"]' -p useruseridx1
$CLEOS get currency balance eosfavoreeth useruseridx2


$CLEOS push action dexchaincode stake2create '["1000000.0000 XEETH", "0.0000 EETH", "eosfavoreeth", "create new token XEETH by stake EETH"]' -p eosfavoreeth
$CLEOS get table dexchaincode XEETH stat

$CLEOS set account permission useruseridx2 active '{"threshold": 1,"keys": [{"key":"'$KEY_PUB_2'","weight":1}],"accounts": [{"permission":{"actor":"dexchaincode","permission":"eosio.code"},"weight":1}]}' owner -p useruseridx2@owner

sleep 3
echoCmd "Stake EETH to get XEETH"
$CLEOS push action dexchaincode stake '["useruseridx2", "useruseridx3", "4,XEETH", "8000.1000 EETH", "eosfavoreeth", "stake EETH to get XEETH"]' -p useruseridx2
exit

#get useruseridx2 account balance of EETH
echoCmd "$CLEOS get currency balance eosfavoreeth useruseridx2"
$CLEOS get currency balance eosfavoreeth useruseridx2

#get stake account balance of EETH
echoCmd "$CLEOS get currency balance eosfavoreeth dexchaincode"
$CLEOS get currency balance eosfavoreeth dexchaincode
#get useruseridx3 account balance of XEETH
echoCmd "$CLEOS get currency balance dexchaincode useruseridx3"
$CLEOS get currency balance dexchaincode useruseridx3
echoCmd "$CLEOS get table dexchaincode XEETH stat"
$CLEOS get table dexchaincode XEETH stat

echoCmd "Unstake XEETH to get EETH"
$CLEOS push action dexchaincode unstake '["useruseridx3", "useruseridx2", "1.1000 XEETH", "4,EETH", "eosfavoreeth", "unstake XEETH to get EETH"]' -p useruseridx3
$CLEOS get currency balance eosfavoreeth useruseridx2

#get useruseridx2 account balance of EETH
echoCmd "$CLEOS get currency balance eosfavoreeth useruseridx2"
$CLEOS get currency balance eosfavoreeth useruseridx2
#get stake account balance of EETH
echoCmd "$CLEOS get currency balance eosfavoreeth dexchaincode"
$CLEOS get currency balance eosfavoreeth dexchaincode
#get useruseridx3 account balance of XEETH
echoCmd "$CLEOS get currency balance dexchaincode useruseridx3"
$CLEOS get currency balance dexchaincode useruseridx3
echoCmd "$CLEOS get table dexchaincode XEETH stat"
$CLEOS get table dexchaincode XEETH stat

$CLEOS push action dexchaincode create '["useruseridx4", "1000000.0000 XTEST"]' -p useruseridx4
echoCmd "$CLEOS get table dexchaincode XTEST stat"
$CLEOS get table dexchaincode XTEST stat
echoCmd "$CLEOS push action dexchaincode issue '["useruseridx3", "5000.1000 XTEST"]' -p useruseridx4"
$CLEOS push action dexchaincode issue '["useruseridx3", "5000.1000 XTEST"]' -p useruseridx4
echoCmd "$CLEOS get currency balance dexchaincode useruseridx3"
$CLEOS get currency balance dexchaincode useruseridx3

$CLEOS push action dexchaincode createdex '["useruseridx3", "onefamousdex", "1000.0000 XTEST", "10.0000 XEETH", 200]' -p useruseridx3
echoCmd "$CLEOS get table dexchaincode onefamousdex approves"
$CLEOS get table dexchaincode onefamousdex approves
$CLEOS set account permission onefamousdex active '{"threshold": 1,"keys": [{"key":"'$KEY_PUB_2'","weight":1}],"accounts": [{"permission":{"actor":"dexchaincode","permission":"eosio.code"},"weight":1}]}' owner -p onefamousdex
echoCmd "$CLEOS get currency balance dexchaincode useruseridx3"
$CLEOS get currency balance dexchaincode useruseridx3
echoCmd "$CLEOS get currency balance dexchaincode ondex"
$CLEOS get currency balance dexchaincode onefamousdex
sleep 1
echoCmd "$CLEOS get table dexchaincode dexchaincode dexmarkets"
$CLEOS get table dexchaincode dexchaincode dexmarkets

$CLEOS push action dexchaincode buytoken '["useruseridx3", "useruseridx5", "onefamousdex", "0.1000 XEETH"]' -p useruseridx3

$CLEOS get currency balance dexchaincode useruseridx5
sleep 1
$CLEOS push action dexchaincode buytoken '["useruseridx3", "useruseridx5", "onefamousdex", "0.1000 XEETH"]' -p useruseridx3
$CLEOS push action dexchaincode selltoken '["useruseridx3", "useruseridx5", "onefamousdex", "0.1000 XTEST"]' -p useruseridx3

echoCmd "$CLEOS get currency balance dexchaincode useruseridx5"
$CLEOS get currency balance dexchaincode useruseridx5
echoCmd "$CLEOS get currency balance dexchaincode useruseridx3"
$CLEOS get currency balance dexchaincode useruseridx3
echoCmd "$CLEOS get currency balance dexchaincode ondex"
$CLEOS get currency balance dexchaincode onefamousdex
sleep 1
echoCmd "$CLEOS get table dexchaincode dexchaincode dexmarkets"
$CLEOS get table dexchaincode dexchaincode dexmarkets
echoCmd "$CLEOS push action dexchaincode closedex '[\"onefamousdex\"]' -p onefamousdex"
$CLEOS push action dexchaincode closedex '["onefamousdex"]' -p onefamousdex
echoCmd "$CLEOS get currency balance dexchaincode onefamousdex"
$CLEOS get currency balance dexchaincode onefamousdex
echoCmd "$CLEOS get table dexchaincode dexchaincode dexmarkets"
$CLEOS get table dexchaincode dexchaincode dexmarkets
echoCmd "$CLEOS get table dexchaincode XEOS stat"
$CLEOS get table dexchaincode XEOS stat
#$CLEOS push action dexchaincode buytoken '["onefamousdex", "useruseridx3", "useruseridx5", "2.0000 XEETH"]' -p useruseridx3
#$CLEOS get currency balance dexchaincode useruseridx5
