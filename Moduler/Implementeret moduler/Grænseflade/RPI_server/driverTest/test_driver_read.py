import time
import os

def main():
    with open('/dev/pan', 'r') as systemfile: 
        contents = systemfile.read()
        print(contents)
    with open('/dev/pan', 'r') as systemfile: 
        contents = systemfile.read()
        print(contents)

    with open('/dev/pan', 'w') as systemfile: 
        systemfile.write("1",)

if __name__== "__main__":
  main()