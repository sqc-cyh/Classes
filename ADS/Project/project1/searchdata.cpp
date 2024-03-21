#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#define TRUE 1
#define FALSE 0
using namespace std;
typedef string filename;
typedef int line;
int stem(char *p, int index, int position);
using namespace std;
string Word_Stem(string x);
int main(){
	vector<string> txt_name;/*used to store all the text in the database*/
	map<string,vector<pair<int,int>>> SearchEngine;/*used to store inverted indexes, in which the two ints in the pair store the book target number and the corresponding number of rows, respectively*/
	map<string,int> Frequency[50];/*used to store the frequency of lookup words in each book.*/
	string name="./docspider/namedata.txt";//*Read the local database file to get the individual article titles*/
	ifstream file_name(name);
	if (!file_name.is_open()){
		cout << "open file error!" << endl;
		return 0;
	}
	string str_read;
    while(getline(file_name,str_read)){
        txt_name.push_back(str_read);
	}
	/*Enter and determine whether it is stopword, and perform root processing at the same time*/
	string str; 
	double rate;
	cout<<"Please input your word and rate:"<<endl;
	cin>>str>>rate;
	string tmpl;
	ifstream stopword("StopWord.txt");
	int flag=1;
	while(getline(stopword,tmpl)){
	    if(str.compare(tmpl)==0) flag=0;
	}
	if(flag==0){
		cout<<"This is a stop word"<<endl;
		return 0;
	}
	str=Word_Stem(str);
	
	/*Check whether the local data has been queried*/
	string Exist_file="./data/";
	Exist_file+=str;
	Exist_file+="_data";
	Exist_file+=".txt";
	ifstream check_name(Exist_file);
	if (!check_name.is_open()){/*If there is no corresponding data locally, start querying the database*/
		for(int i=0;i<txt_name.size();i++){
		    string now_txt;
		    now_txt="./docspider/";
		    now_txt+=txt_name[i];
		    now_txt+=".txt";
		    ifstream file_name(now_txt);
		    if (!file_name.is_open()){
		        cout << "open file error!" << endl;
		        return 0;
	        }
	        string tmpn;
	        int line=0;
	        while(getline(file_name,tmpn)){/*Read by line, chunk by string*/
	        	line++;
	        	tmpn=Word_Stem(tmpn);/*Root processing for each word*/
	        	int index=tmpn.find(str);
	    	    if(index!=string::npos){
	    		    SearchEngine[str].push_back(make_pair(i,line));/*push into the data structure of the search results*/
	    		    Frequency[i][str]++;/*Frequency plus one*/
			    }
		    }
	    }
    	map<string,int> BookName;
		for (const auto& entry : SearchEngine) {/*Write to local txt file*/
			ofstream ofs; 
			string NewTxt="./data/";
			NewTxt+=str;
			NewTxt+="_data.txt";
		    ofs.open(NewTxt,ios::out);
		    string w=entry.first;
		    ofs<<entry.first;
		    for (const auto& subEntry : entry.second) {
		    	string book=txt_name[subEntry.first];
		    	if(BookName[book]==0){
		    		ofs<<endl;
		    		BookName[book]=1;
		    		ofs<<book<<" "<<Frequency[subEntry.first][w]<<endl;
				}
	            ofs<<subEntry.second<<" ";
		    }
		    ofs<<endl;
			BookName.clear();
	        ofs.close();
    	}
    	priority_queue<int> q;//*Use the small root heap to find the k-th largest element, k=rate*total number of articles*/
    	for(int i=0;i<txt_name.size();i++){
    		q.push(Frequency[i][str]);
		}
		for(int i=0;i<rate*txt_name.size();i++){
			q.pop();
		}
		int boundary=q.top();
	    for (const auto& entry : SearchEngine) {/*Output at terminal*/
		    string w=entry.first;
		    cout<<entry.first<<endl;
		    for(int i=0;i<txt_name.size();i++){
		    	if(Frequency[i][w]>=boundary) {
		    	    cout<<txt_name[i]<<" "<<Frequency[i][w]<<" The first appearance is in the ";
					for (const auto& subEntry : entry.second) {
						if(subEntry.first==i){
						    cout<<subEntry.second<<" line"<<endl;
	                        break;	
						} 
		            }	
				}
			}
			BookName.clear();
	    }
	}
	else{/*Existing local data, read local data directly*/
	    priority_queue<int> q;/*Use the small root heap to find the k-th largest element, k=rate*total number of articles*/
		int total=0;	
		string tmpn;
		getline(check_name,tmpn);
		while(getline(check_name,tmpn)){
			int k=atoi(&tmpn[0]);
			if(k==0) {
				total++;
				int sum,i;
				sum=0;
				i=tmpn.size()-1;
				string temp=tmpn;
				while(i>0){
					if(tmpn[i]==' ') break;
					i--;
				}
				temp=temp.substr(i+1, temp.length()-i);
				int fre=atoi(&temp[0]);
				q.push(fre);
			}
		} 
		for(int i=0;i<rate*total;i++){
			q.pop();
		}
		int boundary=q.top();
		ifstream check2_name(Exist_file);
		getline(check2_name,tmpn);
		while(getline(check2_name,tmpn)){/*to output*/
			int k=atoi(&tmpn[0]);
			if(k==0) {
				total++;
				int sum,i;
				sum=0;
				i=tmpn.size()-1;
				string temp=tmpn;
				while(i>0){
					if(tmpn[i]==' ') break;
					i--;
				}
				temp=temp.substr(i+1, temp.length()-i);
				int fre=atoi(&temp[0]);
				if(fre>boundary) {
				    cout<<tmpn<<" The first appearance is in the ";
				    string line;
				    getline(check2_name,line,' ');
				    cout<<line<<" line"<<endl;	
				}
			}
		}
	}
	return 0; 
}
string Word_Stem(string x){
	transform(x.begin(),x.end(),x.begin(),::tolower);
	char*tmp=&x[0];
	int end = stem(tmp, 0, strlen(tmp)-1);
	tmp[end + 1] = 0;
	x=tmp;
	return x;
}

