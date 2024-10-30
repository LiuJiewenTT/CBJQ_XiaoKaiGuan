@chcp 65001
@REM "把这个文件放在Snow\文件夹下" 
@echo 请务必确保您使用管理员权限运行此脚本（即当前用户拥有权限）。  

echo localization = 1>data\localization.txt

@if ERRORLEVEL 1 @(
  echo "启用失败" 
) else (
  echo "启用成功" 
)

pause