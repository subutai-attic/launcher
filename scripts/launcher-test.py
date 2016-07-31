import subutai

def subutaistart():
    print "Subutai Start"
    print subutai.hello()
    print subutai.debug()
    print subutai.version()
    return 0

def multiply(a,b):
    subutaistart()
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    return c
