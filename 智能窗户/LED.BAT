SET PATH=D:\MDK\ARM\ARMCC\Bin;D:\Program Files\VMware\bin\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;D:\Users\22629\Downloads\cygwin64 (2)\cygwin64\bin;C:\Program Files\Git\cmd;C:\Program Files\TortoiseGit\bin;C:\Program Files (x86)\NetSarang\Xshell 7\;C:\Users\22629\AppData\Local\Microsoft\WindowsApps;C:\Users\22629\AppData\Local\Programs\Microsoft VS Code Insiders\bin;D:\Microsoft VS Code\bin
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=LED
SET CPU_CLOCK=0x00B71B00
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\led.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\uart.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\delay.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\rtc.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\nettime.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\esp8266.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\mqtt.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\cjson.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\misc.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_adc.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_bkp.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_exti.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_gpio.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_iwdg.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_pwr.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rcc.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rtc.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_tim.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_usart.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f10x_md._ia"
"D:\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_stm32f10x.__i"
"D:\MDK\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\main.lnp"
"D:\MDK\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\main.axf" --i32combined --output ".\Objects\main.hex"
