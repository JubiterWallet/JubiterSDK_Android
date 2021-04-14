# Introduction
JuBiter SDK Android is a demo for testing [JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git).

# Support Blockchains
Consistent with what is supported by the [JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git), refer to the [JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git) for details.

# Dependency
| Name | URL | Note |
| ---- | ---- | ---- |
| **JubiterSDK_C** | https://github.com/JubiterWallet/JubiterSDK_C.git | JuBiter SDK is what the demo tests. |

# Branch
+ **master**
  + Stable branch. If you are JuBiter SDK users, we recommend you to use this branch.
+ **2.3** and **dev_3.0**
  + Develop branch. JuBiter SDK developers use this branch.


# Requirements
+ Android Studio
+ CMake 3.0.0+
+ NDK 20+

# Compile

Download the project's dependencies libraries. Execute the following instructions at the root of the project.

```bash
git submodule update --init --recursive
```
# Publish
**Release**
+ Disable debug flag '**-DDEBUG**' in build.gradle file which is in the sdk module directory. Make sdk '**Build Variant**' to '**release**'.
  ```bash
  cppFlags "",
        "-std=c++11",
        // "-DDEBUG",
        "-DHAVE_ENDIAN_H"
  ```
**Debug**
+ '**-DDEBUG**' flag controls sdk's log, please ensure it is enable.