/* This is the Porter stemming algorithm, coded up in ANSI C by the
 * author. It may be be regarded as canonical, in that it follows the
 * algorithm presented in
 *
 * Porter, 1980, An algorithm for suffix stripping, Program, Vol. 14,
 * no. 3, pp 130-137,
 *
 * only differing from it at the points marked --DEPARTURE-- below.
 *
 * See also http://www.tartarus.org/~martin/PorterStemmer
 *
 * The algorithm as described in the paper could be exactly replicated
 * by adjusting the points of DEPARTURE, but this is barely necessary,
 * because (a) the points of DEPARTURE are definitely improvements, and
 * (b) no encoding of the Porter stemmer I have seen is anything like
 * as exact as this version, even with the points of DEPARTURE!
 *
 * You can compile it on Unix with 'gcc -O3 -o stem stem.c' after which
 * 'stem' takes a list of inputs and sends the stemmed equivalent to
 * stdout.
 *
 * The algorithm as encoded here is particularly fast.
 *
 * Release 1: was many years ago
 * Release 2: 11 Apr 2013
 *     fixes a bug noted by Matt Patenaude <matt@mattpatenaude.com>,
 *
 *     case 'o': if (ends("\03" "ion") && (b[j] == 's' || b[j] == 't')) break;
 *         ==>
 *     case 'o': if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) break;
 *
 *     to avoid accessing b[k0-1] when the word in b is "ion".
 * Release 3: 25 Mar 2014
 *     fixes a similar bug noted by Klemens Baum <klemensbaum@gmail.com>,
 *     that if step1ab leaves a one letter result (ied -> i, aing -> a etc),
 *     step2 and step4 access the byte before the first letter. So we skip
 *     steps after step1ab unless k > k0. */

/* The main part of the stemming algorithm starts here. b is a buffer
 * holding a word to be stemmed. The letters are in b[k0], b[k0+1] ...
 * ending at b[k]. In fact k0 = 0 in this demo program. k is readjusted
 * downwards as the stemming progresses. Zero termination is not in fact
 * used in the algorithm.
 *
 * Note that only lower case sequences are stemmed. Forcing to lower case
 * should be done before stem(...) is called. */

