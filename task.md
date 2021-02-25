## 任务
#### 数据回滚
在智能合约端系统准备好了多索引容器可以直接使用（多索引容器大致使用了二级索引，一级表示代码的使用账户get_self()，二级表示操作的账户地址scope）    
在eos内部系统使用了特别的数据库，来支持对数据的回滚，具体参考如下的两个对象在eos中的实现方法
``` C++
chainbase::database            db;
chainbase::database            reversible_blocks; ///< a special database to persist blocks that have successfully been applied but are still reversible
```
通过以上的操作，完成了智能合约内部到eos内部的数据共享，  
内存数据库估计需要改造下就能完成目标了squash函数，修改成如果从最远处执行，就能满足项目需求
#### gas模型