项目在codeblocks环境开发
如果运行时出现乱码，大概率是字符编码设置出现了问题，按照以下方式设置可能会解决：
- settings -> General settings -> Encoding settings -> Using encoding when opening files:`UTF-8`
- Compiler setting -> Global compiler settings -> Compiler settings -> Other compiler options，添加如下部分
```
-finput-charset=UTF-8
-fexec-charset=GBK
```
> 项目各部分来源于网络，具体出处记不清楚了，如有雷同，谢谢贡献。