/* buffer for word to be stemmed */
static char *b;

static int k;
static int k0;

/* j is a general offset into the string */
static int j;

/**
 * TRUE when `b[i]` is a consonant.
 */

static int
isConsonant(int index) {
  switch (b[index]) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return FALSE;
    case 'y':
      return (index == k0) ? TRUE : !isConsonant(index - 1);
    default:
      return TRUE;
  }
}

/* Measure the number of consonant sequences between
 * `k0` and `j`.  If C is a consonant sequence and V
 * a vowel sequence, and <..> indicates arbitrary
 * presence:
 *
 *   <C><V>       gives 0
 *   <C>VC<V>     gives 1
 *   <C>VCVC<V>   gives 2
 *   <C>VCVCVC<V> gives 3
 *   ....
 */
static int
getMeasure() {
  int position;
  int index;

  position = 0;
  index = k0;

  while (TRUE) {
    if (index > j) {
      return position;
    }

    if (!isConsonant(index)) {
      break;
    }

    index++;
  }

  index++;

  while (TRUE) {
    while (TRUE) {
      if (index > j) {
        return position;
      }

      if (isConsonant(index)) {
        break;
      }

      index++;
    }

    index++;
    position++;

    while (TRUE) {
      if (index > j) {
        return position;
      }

      if (!isConsonant(index)) {
        break;
      }

      index++;
    }

    index++;
  }
}

/* `TRUE` when `k0, ... j` contains a vowel. */
static int
vowelInStem() {
  int index;

  index = k0 - 1;

  while (++index <= j) {
    if (!isConsonant(index)) {
      return TRUE;
    }
  }

  return FALSE;
}

/* `TRUE` when `j` and `(j-1)` are the same consonant. */
static int
isDoubleConsonant(int index) {
  if (b[index] != b[index - 1]) {
    return FALSE;
  }

  return isConsonant(index);
}

/* `TRUE` when `i - 2, i - 1, i` has the form
 * `consonant - vowel - consonant` and also if the second
 * C is not `"w"`, `"x"`, or `"y"`. this is used when
 * trying to restore an `e` at the end of a short word.
 *
 * Such as:
 *
 * `cav(e)`, `lov(e)`, `hop(e)`, `crim(e)`, but `snow`,
 * `box`, `tray`.
 */
static int
cvc(int index) {
  int character;

  if (index < k0 + 2 || !isConsonant(index) || isConsonant(index - 1) || !isConsonant(index - 2)) {
    return FALSE;
  }

  character = b[index];

  if (character == 'w' || character == 'x' || character == 'y') {
    return FALSE;
  }

  return TRUE;
}

/* `ends(s)` is `TRUE` when `k0, ...k` ends with `value`. */
static int
ends(const char *value) {
  int length = value[0];

  /* Tiny speed-up. */
  if (value[length] != b[k]) {
    return FALSE;
  }

  if (length > k - k0 + 1) {
    return FALSE;
  }

  if (memcmp(b + k - length + 1, value + 1, length) != 0) {
    return FALSE;
  }

  j = k - length;

  return TRUE;
}

/* `setTo(value)` sets `(j + 1), ...k` to the characters in
 * `value`, readjusting `k`. */
static void
setTo(const char *value) {
  int length = value[0];

  memmove(b + j + 1, value + 1, length);

  k = j + length;
}

/* Set string. */
static void
replace(const char *value) {
  if (getMeasure() > 0) {
    setTo(value);
  }
}

/* `step1ab()` gets rid of plurals, `-ed`, `-ing`.
 *
 * Such as:
 *
 *   caresses  ->  caress
 *   ponies    ->  poni
 *   ties      ->  ti
 *   caress    ->  caress
 *   cats      ->  cat
 *
 *   feed      ->  feed
 *   agreed    ->  agree
 *   disabled  ->  disable
 *
 *   matting   ->  mat
 *   mating    ->  mate
 *   meeting   ->  meet
 *   milling   ->  mill
 *   messing   ->  mess
 *
 *   meetings  ->  meet
 */
