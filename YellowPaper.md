# "区块链4.0" DexChain，通证经济及去中心化交易链探索
&nbsp;&nbsp;&nbsp;&nbsp;首先看到这里，你可能会有点失望，区块链4.0怎么不是新的共识算法，新的记账系统，新的DAPP虚拟机，新的….<br>
&nbsp;&nbsp;&nbsp;&nbsp;这里的区块链4.0简单来说就是通证经济，价值即token, token呈现价值，去中心化交换价值传递价值。最近币改是区块链非常热门的话题，很多团队都在做通证经济学相关的理论研究，以探索币改生态。其实币改只是区块链4.0的一个落地点，只要你的token(代币)有某种实际价值或权益，你的token就有价值，就会反应到token的价格上，近到你小超市的购物券，大到公司股票权益。

# 传统交易所
&nbsp;&nbsp;&nbsp;&nbsp;推广开来的通证在传统交易所是不可能实现的，高企的上币费，高难度的审核，小价值容量的币根本没有露面的机会。再者，目前传统交易所上币高门槛，导致币壳稀缺，币壳比币本身值钱，进而导致区块链代币只注重宣传，而不重视价值，绝大部分不需要token的场景也发币圈钱。因而token价值流动的载体交易所变革才是区块链4.0的基石，这个变革就是去中心化交易所。大家可能会说，去中心化交易所不是早就有了吗？其实不然，传统的去中心化交易所都是撮合型(买单和卖单)的交易所，在总量少，交易不及时的负反馈下，交易量只会更少，更不及时。因而高效实时的去中心化交易所的诞生才是区块链4.0的萌芽。

# 去中心化交易所
&nbsp;&nbsp;&nbsp;&nbsp;大家很多人最近可能都在骂BM，其实BM在EOS引入的很多经济模型都是很牛逼的，最简单的就是RAM交易市场引入的Bancor机制，这个是真正意义上的区块链去中心化交易所的鼻祖，当然这个交易所也是有一些问题的，但新鲜事物出来都是需要进化的。<br>
&nbsp;&nbsp;&nbsp;&nbsp;目前所有号称去中心化的交易所中，只有刚发布的币安去中心化交易所链Binance Chain才是最接近可以承载广义通证经济区块链4.0的，但它也才刚刚发布。

# 区块链4.0标准

&nbsp;&nbsp;&nbsp;&nbsp;区块链4.0标准
```
1）零成本发token
1) 零成本上交易所流通
3）去中心化交易所
    去中心化交易所会借助市场机制完成通证token的价格形成及价值交换
4）高TPS
5）高安全性
6）信用体系
7）可只支持结算层
8) 高兼容及扩展性，可接入已有主流公链
```
&nbsp;&nbsp;&nbsp;&nbsp;区块链4.0之前，只有大佬们是价值标的物的发球人，我们个体只是价值的传球手。4.0之后，任何有价值有能力的人都可以上球场玩，成为价值的发球人。<br>
&nbsp;&nbsp;&nbsp;&nbsp;区块链4.0之前，似乎只有和区块链链本身相关的标的物才有价值，可惜的是绝大部分和区块链相关的其实最后也没有真正价值。区块链4.0之后，大量真正能看到价值的标的物上链了，礼品卡，购物券，家政服务，软件外包服务，公司股权，只要市场认可，皆可创币上链，价格形成交给市场。

# DexChain
&nbsp;&nbsp;&nbsp;&nbsp;好了，到了介绍项目的时候了。DexChain是一个试验性的去中心化交易所链，是一个以探索去中心交易及通证经济为目标的链。首先DexChain将在EOS生态探索，后面会支持更多生态，合适的时机最后会形成一个统一的公链。<br>
&nbsp;&nbsp;&nbsp;&nbsp;DexChain免费开源，本身不做任何商业化运作，只是一个实验性的网络，用来学习探索通证经济去中心化交易所，我们力量很微薄，我们也仅在尝试中，欢迎大家一起参与，一起促进区块链行业的发展，大家可以按照MIT License标准自由使用DexChain。

