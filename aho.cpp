#include <algorithm>
#include <assert.h>
#include <bitset>
#include <complex>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <list>
#include <map>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <queue>
#include <set>
#include <stack>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace std;

typedef long long LL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<LL> VLL;
typedef vector<VLL> VVLL;
typedef pair<LL, LL> PLL;
typedef pair<LL, LL> PII;
typedef vector<PLL> VPLL;
typedef vector<std::string> VS;

#define arjun int main()
#define FOR(i, x, y) for (LL i = (x); i < (y); ++i)
#define FORO(it, c)                                                            \
  for (__typeof((c).begin()) it = (c).begin(); it != (c).end(); it++)
#define mp make_pair
#define fast                                                                   \
  ios_base::sync_with_stdio(false);                                            \
  cin.tie(NULL);

// tasks
// make failure function DONE
// make edges defined or not or make graph function DONE
// now make find pattern fucntion
// find number of alphabets DONE
// find total number of charecters in the pattern DONE
// storing the number of charecters in the input. DONE
std::map<char, int> m;  // DONE number of alphabet count created
std::map<int, char> rm; // reverse map
int alphabet_count = 0; // Stored
int num_charecter = 0;  // calculated number of charecter

// keep track of all declarations
int alreadyDefinedStates = 1;
bitset<100000> outputBit[110];

map<string, VPLL> ans;

// main search the strings function after
void searchWords(string text, string *s, int k, int **go, int *failure,
                 int *output) {
  int startState = 0;
  int chmap;
  int answerState = 0;

  FOR(i, 0, (LL)text.size()) {
    chmap = m[text[i]];
    answerState = startState;
    while (go[answerState][chmap] == -1) {
      answerState = failure[answerState];
    }
    startState = go[answerState][chmap];
    if (outputBit[startState] == 0)
      continue;
    FOR(j, 0, k) {
      if (outputBit[startState][j]) {
        ans[s[j]].push_back(mp(i - s[j].size() + 1, i));
      }
    }
  }
}

// failure function DONE
void computeFailure(string t, string *s, int k, int **go, vector<int> *depth,
                    int maxD, int *output) {
  // f(s) = 0 for all states of depth 1
  int *failure = new int[alreadyDefinedStates];
  FOR(i, 0, alreadyDefinedStates) { failure[i] = -1; }
  FORO(it, depth[1]) { failure[*it] = 0; }
  // now calculating failure function for other d's
  // failure f(r) where r belongs to depth d depends on s where s belongs to
  // depth (d-1)
  int f;
  int curr = 0;
  FOR(i, 1, maxD - 1) {
    // find all nodes at depth i
    FORO(it, depth[i]) {
      f = failure[*it]; // calculating failure of the current state as its
                        // already calculated
      curr = *it;
      FOR(j, 0, alphabet_count) {
        f = failure[*it]; // calculating failure of the current state as its
        if (go[*it][j] == -1)
          continue;
        while (go[f][j] == -1) {
          f = failure[f];
        }
        failure[go[curr][j]] = go[f][j];
        // taking union of the outputs
        outputBit[go[curr][j]] |= outputBit[go[f][j]];
      }
    }
  }
  searchWords(t, s, k, go, failure, output);
}

/* k = defined*/
void add_words(string t, string *s, int **go, int k, std::vector<int> *depth,
               int maxD) {
  int currS = 0;
  int ch = 0;
  depth[0].push_back(0);
  int *output = new int[num_charecter];
  FOR(i, 0, num_charecter) { output[i] = 0; }
  FOR(i, 0, k) {
    currS = 0;
    FOR(j, 0, (LL)s[i].length()) {
      ch = m[s[i][j]];
      if (go[currS][ch] == -1) {
        go[currS][ch] = alreadyDefinedStates++;
        depth[j + 1].push_back(alreadyDefinedStates - 1);
      }
      currS = go[currS][ch];
    }
    outputBit[currS][i] = 1;
  }
  FOR(i, 0, alphabet_count) {
    if (go[0][i] == -1) {
      go[0][i] = 0;
    }
  }
  // tasks completed
  // Depth is calculated
  // goto function is calculated
  // TODO:Compute failure function DONE
  computeFailure(t, s, k, go, depth, maxD, output);
}
// Function for all initialisations
void init(string t, string *s, int k, std::vector<int> *depth, int maxD) {
  int *go[num_charecter + 1];
  FOR(i, 0, num_charecter + 1) { go[i] = new int[alphabet_count]; }
  FOR(i, 0, num_charecter + 1) {
    FOR(j, 0, alphabet_count) { go[i][j] = -1; }
  }
  // make a trie!
  add_words(t, s, go, k, depth, maxD);
}

map<string, bool> ispresent;

// preprocess input
void solve() {
  string in;
  std::cout << "Give the path of input file(text file)"
            << "\n";
  al cin >> in;
  string out;
  std::cout << "Give the path of input file(pattern file)"
            << "\n";
  cin >> out;
  string text;
  ifstream infile(in.c_str());
  ifstream pattfile(out.c_str());

  getline(infile, text);
  // std::cout << text << "\n";
  int n;
  pattfile >> n;
  // std::cout << n << "\n";
  string *s = new string[n];
  set<char> alphabets;
  int number_of_charecters = 0;
  int prev_size = 0;
  int increse = 0; // mapping to map
  string s2;
  int defined = 0; // number of unique strings
  int maxD = -1;   // max depth of the trie
  FOR(i, 0, 110) { outputBit[i].reset(); }
  getline(pattfile, s2);
  FOR(i, 0, n) {
    getline(pattfile, s2);
    if (ispresent[s2])
      continue;
    ispresent[s2] = true;
    s[defined] = s2;
    defined++;
    // std::cout << s[defined - 1] << " " << defined - 1 << "\n";
    maxD = max(maxD, (int)s2.length());
    number_of_charecters += s2.length();
    FOR(i, 0, (LL)s2.length()) {
      prev_size = alphabets.size();
      alphabets.insert(s2[i]);
      if (alphabets.size() > prev_size) {
        m[s2[i]] = increse;
        rm[increse] = s2[i];
        increse++;
      }
    }
  }
  // take into consideration the alphabets of the text too
  FOR(i, 0, (LL)text.length()) {
    prev_size = alphabets.size();
    alphabets.insert(text[i]);
    if (alphabets.size() > prev_size) {
      m[text[i]] = increse;
      rm[increse] = text[i];
      increse++;
    }
  }
  alphabet_count = increse;
  num_charecter = number_of_charecters;
  // initialise a depth vector to be used after words for getting the depth of
  // the nodes
  std::vector<int> *depth =
      new std::vector<int>[maxD + 1]; // for storing the depths of the states
  init(text, s, defined, depth, maxD + 1);
  // printing the output
  FOR(i, 0, defined) {
    std::cout << "occurances of " << s[i] << " : " << ans[s[i]].size();
    if (ans[s[i]].size() > 0) {
      std::cout << "\nThe occurances are:"
                << "\n";
    }
    int occured = ans[s[i]].size();
    FORO(it, ans[s[i]]) {
      if (occured == 1)
        std::cout << (*it).first << " to " << (*it).second << endl;
      else {
        std::cout << (*it).first << " to " << (*it).second << " , ";
      }
      occured--;
    }
    std::cout << "\n";
  }
}

arjun {
  fast;
  solve();
  return 0;
}
