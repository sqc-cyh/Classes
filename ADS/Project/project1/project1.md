## Chapter 2:Data Structure / Algorithm Specificatio

### 1.Data Structure
The programming language we use is C++. The data of our program is mainly based on $map,vector,and \ pair$. 
```C++
vector<string> txt_name;
map<string,vector<pair<int,int>>> SearchEngine;
map<string,int> Frequency[50];
```
* **vector<string> txt_name** is the vector used to store all the text in the database.
* **map<string, vector<pair<int, int>>>** SearchEngine is a data structure used to store inverted indexes, in which the two ints in the pair store the book target number and the corresponding number of rows, respectively.
* **map<string,int> Frequency[50]** is a data structure used to store the frequency of lookup words in each book.

### 2. Descriptions of all the key algorithms:
* **Overview of the algorithm**:We first perform root processing and case conversion of the input words, and then use the inverted index to query the input words, store the results, and store the query results in a local txt file (the path is ./data./**word**_data.txt, where **word** is the name of the query word), so as to shorten the time for searching for the word for the second time.  

**Database reads** : Read the title of each article in the database and push it into **txt_name**.
```C++
    string name="./docspider/namedata.txt";/*We stored the title of the article in a txt text via a crawler*/
	ifstream file_name(name);
	if (!file_name.is_open()){
		cout << "open file error!" << endl;
		return 0;
	}
	string str_read;
    while(getline(file_name,str_read)){
        txt_name.push_back(str_read);
	}
```
**Word_Stem** : function is used to process the root word and convert it to case.We processed some of the roots by calling an external stem library.
```C++
string Word_Stem(string x){
	transform(x.begin(),x.end(),x.begin(),::tolower);
	char*tmp=&x[0];
	int end = stem(tmp, 0, strlen(tmp)-1);
	tmp[end + 1] = 0;
	x=tmp;
	return x;
}
```
**Input & Query** : We first read the **word** and the **ratio** to be queried, and then look for the existence of the relevant data file locally.At the same time, we first judge the query word to determine whether it is a **stop word**. We use a more standard deactivation thesaurus for comparison, which already exists locally and can be read from the file.
```C++
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
	string Exist_file="./data/";
	Exist_file+=str;
	Exist_file+="_data";
	Exist_file+=".txt";
	ifstream check_name(Exist_file);
    if (!check_name.is_open()){
        Traverse the database for queries and store inverted index data locally.
    }
    else{
        Read the data file for data processing.
    }
```
**Query method(Words that have not been searched)** : First, we read the name of the document locally, then open the corresponding document, demarcate each string with a space, and store the read words in the map and record it.The document index number and the position of the word in the document (number of lines). If the word Repeat, the number of repeated words in the word list is increased by one, and
The result of the final read is stored in the map.
```C++
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
		    	    cout<<txt_name[i]<<" "<<Frequency[i][w]<<" 第一次出现的位置在第";
					for (const auto& subEntry : entry.second) {
						if(subEntry.first==i){
						    cout<<subEntry.second<<"行"<<endl;
	                        break;	
						} 
		            }	
				}
			}
			BookName.clear();
	    }
```
**Query method(Words that have been searched)** :Reads the txt file where the word is stored locally. Since the storage format of the data file is fixed, the first line of each book block stores the title of the book and the frequency in the book, and the next few lines store the number of lines that appear, so we only need to iterate through each line of the txt file, extract the book title and its corresponding frequency, and output the book title and the number of lines where the word appears for the first time in the book.
```C++
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
				    cout<<tmpn<<" 第一次出现的位置在第";
				    string line;
				    getline(check2_name,line,' ');
				    cout<<line<<"行"<<endl;	
				}
			}
		}
```
## Chapter4 Analysis and Comments
### 1.Complexity analysis
**1)** For words that have not been searched:  
* **Time Complexity:** $O(M*L+\Phi+K)=O(M*L)$   
**K** is the sum of the number of times the file is read and the number of articles, **M** is the number of articles,**L** is the number of lines in the article,$\Phi$ is the sum of the time it takes to build the root heap and the number of outputs.   
* **Space Complexity:** $O(M*L+\Phi)$  
**M** is the number of articles,**L** is the number of lines in the article,$\Phi$ is the sum of the space it takes to build the root heap and the number of outputs.  

 **2)** For words that have been searched:  
* **Time Complexity:** $O(N)$  
**N** is the number of lines in the **word_data.txt**,and the $O(N)$ here is much smaller than the $O(M*L)$ above.Because all the desired results are already in the local text, it is only necessary to iterate through each line of the text and extract the data.
* **Space Complexity:** $O(\Phi)$  
$\Phi$ is the sum of the space it takes to build the root heap and the number of outputs.  Because all the desired results are already in the local text, it is only necessary to create a small root heap to filter the text of the output.

### 2.Program Highlights
It can be seen that when a word is searched for obsolete, the time and space cost of searching for the word again is greatly reduced, and in the face of massive data, the search speed will be faster and faster after the user's continuous search.