# Eos-DexChain
&nbsp;&nbsp;&nbsp;&nbsp;Eos-DexChain是DexChain在EOS生态下的一种试验性实现。为了更好的理解Eos-DexChain,我们再次重述一下区块链4.0的标准。
```
1）零成本发token
2) 零成本上交易所流通
3）去中心化交易所
    去中心化交易所会借助市场机制完成通证token的价格形成及价值交换
4）高TPS
5）高安全性
6）信用体系
7）可只支持结算层
8) 高兼容及扩展性，可接入已有主流公链
```
## 高TPS,高安全性
&nbsp;&nbsp;&nbsp;&nbsp;EOS本身具备高TPS和一定的高安全性，因而在EOS上创建一个智能合约来实现EOS生态的DexChain是最简单最直接的方式了。
## 零成本发币
&nbsp;&nbsp;&nbsp;&nbsp;Eos-DexChain实现一个增强的eosio.token的智能合约，能够比较便捷的实现该功能。<br>
&nbsp;&nbsp;&nbsp;&nbsp;Eos-DexChain的币分为两种
### 场内币（create）
&nbsp;&nbsp;&nbsp;&nbsp;这种币是给那些大量尾部需求的发币人准备的。通过Eos-DexChain发行场内币，只需消耗很少量的RAM（<1k）即可实现，不再需要部署发币合约而消耗大量的金钱，真正实现了零成本发币。且Eos-DexChain内置推广功能，比如主动领取空投，转账领取空投。同时场内币的交易很简单，非常容易实现流动性，更多信息将在交易一节讲解。
### 抵押币 (createbystake)
&nbsp;&nbsp;&nbsp;&nbsp;抵押币是为了给那些已经发行的币准备的，已经发行的币要在DexChain交易流通，必须先通过抵押方式转换为抵押币，场外币和Eos-DexChain的抵押币按1:1的比例发行。抵押一个场外比，Eos-DexChain发行一个抵押币。通过1：1抵押的方法实现了场外币和抵押币的价值绑定，且这个抵押发行过程是完全代码控制的，安全且可靠。
## 零成本上交易所，去中心化交易所（createdex)
&nbsp;&nbsp;&nbsp;&nbsp;Eos-DexChain是一个去中心化链，一个币币交易对你可以看成是一个去中心化交易所。
### 创建交易对
&nbsp;&nbsp;&nbsp;&nbsp;Eos-DexChain内采用Bancor机制来实现币币交易，Bancor实时交易是目前最好的去中心化交易所的模型。任何用户可以通过抵押交易币对的方式免费自由创建币(场内币,抵押币)交易对。
### 币币交易对初始价格
&nbsp;&nbsp;&nbsp;&nbsp;交易对的初始价格由抵押的量的比例决定。比如用户通过抵押1000EETH，10EOS来创建一个交易对EETH/EOS，那么初始的EETH价格是0.01EOS，后续价格由交易对的买卖交易市场形成。由于创建交易对需要抵押，因而是有成本的，如果定价不合理，创建者会损失资产。比如如果EETH抵押价格过高，就会有EETH持有者卖掉EETH获取EOS, 从而创建者的抵押的EOS变少了，总价值也减少了。因而该机制会促使创建者定一个他觉得合理的价格。
### 代币价格形成机制
&nbsp;&nbsp;&nbsp;&nbsp;cw=0.5的Bancor交易所初始价格变化比较平缓，后面比较剧烈，一开始的50%的交易量只会导致价格上涨一倍或者下跌一倍左右，因而市场价格形成不够迅速，因此开始可采取少量抵押量的方式先让市场先形成价格。价格形成后，可能会进入曲线的快速变动段，因而少量交易就会导致价格大波动，该阶段比较容易进入投机阶段，即大量买进导致价格飞涨（具体可参考RAM暴涨期）。由于价格急剧飞涨，交易对创建者也有巨额收入，他也应该乐于坐享其成，真相难道真会是这样的吗？非也，不像RAM市场是由BM,BP一小撮人控制的，且变动前必须提前通知，因而不够市场化。Eos-DexChain市场内，交易对是可以任意创建的，且相同的交易对也可以创建多个。如果前面的价格飞涨是因为老的交易所内的容量不够导致的，且超出了合理价格，那么其他人就会重开一个容量更大的相同的交易对来增加交易量，进而抑制了价格的继续上涨。新的交易所对老交易所的竞争会促使老交易所增加bancor的交易容量，进而减少波动。同时为了减少价格波动和抑制老交易所杀熟投机，老交易所必须按照当前价增加容量<br>
### 币币交易对常见创建者
* 1）代币的发行人或者大户<br>
    交易才能产生价值，才能形成价格，因而代币的发行人是最有动力去做这个事情的。也只有他们最有动力愿意冒着损失一部分抵押币的风险来创建。
* 2）交易所老手<br>
    交易对价格稳定后，就会有成熟的交易所老手开建相同的交易对以更好的服务赚取手续费。
