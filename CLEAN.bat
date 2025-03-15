rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved
rmdir /s /q DerivedDataCache
rmdir /s /q Build
rmdir /s /q .vs
del **.opensdf
del **.sdf
del **.sln
del **.suo
del **.xcodeproj

for /f %%G in ('dir /s /b .\Plugins\*.uplugin') do rmdir /s /q %%~dpGBinaries
for /f %%G in ('dir /s /b .\Plugins\*.uplugin') do rmdir /s /q %%~dpGIntermediate

pause