static void
step1ab() {
  int character;

  if (b[k] == 's') {
    if (ends("\04" "sses")) {
      k -= 2;
    } else if (ends("\03" "ies")) {
      setTo("\01" "i");
    } else if (b[k - 1] != 's') {
      k--;
    }
  }

  if (ends("\03" "eed")) {
    if (getMeasure() > 0) {
      k--;
    }
  } else if ((ends("\02" "ed") || ends("\03" "ing")) && vowelInStem()) {
    k = j;

    if (ends("\02" "at")) {
      setTo("\03" "ate");
    } else if (ends("\02" "bl")) {
      setTo("\03" "ble");
    } else if (ends("\02" "iz")) {
      setTo("\03" "ize");
    } else if (isDoubleConsonant(k)) {
      k--;

      character = b[k];

      if (character == 'l' || character == 's' || character == 'z') {
        k++;
      }
    } else if (getMeasure() == 1 && cvc(k)) {
      setTo("\01" "e");
    }
  }
}

/* `step1c()` turns terminal `"y"` to `"i"` when there
 * is another vowel in the stem. */
static void
step1c() {
  if (ends("\01" "y") && vowelInStem()) {
    b[k] = 'i';
  }
}

/* `step2()` maps double suffices to single ones.
 * so -ization ( = -ize plus -ation) maps to -ize etc.
 * note that the string before the suffix must give
 * getMeasure() > 0. */
static void
step2() {
  switch (b[k - 1]) {
    case 'a':
      if (ends("\07" "ational")) {
        replace("\03" "ate");
        break;
      }

      if (ends("\06" "tional")) {
        replace("\04" "tion");
        break;
      }

      break;
    case 'c':
      if (ends("\04" "enci")) {
        replace("\04" "ence");
        break;
      }

      if (ends("\04" "anci")) {
        replace("\04" "ance");
        break;
      }

      break;
    case 'e':
      if (ends("\04" "izer")) {
        replace("\03" "ize");
        break;
      }

      break;
    case 'l':
      /* --DEPARTURE--: To match the published algorithm,
       * replace this line with:
       *
       * ```
       * if (ends("\04" "abli")) {
       *     replace("\04" "able");
       *
       *     break;
       * }
       * ```
       */
      if (ends("\03" "bli")) {
        replace("\03" "ble");
        break;
      }

      if (ends("\04" "alli")) {
        replace("\02" "al");
        break;
      }

      if (ends("\05" "entli")) {
        replace("\03" "ent");
        break;
      }

      if (ends("\03" "eli")) {
        replace("\01" "e");
        break;
      }

      if (ends("\05" "ousli")) {
        replace("\03" "ous");
        break;
      }

      break;
    case 'o':
      if (ends("\07" "ization")) {
        replace("\03" "ize");
        break;
      }

      if (ends("\05" "ation")) {
        replace("\03" "ate");
        break;
      }

      if (ends("\04" "ator")) {
        replace("\03" "ate");
        break;
      }

      break;
    case 's':
      if (ends("\05" "alism")) {
        replace("\02" "al");
        break;
      }

      if (ends("\07" "iveness")) {
        replace("\03" "ive");
        break;
      }

      if (ends("\07" "fulness")) {
        replace("\03" "ful");
        break;
      }

      if (ends("\07" "ousness")) {
        replace("\03" "ous");
        break;
      }

      break;
    case 't':
      if (ends("\05" "aliti")) {
        replace("\02" "al");
        break;
      }

      if (ends("\05" "iviti")) {
        replace("\03" "ive");
        break;
      }

      if (ends("\06" "biliti")) {
        replace("\03" "ble");
        break;
      }

      break;
    /* --DEPARTURE--: To match the published algorithm, delete this line. */
    case 'g':
      if (ends("\04" "logi")) {
        replace("\03" "log");
        break;
      }
  }
}

/* `step3()` deals with -ic-, -full, -ness etc.
 * similar strategy to step2. */
