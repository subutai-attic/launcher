from subprocess import run, PIPE

#p = Popen(['/bin/cat', '/Users/mike/.vimrc'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
#stdout, stderr = p.communicate('')

p = run(['/bin/cat', '/Users/mike/.vimrc'], stdout=PIPE, stderr=PIPE, encoding="utf-8")

print("Out:")
print(p.stdout)

if p.stderr != b'':
    print("Err:")
    print(str(p.stderr))