### 代币(Token通证)的真正价值
&nbsp;&nbsp;&nbsp;&nbsp;交易形成价格，投机交易也可能导致不合理价格，但是核心的还是代币的真正价值。具体到Eos-DexChain，场内币的价值由该代币所能绑定的实际价值决定。比如小超市的购物券币，就由该购物券所能使用的超市数量，用户量决定。抵押币通过代码1:1绑定后，抵押币的价值就完全和抵押的币的价值一致。比如XEOS(Eos-DexChain抵押币)就和EOS的价值一样。
### Eos-DexChain交易的各种安全保障
* 抵押币的安全。<br>
抵押币发行时会有对应量的场外币进入Eos-DexChain合约的抵押账号，因而抵押币是肯定可以提现的。比如1XEOS是肯定可以兑换成1EOS的。由于用作抵押的币的资金操作必须要赋值智能合约eosio.code权限，从而导致用户的场外币长时间可被智能合约操作。EOS-DexChain的抵押币设计了Stake,Unstake操作，只有执行这两个action时才需要eosio.code权限，其他抵押币交易阶段不需要eosio.code权限，大大降低了eosio.code权限的暴露时间。
* 交易所的安全<br>
交易所管理账号的资金都是受合约监管的。EOS-DexChain新建了approve机制，交易所账号的所有资金都approve给了合约，这些资金只有智能合约能够管理。只有在交易所关闭时，才能提取这些资金，且关闭交易所会有延时，从而有足够时间让交易所的用户做出合理的响应，比如及时买卖。
* 合约的安全<br>
  DexChain合约推荐使用该文<a href="https://bihu.com/article/1042556">【固化EOS智能合约，监管升级权限，净化EOS DAPP生态】</a>的建议，经过测试周期后，固化移交升级权限，实现合约自身的安全
## Eos-DexChain API
### create(创建场内币)
#### 介绍
```
void create( account_name issuer, asset  maximum_supply)
```
* issuer<br>
   场内币发行人，任何账号都可调用该接口创建场内币
* maximum_supply<br>
    最大发行量

#### 示例
```
cleos push action dexchaincode create '["useruseridx1", "100000000.0000 XTEST"]' -p useruseridx1
```
上图就创建了一个XTEST场内币，然后你就可以直接使用这个币宣传运营了
useruseridx1

### createbystake(创建抵押币)
#### 介绍
```
void createbystake(asset           maximum_supply,
                    asset           stake,
                    account_name    stake_contract);
```
和场内币创建不一样，抵押币没有发行账号issuer，抵押一个就发行一个.且只能是抵押的场外币的合约拥有者才可调用
* stake<br>
   抵押的场外币的资金量
* stake_contract<br>
   抵押的场外币的智能合约账号
* maximum_supply<br>
    最大发行量

#### 示例
```
cleos push action dexchaincode createbystake '["1000000.0000 XEETH", "0.0000 EETH", "eosfavorcomm", "create new token XEETH by stake EETH"]' -p eosfavorcomm
```
上图就创建了一个XEETH抵押币，然后任何EETH的持有人都可以通过stake将EETH转化为XEETH抵押币并进行交易

### stake(抵押获得抵押币)
#### 介绍
```
void stake(account_name player, account_name receiver,
            symbol_type token_symbol, asset stake,
            account_name stake_contract,
            string memo )
```
* player<br>
   抵押的场外币的消耗账号
* receiver<br>
   抵押币的接收账号
* token_symbol<br>
   要获取的抵押币的符号信息
* stake<br>
   抵押的场外币的资金量
* stake_contract<br>
   抵押的场外币的智能合约账号

#### 示例
```
cleos push action dexchaincode stake '["useruseridx2", "useruseridx3", "4,XEETH", "8000.1000 EETH", "eosfavorcomm", "stake EETH to get XEETH"]' -p useruseridx2
```
useruseridx3会接收到8000.1000 XEETH

### unstake(取消抵押赎回已抵押的场外币)
#### 介绍
```
void unstake(account_name player, account_name receiver,
            asset quantity, symbol_type stake_symbol,
            account_name stake_contract,
            string memo )
```
* player<br>
   抵押的场外币的消耗账号
* receiver<br>
   抵押币的接收账号
* quantity<br>
   抵押币的数量
* stake_symbol<br>
   要获取的场外币的符号信息
* stake_contract<br>
   抵押的场外币的智能合约账号

#### 示例
```
cleos push action dexchaincode unstake '["useruseridx3", "useruseridx2", "1.1000 XEETH", "4,EETH", "eosfavorcomm", "unstake XEETH to get EETH"]' -p useruseridx3
```
useruseridx2会接收到8000.1000 EETH

### createdex(创建交易对交易所)
#### 介绍
```
void createdex(account_name creator, account_name dex_account,
    asset base, asset quote, uint32_t fee_amount);
```
* creator<br>
   交易对的创建者，也是抵押币的消耗者
