Get-ChildItem -Filter bin/test_*.exe | ForEach {Write-Output "" "" Starting $_.Fullname; &$_.Fullname}