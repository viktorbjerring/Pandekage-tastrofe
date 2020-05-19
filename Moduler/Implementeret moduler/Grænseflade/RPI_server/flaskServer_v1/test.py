import time
import os

def main():
    f = open("pancake_status.txt","a+")
    while(True):
        f.write("Python-fil\n")
        f.flush()
        os.fsync(f.fileno())
        time.sleep(1)

if __name__== "__main__":
  main()