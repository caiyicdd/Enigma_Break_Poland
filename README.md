说明：

初始转子、反射器、插线板设置见代码。

Enigma_crack_integration：Enigma密码机基本功能，设置转子初始位置，生成测试密钥（用于破解测试）

engima4_lj：生成26\*26\*26\*6=10万的圈字典，输出到文件rollMap1.txt

engima6.rej：破解程序，导入rollMap1.txt，输入测试密钥，得到当日初始转子顺序（日密钥）

