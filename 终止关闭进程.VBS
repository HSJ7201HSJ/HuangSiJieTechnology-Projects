set f=createobject("scripting.filesystemobject")
set mi=getobject("winmgmts:win32_process").instances_
for each p in mi
if ucase(p.name)=ucase("wscript.exe") then
p.terminate
wscript.sleep 2000
end if
next
wscript.sleep 0