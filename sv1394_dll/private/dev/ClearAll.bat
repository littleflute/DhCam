dir .\sdk /ad /b > sdk.txt

rem for /r %%a in (debug release) do @echo %%a

for /r %%a in (debug release obj objchk objfre) do  rd /s /q %%a

  del sdk.txt
