echo Benchmark (4 - 50)

@echo off

:: Move to build path
pushd ..\build\win32

:: Create benchmark paths
set Output4="I:\Projects\Current\Programming\Morph\project\data\Output_4"
set Output25="I:\Projects\Current\Programming\Morph\project\data\Output_25"
set Output50="I:\Projects\Current\Programming\Morph\project\data\Output_50"

:: Create build path if it doesn't exist
if not exist %Output4% mkdir %Output4%
if not exist %Output25% mkdir %Output25%
if not exist %Output50% mkdir %Output50%

:: 4 Morph
::morph.exe I:\Projects\Current\Programming\Morph\project\data\BOOM_A I:\Projects\Current\Programming\Morph\project\data\BOOM_B 4 %Output4% 1 0.5 0.9

:: 25 Morph
::morph.exe I:\Projects\Current\Programming\Morph\project\data\BOOM_A I:\Projects\Current\Programming\Morph\project\data\BOOM_B 25 %Output25% 1 0.5 0.9

:: 50 Morph
morph.exe I:\Projects\Current\Programming\Morph\project\data\BOOM_A I:\Projects\Current\Programming\Morph\project\data\BOOM_B 50 %Output50% 1 0.5 0.9

popd