import subutai

def subutaistart():
    rc = subutai.HelloWorld()
    if rc[0] != 120:
        print("Tuple problem: != 120")
        return -1
    if rc[1] != "Hello, World!":
        printf("Tuple problem: != Hello, World!")
        return -2
