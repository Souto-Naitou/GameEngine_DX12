Get-ChildItem -Path . -Recurse -Directory -Force -Filter ".vs"
pause
Get-ChildItem -Path "./" -Recurse -Directory -Force -Filter ".vs" | Remove-Item -Recurse -Force
pause

Get-ChildItem -Path . -Recurse -Directory -Force -Filter "Bin"
Pause
Get-ChildItem -Path "./" -Recurse -Directory -Force -Filter "Bin" | Remove-Item -Recurse -Force
Pause

Get-ChildItem -Path . -Recurse -Force -Filter "*.user"
Pause
Get-ChildItem -Path "./" -Recurse -Force -Filter "*.user" | Remove-Item -Recurse -Force
Pause