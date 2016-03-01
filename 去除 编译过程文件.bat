    :: file: 清除临时编译文件.bat  
    :: purpose: 清除临时编译文件  
    :: author: xue  
    :: time:2013.07.17  
      
    del *.map /S>>logs.txt>logs.txt  
    del *.trg /S>>logs.txt  
    del *.tlb /S>>logs.txt  
    del *.sym /S>>logs.txt  
    del *.vxd /S>>logs.txt  
    del *.386 /S>>logs.txt  
    del *.exp /S>>logs.txt  
    del *.pel /S>>logs.txt  
    del *.rsp /S>>logs.txt  
    del *.res /S>>logs.txt  
    del *.pdb /S>>logs.txt  
    del *.pch /S>>logs.txt  
    del *.sbr /S>>logs.txt  
    del *.bsc /S>>logs.txt  
    del *.plg /S>>logs.txt  
    del *.ncb /S>>logs.txt  
    del *.opt /S>>logs.txt  
    del *.aps /S>>logs.txt  
    del *.tmp /S>>logs.txt  
    del *.ilk /S>>logs.txt  
    del *.idb /S>>logs.txt  
    del *.sup /S>>logs.txt  
    del *.tlh /S>>logs.txt  
    del *.tli /S>>logs.txt  
	del *.hpj /S>>logs.txt 
	del *.mdp /S>>logs.txt
	del *.obj /S>>logs.txt
	del *.exp /S>>logs.txt
	del *.clw /S>>logs.txt
del *.ipch /S>>logs.txt
del *.sdf /S>>logs.txt
del *.cache /S>>logs.txt
del *.tlog /S>>logs.txt

	
    @for /F "delims=" %%i in ('dir *.obj /S /B^|find "\Release\"') do del "%%i">>logs.txt  
    @for /F "delims=" %%i in ('dir *.obj /S /B^|find "\Debug\"') do del "%%i">>logs.txt  
    @for /f "delims=" %%i in ('dir /S /B /AD^|find "\Release"' ) do rd /q "%%i">>logs.txt  
    @for /f "delims=" %%i in ('dir /S /B /AD^|find "\Debug"' ) do rd /q "%%i">>logs.txt  