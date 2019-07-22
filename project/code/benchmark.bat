echo Benchmark (1 - 10)

@echo off

:: Move to build path
pushd ..\build\win32

:: Create benchmark paths
set Output1="I:\Projects\Current\Programming\Morph\project\data\Output_1"
set Output5="I:\Projects\Current\Programming\Morph\project\data\Output_5"
set Output10="I:\Projects\Current\Programming\Morph\project\data\Output_10"

:: Create build path if it doesn't exist
if not exist %Output1% mkdir %Output1%
if not exist %Output5% mkdir %Output5%
if not exist %Output10% mkdir %Output10%

:: 1 Morph
morph.exe I:\Projects\Current\Programming\Morph\project\data\BOOM_Bird I:\Projects\Current\Programming\Morph\project\data\BOOM_Tiger 1 %Output1% 1 0.5 0.9

:: 5 Morph
morph.exe I:\Projects\Current\Programming\Morph\project\data\BOOM_Bird I:\Projects\Current\Programming\Morph\project\data\BOOM_Tiger 5 %Output5% 1 0.5 0.9

:: 10 Morph
morph.exe I:\Projects\Current\Programming\Morph\project\data\BOOM_Bird I:\Projects\Current\Programming\Morph\project\data\BOOM_Tiger 10 %Output10% 1 0.5 0.9

popd