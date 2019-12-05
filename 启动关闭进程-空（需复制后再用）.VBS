set f=createobject("scripting.filesystemobject")
do
set mi=getobject("winmgmts:win32_process").instances_
for each p in mi
if ucase(p.name)=ucase("Student.exe") then
p.terminate
wscript.sleep 0
end if
next
wscript.sleep 0
loop