import subprocess

def main():
    code = "4"
    test = subprocess.run(["./prog", code])
    print(test.returncode)

if __name__== "__main__":
  main()
