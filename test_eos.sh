#!/usr/bin/env bash
## 参考
#http://fileem.com/eos-private-chain-building-based-on-v1-8-5
git clone https://github.com/EOSIO/eos
cd eos
git checkout v1.8.14
git submodule update --recursive 
./scripts/eosio_build.sh -o Debug
sudo ./scripts/eosio_install.sh
export PATH=$PATH:$HOME/eosio/1.8/bin
nodeos 
kill `pgrep nodeos`
cp $当前文件夹/config.ini $HOME/.local/share/eosio/nodeos/config
nodeos
cleos get info 
# 有正常输出表示启动完成

# 
cleos wallet create -n test  --to-console
' 如下输出
Creating wallet: test
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
"PW5K9en6N7Amdbf9p6T5cL7AJ4eDUMVVZpxt6ecT8gu2CtyUCgTnU"
'
# 解索钱包
cleos wallet unlock -n test --password PW5K9en6N7Amdbf9p6T5cL7AJ4eDUMVVZpxt6ecT8gu2CtyUCgTnU

cleos wallet create_key -n test
'
Created new private key with a public key of: "EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad"
'
cleos wallet create_key -n test
'
Created new private key with a public key of: "EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao"
'
cleos  wallet private_keys -n test --password PW5K9en6N7Amdbf9p6T5cL7AJ4eDUMVVZpxt6ecT8gu2CtyUCgTnU
'
[[
    "EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao",
    "5Kcyku1m671B52kB6xupF1SqXwpADcj8kD8JQPtQ3BhdCd8WcPb"
  ],[
    "EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad",
    "5JFFWLGHTGRMF5y8b9mMrbjz6ZiJJcG6d1yYLdUkL7mAR3mF4pQ"
  ]
]
'

# 导入eosio账户私钥，该私钥是系统默认自带的已经硬编码到系统中
cleos wallet import -n test --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
'
imported private key for: EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
'

# 创建test账户
cleos create account eosio test EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
'
executed transaction: 0eb7fc4f317c32fd2b6b591d5fe71a2f7716f529a756e66a591861a68d6e3d34  200 bytes  732 us
#         eosio <= eosio::newaccount            {"creator":"eosio","name":"test","owner":{"threshold":1,"keys":[{"key":"EOS6zmEuqUpPzQ7kjiVK6vcsTCD3...
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
'

# 查看账户信息
cleos get account test
'
created: 2021-02-08T02:21:50.000
permissions: 
     owner     1:    1 EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao
        active     1:    1 EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
memory: 
     quota:       unlimited  used:      2.66 KiB  

net bandwidth: 
     used:               unlimited
     available:          unlimited
     limit:              unlimited

cpu bandwidth:
     used:               unlimited
     available:          unlimited
     limit:              unlimited
'


############# 部署合约 ############
# 查看组件
curl -X POST http://127.0.0.1:8888/v1/producer/get_supported_protocol_features -d '{}' | jq
# 开启组件
curl -X POST http://127.0.0.1:8888/v1/producer/schedule_protocol_feature_activations -d '{"protocol_features_to_activate": ["0ec7e080177b2c02b278d5088611686b49d739925a92d9bfcacd7fc6b74053bd"]}' | jq
'
{
  "result": "ok"
}
'
# 
cd build/unittests
cleos set contract eosio contracts/eosio.bios -p eosio
'
Reading WASM from /home/shang/shangqd/eos/build/unittests/contracts/eosio.bios/eosio.bios.wasm...
Publishing contract...
executed transaction: 716174da4156c298f9e30a7220c18124b413c80889ad79255ce5bdbef6c076be  7024 bytes  5973 us
#         eosio <= eosio::setcode               {"account":"eosio","vmtype":0,"vmversion":0,"code":"0061736d01000000019d011a60000060037f7e7f0060027f...
#         eosio <= eosio::setabi                {"account":"eosio","abi":"0e656f73696f3a3a6162692f312e310019086162695f686173680002056f776e6572046e61...
<4>warn  2021-02-08T02:28:46.524 cleos     main.cpp:501                  print_rwarning: transaction executed locally, but may not be confirmed by the network yet
'

cleos create account eosio eosio.bpay EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.msig EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.names EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.ram EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.ramfee EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.saving EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.stake EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.token EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.vpay EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos create account eosio eosio.rex EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad

# 部署eosio.token合约，部署账户为eosio.token
cleos set contract eosio.token contracts/eosio.token -p eosio.token

# 
cleos  set  contract  eosio.msig  contracts/eosio.msig  -p  eosio.msig

# 创建eos 
cleos  push  action  eosio.token  create  '[ "eosio", "1000000000.0000 EOS"]' -p eosio.token
# 增发eos
cleos  push  action  eosio.token  issue  '[ "eosio", "1000000000.0000 EOS", "test message"]' -p eosio

# 查看
cleos get currency stats eosio.token EOS

# 转账
cleos push action eosio.token transfer '[ "eosio", "test", "50000.0000 EOS", "hello eos" ]' -p eosio

# 查看test账户eos余额
cleos get currency balance eosio.token test

##########################################################################################################################
### 钱包已经准备好从创始块开始快速测试
cleos wallet unlock -n test --password PW5K9en6N7Amdbf9p6T5cL7AJ4eDUMVVZpxt6ecT8gu2CtyUCgTnU
cleos create account eosio test EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad
cleos get account test

curl -X POST http://127.0.0.1:8888/v1/producer/schedule_protocol_feature_activations -d '{"protocol_features_to_activate": ["0ec7e080177b2c02b278d5088611686b49d739925a92d9bfcacd7fc6b74053bd"]}' | jq

cleos create account eosio eosio.token EOS6zmEuqUpPzQ7kjiVK6vcsTCD3RCGyr6SoacAptPuXCoPzLkQao EOS73a9XYkvJTGfwi4Gy91DxGDz9GLCLGzcWYuD7dvfYjT3K3iNad

cleos set contract eosio.token contracts/eosio.token -p eosio.token
cleos  push  action  eosio.token  create  '[ "eosio", "1000000000.0000 EOS"]' -p eosio.token
cleos  push  action  eosio.token issue '[ "eosio", "1000000000.0000 EOS", "test message"]' -p eosio
cleos push action eosio.token transfer '[ "eosio", "test", "50000.0000 EOS", "hello eos" ]' -p eosio

cleos get currency stats eosio.token EOS
cleos get currency balance eosio.token test
