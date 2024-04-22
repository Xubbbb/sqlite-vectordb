# SQLite VectorDB

### How to compile it?

This demo will load sqlite extension in C. Before you start this demo make sure that you have compiled the `vss-extension` for SQLite. You can obtain source code from https://github.com/asg017/sqlite-vss.git. And the tutorial for its compilation is in its repo. When you get `vector0.so` and `vss0.so` these 2 dynamic link library, you can replace the PATH to these 2 files in the main.c src code. Then you can just `make` to compile demo. 

### How to run it?

```shell
./app
# every time after run remember to make clean or change src code to implement other abilities
make clean
```

### What next?

I believe it's portable to OpenHarmony. You can try Native API of OH to make it runnable on RK3568 !

