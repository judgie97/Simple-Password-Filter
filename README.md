# Simple Password Filter

Install on all domain controllers to prevent users settings passwords that include specific strings. This is useful to prevent common passwords like the company name or passwords that include "password".

## Compilation

* Make a folder called build 
* Switch into it in the msbuild tools command prompt and run the below.

```
cmake .. -G "Visual Studio 17 2022" -A x64
msbuild PasswordFilter.sln /p:Configuration=Release /p:Platform=x64
```

## Installation
* Copy the DLL from the build/Release/x64 folder to C:\Windows\System32
* Update registry key `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Lsa` Add `PasswordFilter` to the REG_MULTI_SZ `Notification Packages` 
* Create a file `C:\Windows\PasswordFilter.txt` with forbidden words, not case-sensitive.
* Restart server

See the [Microsoft documentation](https://learn.microsoft.com/en-us/windows/win32/secmgmt/installing-and-registering-a-password-filter-dll) on installing password filters for more details

## Compatibility
This was tested on Windows Server 2022 and 2019 domain controllers. The dll was compiled on the target operating system, caused crashes when compiled on a different version of windows. Also requires that the MSVC Redist 2015-2019 be installed on the domain controller.