# Eos-DexChain Fund Model
# 社区(Contact)
&nbsp;&nbsp;&nbsp;&nbsp;项目网站:https://github.com/DexChain <br>
&nbsp;&nbsp;&nbsp;&nbsp;币乎:https://bihu.com/people/224762<br>
&nbsp;&nbsp;&nbsp;&nbsp;电报:https://t.me/dexchain<br>
&nbsp;&nbsp;&nbsp;&nbsp;公众号:区块链斜杠青年<br>
&nbsp;&nbsp;&nbsp;&nbsp;欢迎交流技术及合作，有任何问题请加我微信:itleaks<br>
<img src="https://raw.githubusercontent.com/itleaks/eos-contract/master/files/weixin.png" width=200 height=256 />
# 简介
&nbsp;&nbsp;&nbsp;&nbsp;上周在我的文章里提到过，去中心化链里的通证的价值锚定在生态成熟之前比较有难度，因而我在是思考一种代码保障的价值通证。目前阶段，要想实现价值通证，必须从EOS内部生态入手，因而我提出了一个基金币模型。<br>
&nbsp;&nbsp;&nbsp;&nbsp;基金币采用100%的bancor模型，用户使用基金母币(一般是比较稳定的价值币，比如XEOS/EOS)购买基金币(比如VEX,投票代理基金币)，基金币可随时按当前市价兑换成母币。基金币分为两类:
* 保本型基金币<br>
  基金币只涨不跌，价格随着基金币的收益增加而上涨，早期购买，可以早点获取收益，任何时候购买都不晚，永不亏本金。此类基金币的经典场景是EOS CPU租赁市场，投票代理市场等。这种市场，基金管理人员(可以是代码)只可把募集到的母币用于出租，投票，是肯定可以回收的(所以不亏本金)，同时租金，投票收益记入母币总量，从而基金币价格增长，投资人获取收益。
* 非保本型基金<br>
  基金管理人员可以支配一定比例的母币去投资以获取收益，进而提高了基金总价值，对应的基金币价格上涨。如果亏损，则反之。此类基金币的场景更广泛，比如量化交易团体，传统行业投资团队。

## Eos-DexChain FUND API
### createfund(创建基金币)
#### 介绍
```
createfund(account_name creator, account_name fund_account, symbol_name base, symbol_name fund, uint8_t permission)
```
* creator<br>
   基金币创建人，任何账号都可调用该接口创建场内币
* fund_account<br>
  基金币管理账号
* base<br>
  基金币
* fund<br>
  母币
* permission<br>
  母币操作权限，比如只能代理，投票等

### buyfund(购买基金)
#### 介绍
```
void buyfund(account_name payer, account_name receiver, account_name fund_account, asset quantity);
```
* player<br>
   购买人
* receiver<br>
   基金币接收账号
* fund_account<br>
  基金管理账号
* quantity<br>
   购买量

### sellfund(购买基金)
#### 介绍
```
void sellfund(account_name account, account_name receiver, account_name fund_account, asset quantity);
```
* account<br>
   出售账号
* fund_account<br>
  基金管理账号
* quantity<br>
   出售量

### addfund(增加基金母币)
#### 介绍
```
void addfund(account_name account, account_name fund_account, asset quantity);
```
* account<br>
   账号
* fund_account<br>
  基金管理账号
* quantity<br>
   增加量

比如租赁CPU，投票获得的收益就可以通过这个接口增加基金价值

## License
&nbsp;&nbsp;&nbsp;&nbsp;该项目是一个区块链4.0通证经济去中心化链的试验性探索链，遵循MIT License, 任何企业或个体可在MIT规范下使用本项目的源码。<br>
&nbsp;&nbsp;&nbsp;&nbsp;由于该项目是一个试验性探索项目，目前该项目已经上线jungle测试网络，智能合约账号dexchaincode, 方便大家测试实践。

## Contact
&nbsp;&nbsp;&nbsp;&nbsp;项目网站:https://github.com/DexChain <br>
&nbsp;&nbsp;&nbsp;&nbsp;币乎:https://bihu.com/people/224762<br>
&nbsp;&nbsp;&nbsp;&nbsp;电报:https://t.me/dexchain<br>
&nbsp;&nbsp;&nbsp;&nbsp;公众号:区块链斜杠青年<br>
&nbsp;&nbsp;&nbsp;&nbsp;欢迎交流技术及合作，有任何问题请加我微信:itleaks<br>
<img src="https://raw.githubusercontent.com/itleaks/eos-contract/master/files/weixin.png" width=200 height=256 />