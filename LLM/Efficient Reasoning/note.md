## Make Long CoT Shorter

### SFT based

- TokenSkip: 针对CoT中不同token的语义重要性不同, 设置ration取得前ratio%重要的token来压缩CoT长度, 同时保证准确率. 
  - 作者利用语义重要性来压缩CoT. 是否还有别的方法来压缩CoT? 

