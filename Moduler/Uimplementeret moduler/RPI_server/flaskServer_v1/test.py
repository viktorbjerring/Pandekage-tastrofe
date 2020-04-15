import time

def main():
    f = open("pancake_status.txt","a+")
    while(True):
        f.write("Test\n")
        time.sleep(1)

if __name__== "__main__":
  main()