* dex_account<br>
   交易所管理账号
* base<br>
   bancor交易所的base币
* quote<br>
   bancor交易所的quote币
* fee_amount<br>
   交易费，= 1/fee_amount, 比如200就是0.5%的手续费

#### 示例
```
cleos push action dexchaincode createdex '["useruseridx3", "onefamousdex", "1000.0000 XTEST", "10.0000 XEOS", 200]' -p useruseridx3
```
创建一个交易所对，管理账号时onefamousdex, 交易对是XTEST/XEOS, XTEST的初始价格是10/1000=0.01 XEOS

### buytoken(买base币)
#### 介绍
```
void buytoken(account_name owner, account_name receiver, account_name       dex_account, asset quat);

```
* owner<br>
   交易者
* receiver<br>
   币的接收账号
* dex_account<br>
   交易对管理账号
* quat<br>
   用来购买base币的quote币量

#### 示例
```
cleos push action dexchaincode buytoken '["useruseridx3", "useruseridx5", "onefamousdex", "0.1000 XEOS"]' -p useruseridx3
```
useruseridx5将获得XTEST币

### selltoken(卖base币)
#### 介绍
```
void selltoken(account_name owner, account_name receiver, account_name dex_account, asset quant);
```
* owner<br>
   交易者
* receiver<br>
   币的接收账号
* dex_account<br>
   交易对管理账号
* quat<br>
   卖掉的base币的量

#### 示例
```
cleos push action dexchaincode selltoken '["useruseridx3", "useruseridx5", "onefamousdex", "0.1000 XTEST"]' -p useruseridx3
```
useruseridx5将获得XEOS币

### closedex(关闭交易所)
#### 介绍
```
void closedex(account_name dex_account)
```
* dex_account<br>
   交易对管理账号

#### 示例
```
cleos push action dexchaincode closedex '["onefamousdex"]' -p onefamousdex
```
onefamousdex账号对应的交易对关闭

### 获取交易所信息
```
> cleos get table dexchaincode dexchaincode dexmarkets
{
  "rows": [{
      "supply": "100000000000000 DEXCORE",
      "base": {
        "balance": "980.5879 XTEST",
        "weight": "0.50000000000000000"
      },
      "quote": {
        "balance": "10.1980 XEOS",
        "weight": "0.50000000000000000"
      },
      "dex_account": "onefamousdex",
      "fee_rate": 200
    }
  ],
  "more": false
}

```

### 获取代币信息
```
> cleos get table dexchaincode XEOS stat
{
  "rows": [{
      "supply": "0.0000 XEOS",
      "max_supply": "10000000000.0000 XEOS",
      "issuer": "dexchaincode",
      "stake_symbol": "4,EOS",
      "stake_contract": "eosio.token"
    }
  ],
  "more": false
}
```

### 获取代币持有
```
> cleos get currency balance dexchaincode useruseridx3
7989.0000 XEOS
4000.1000 XTEST
```
### 汇总
&nbsp;&nbsp;&nbsp;&nbsp;上面的EOS可以是任意其他已经发行的场外币，XTEST可以是Eos-DexChain合约里的任意币(场内币和抵押币)

## 编译部署
```
cd dexchain
eosiocpp -o dexchain.wast exchange_state.cpp dexchain.cpp
cleos set contract your_account ./dexchain/ -p your_account
```
## License
&nbsp;&nbsp;&nbsp;&nbsp;该项目是一个区块链4.0通证经济去中心化链的试验性探索链，遵循MIT License, 任何企业或个体可在MIT规范下使用本项目的源码。<br>
&nbsp;&nbsp;&nbsp;&nbsp;该项目是一个试验性探索项目，目前该项目已经上线jungle测试网络，智能合约账号dexchaincode, 方便大家测试实践。
## 其他生态实现
以太坊等其他生态的DexChain将视情况适时开发推进
## Contact
&nbsp;&nbsp;&nbsp;&nbsp;项目网站:https://github.com/DexChain <br>
&nbsp;&nbsp;&nbsp;&nbsp;币乎:https://bihu.com/people/224762<br>
&nbsp;&nbsp;&nbsp;&nbsp;电报:https://t.me/dexchain<br>
&nbsp;&nbsp;&nbsp;&nbsp;公众号:区块链斜杠青年<br>
&nbsp;&nbsp;&nbsp;&nbsp;欢迎交流技术及合作，有任何问题请加我微信:itleaks<br>
<img src="https://raw.githubusercontent.com/itleaks/eos-contract/master/files/weixin.png" width=200 height=256 />
