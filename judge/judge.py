#!/usr/bin/python3

from copy import deepcopy
import sys

end = None

def gotoRC(r,c):
  print("\033[{};{}f".format(r+3,c+1),end='')


def cls():
  print("\033[2J",end='')
  gotoRC(0,0)

  
def readBoard(fname):
  f=open(fname)
  H,W = f.readline().strip('\n').split()
  H = int(H)
  W = int(W)
  K = int(f.readline())
  D = int(f.readline())
  N = int(f.readline())
  t = []
  for _ in range(H):
    t.append(list(f.readline().strip('\n')))
  return (K,D,N,t)


def printBoard(t):
#  cls()
  H = len(t)
  W = len(t[0])
  for y in range(H):
    for x in range(W):
      print(t[y][x],end='')
    print()
  print()

    
def readSolution(fname, K):
  P = []
  f = open(fname)
  for i in range(K):
    P.append( f.readline().strip('\n') )
  return P



def getPositions( T ):
  W = len(T[0])
  H = len(T)

  pos = dict()

  for y in range(H):
    for x in range(W):
      if T[y][x].isalpha():
        pos[T[y][x]] = (x,y)
        T[y][x] = "."

  return pos


LETTER = "".join( [chr(ord('A')+i) for i in range(26)] )
letter = "".join( [chr(ord('a')+i) for i in range(26)] )

MOVE = dict()
MOVE["G"] = (0,-1)
MOVE["D"] = (0,1)
MOVE["P"] = (1,0)
MOVE["L"] = (-1,0)
MOVE["S"] = (0,0)


def dist(A,B):
  return ((A[0]-B[0])**2 + (A[1]-B[1])**2)**0.5
  
  

def correctPositions( pos, K, D ):
  for r1 in letter[:K]:
    for r2 in letter[:K]:
      if r1 == r2: continue
      if dist( pos[r1], pos[r2] ) <= D:
        return False
  return True


ERROR = dict()
ERROR[0  ] = "rozwiązanie poprawne"
ERROR[-1 ] = "roboty znalazly sie za blisko"
ERROR[-2 ] = "ktoras ze sciezek jest za dluga"
ERROR[-3 ] = "robot minal sie z drugim"
ERROR[-4 ] = "robot wyszedl poza plansze"
ERROR[-5 ] = "robot wszedł na przeszkodę"
ERROR[-10] = "po zakonczeniu roboty na blednych pozycjach"


def check( K, D, N, T, P, view ):

  W = len(T[0])
  H = len(T)

  # sprawdz dlugosc rozwiazania
  for i in range(K):
    if N < len(P[i]): return -2
    P[i] = P[i].ljust(N,"S")

  # odczytaj pozycje robotow
  pos = getPositions( T )

  # wykonuj kroki
  for i in range(len(P[0])):
    if view: print(f"-------- {i+1} -------")
    # sprawdz czy dwa roboty się mijaja
    pass_pos = deepcopy(pos)
    for r in range(K):
      pass_pos[letter[r]] = ( pass_pos[letter[r]][0] + 0.5*MOVE[P[r][i]][0],
                              pass_pos[letter[r]][1] + 0.5*MOVE[P[r][i]][1] )

    if not correctPositions( pass_pos, K, 0.1 ):
      return -3

    # wykonaj faktyczny krok
    for r in range(K):
      pos[letter[r]] = ( pos[letter[r]][0] + MOVE[P[r][i]][0],
                         pos[letter[r]][1] + MOVE[P[r][i]][1] )

    # sprawdz czy pozycja jest legalna
    for r in range(K):
      pp = pos[letter[r]]
      if pp[0] < 0: return -4
      if pp[1] < 0: return -4
      if pp[0] >= W: return -4
      if pp[1] >= H: return -4
      if T[pp[1]][pp[0]] == "#": return -5

    # sprawdz czy legalna pozycja (w szczegolnosci czy jeden nie wlazl drugiemu na glowe)
    if not correctPositions( pos, K, D ):
      return -1

    if view:
      TT = deepcopy(T)
      for i in range(K):
        L = LETTER[i]
        TT[pos[L][1]][pos[L][0]] = L
      for i in range(K):
        l = letter[i]
        TT[pos[l][1]][pos[l][0]] = l
      printBoard( TT )

  # sprawdz czy roboty na prawidlowych pozycjach
  for i in range(K):
    if pos[letter[i]] != pos[LETTER[i]]: return -10

  return 0



if __name__=="__main__":
  if len(sys.argv)<3:
    print("Wywołanie:\n   judge.py <infile> <outfile> [-v]")
  else:
    view = len(sys.argv)==4 and sys.argv[3]=='-v'

    try:
      (K,D,N,T) = readBoard(sys.argv[1])
      if view: printBoard( T )
      P = readSolution(sys.argv[2], K)

      ret = check( K, D, N, T, P, view )
    
      if ret==0:
        print('OK')
        exit(0)
      else:
        print('ERROR',ret, ERROR[ret])
        exit(1)
    except Exception as e:
      print("ERROR:",e)
      exit(2)