static void
step3() {
  switch (b[k]) {
    case 'e':
      if (ends("\05" "icate")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\05" "ative")) {
        replace("\00" "");
        break;
      }

      if (ends("\05" "alize")) {
        replace("\02" "al");
        break;
      }

      break;
    case 'i':
      if (ends("\05" "iciti")) {
        replace("\02" "ic");
        break;
      }

      break;
    case 'l':
      if (ends("\04" "ical")) {
        replace("\02" "ic");
        break;
      }

      if (ends("\03" "ful")) {
        replace("\00" "");
        break;
      }

      break;
    case 's':
      if (ends("\04" "ness")) {
        replace("\00" "");
        break;
      }

      break;
  }
}

/* `step4()` takes off -ant, -ence etc., in
 * context <c>vcvc<v>. */
static void
step4() {
  switch (b[k - 1]) {
    case 'a':
      if (ends("\02" "al")) {
        break;
      }

      return;
    case 'c':
      if (ends("\04" "ance")) {
        break;
      }

      if (ends("\04" "ence")) {
        break;
      }

      return;
    case 'e':
      if (ends("\02" "er")) {
        break;
      }

      return;
    case 'i':
      if (ends("\02" "ic")) {
        break;
      }

      return;
    case 'l':
      if (ends("\04" "able")) {
        break;
      }

      if (ends("\04" "ible")) {
        break;
      }

      return;
    case 'n':
      if (ends("\03" "ant")) {
        break;
      }

      if (ends("\05" "ement")) {
        break;
      }

      if (ends("\04" "ment")) {
        break;
      }

      if (ends("\03" "ent")) {
        break;
      }

      return;
    case 'o':
      if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) {
        break;
      }

      /* takes care of -ous */
      if (ends("\02" "ou")) {
        break;
      }

      return;
    case 's':
      if (ends("\03" "ism")) {
        break;
      }

      return;
    case 't':
      if (ends("\03" "ate")) {
        break;
      }

      if (ends("\03" "iti")) {
        break;
      }

      return;
    case 'u':
      if (ends("\03" "ous")) {
        break;
      }

      return;
    case 'v':
      if (ends("\03" "ive")) {
        break;
      }

      return;
    case 'z':
      if (ends("\03" "ize")) {
        break;
      }

      return;
    default:
      return;
  }

  if (getMeasure() > 1) {
    k = j;
  }
}

/* `step5()` removes a final `-e` if `getMeasure()` is
 * greater than `1`, and changes `-ll` to `-l` if
 * `getMeasure()` is greater than `1`. */
static void
step5() {
  int a;

  j = k;

  if (b[k] == 'e') {
    a = getMeasure();

    if (a > 1 || (a == 1 && !cvc(k - 1))) {
      k--;
    }
  }

  if (b[k] == 'l' && isDoubleConsonant(k) && getMeasure() > 1) {
    k--;
  }
}

/* In `stem(p, i, j)`, `p` is a `char` pointer, and the
 * string to be stemmed is from `p[i]` to
 * `p[j]` (inclusive).
 *
 * Typically, `i` is zero and `j` is the offset to the
 * last character of a string, `(p[j + 1] == '\0')`.
 * The stemmer adjusts the characters `p[i]` ... `p[j]`
 * and returns the new end-point of the string, `k`.
 *
 * Stemming never increases word length, so `i <= k <= j`.
 *
 * To turn the stemmer into a module, declare 'stem' as
 * extern, and delete the remainder of this file. */
int
stem(char *p, int index, int position) {
  /* Copy the parameters into statics. */
  b = p;
  k = position;
  k0 = index;

  if (k <= k0 + 1) {
    return k; /* --DEPARTURE-- */
  }

  /* With this line, strings of length 1 or 2 don't
   * go through the stemming process, although no
   * mention is made of this in the published
   * algorithm. Remove the line to match the published
   * algorithm. */
  step1ab();

  if (k > k0) {
    step1c();
    step2();
    step3();
    step4();
    step5();
  }

  return k;
}
