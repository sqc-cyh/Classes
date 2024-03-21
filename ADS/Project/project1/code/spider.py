#注：本程序主要通过文件流的形式进行爬取，从而避免某些全局变量的出现
#注：本程序爬取得到的所有信息均与本程序处于同名文件夹下

import requests
from bs4 import BeautifulSoup

headers = {"user-agent":'Mozilla/5.0(Windows NT 10.0; Win64; x64) AppleWebKit/537.36(KHTML,like Gecko) Chrome/86.0.4240.198 Safari/537.36'}
#上句为反反爬的一种操作，通过模拟浏览器访问实现html获取

#函数一：爬取基本信息，这个函数必须第一个执行，以更新文件流
def maindata():
    url = 'http://shakespeare.mit.edu/'
    response = requests.get(url, headers = headers)
    if (response.status_code != 200):
        print("request declined")#异常，有反爬装甲
    else:
        dstring = response.text

    f = open("data.txt", "w")#把主页的html存到文件里对比下
    f.write(dstring)
    f.close()
    f = open("data.txt", "r")
    dstring = f.read()
    f.close()
    soup = BeautifulSoup(dstring, 'lxml')
    passagesoup = soup.body
    tablesoup = passagesoup.find_all("table")
    targettable = tablesoup[1]
    targettr = targettable.find_all("tr")[1]#这里与网页本身有关，第一个tr是分类
    targeta = targettr.find_all("a")

    nameset = ["string"]
    nameset += [None] * 100
    f = open("firstaddress.txt", "w")
    for i in range(len(targeta)):
        f.write(url)
        f.write(targeta[i].attrs['href'])
        f.write("\n")
    f.write("#")
    f.close()
    for i in range(len(targeta)):
        nameset[i] = targeta[i].string
        nameset[i] = nameset[i].replace('\n', ' ').strip()
    articlenumber = len(targeta)
    namedata = open("namedata.txt", "w")
    for i in range(articlenumber):
        namedata.write(nameset[i])
        namedata.write('\n')
    namedata.close()
    print("the files are initialized succesfully.")

#现在已经获取了第一层的所有地址，接下来需要对这些地址继续爬取(即，章节名字)
#second = open("secondaddress.txt", "w")
#first = open("firstaddress.txt", "r")
#listofname = first.readlines()#这里面的字符串后面有回车，记得strip一下
#first.close()
#for i in range(articlenumber):
#url = listofname[0].strip()
#response = requests.get(url, headers = headers)
#if (response.status_code != 200):
#    print("request declined")#异常，有反爬装甲
#else:
#    sec_string = response.text
#sec_soup = BeautifulSoup(sec_string, 'lxml')
#sec_body = sec_soup.body
#print(list(enumerate(sec_body.p.next_siblings)))
#结果经过以上代码，发现其实二层地址下有一个full.html，加这个后缀就可以爬全书了，偷个懒ovo
#打完所有代码回来，偷懒成功，上面的东西基本没啥用了，但也可以留着

#为了方便，以下全局变量需被声明并定义
maindata()
namedata = open("namedata.txt", "r")
first = open("firstaddress.txt", "r")
listofname = namedata.readlines()#因为这里存文件流，所以还是保存\n了，避免排版太抽象
listofaddress = first.readlines()
namedata.close()
first.close()

def bookspider():
    for i in range(37):#一共有37个可以直接爬的书
        content = open(listofname[i].strip() + ".txt", "w")
        content.write(listofname[i])
        url = listofaddress[i][:-11] + "full.html"
        print(url)
        response = requests.get(url, headers = headers)
        if (response.status_code != 200):
            print("request declined")#异常，有反爬装甲
        else:
            dstring = response.text
        wholesoup = BeautifulSoup(dstring, 'lxml')
        body_soup = wholesoup.body
        a_soup = body_soup.find_all(["a", "i"])#a后是剧本，i后是出场/入场
        num_of_line = len(a_soup)
        for j in range(2, num_of_line):
            content.write(a_soup[j].string)
            content.write('\n')
    # 此时运行程序，会生成37个txt文档，每个文档都是一本书

#以下是十四行诗的爬虫，因为它本身结构不同，又要与其他诗歌区别开来
def sonnetspider():
    index = 37
    content = open(listofname[index].strip() + ".txt", "w")
    content.write(listofname[index])
    url = listofaddress[index].strip()
    print(listofname[index])
    print(url)
    response = requests.get(url, headers = headers)
    if (response.status_code != 200):
        print("request declined")
    else:
        dstring = response.text
    sonnetsoup = BeautifulSoup(dstring, 'lxml')
    secondaddress = sonnetsoup.body.find_all("a")
    for i in range(1, len(secondaddress)):
        url = "http://shakespeare.mit.edu/Poetry/" + secondaddress[i].attrs['href']
        response = requests.get(url, headers = headers)
        if (response.status_code != 200):
            print("request declined")
        else:
            dstring = response.text
        partsoup = BeautifulSoup(dstring, 'lxml')
        partstring = partsoup.body.find_all("blockquote")
        print(secondaddress[i].attrs['href'][:-5])
        content.write(secondaddress[i].attrs['href'][:-5])
        content.write('\n')
        for j in range(len(partstring)):
            content.write(partstring[j].text)


#以下是其他诗歌的爬虫
def poemspider():
    index = 38
    for i in range(index, len(listofname)):
        content = open(listofname[i].strip() + ".txt", "w")
        content.write(listofname[i])
        url = listofaddress[i].strip()
        print(url)
        response = requests.get(url, headers= headers)
        if (response.status_code != 200):
            print("request declined")
        else:
            dstring = response.text
        poetsoup = BeautifulSoup(dstring, 'lxml')
        poetcontent = poetsoup.body.find_all(['p', 'blockquote'])
        for j in range(len(poetcontent)):
            content.write(poetcontent[j].text)

print("please input the part you want to update: book/sonnet/poem/(exit: others)")
query = input()
if query == "book":
    bookspider()
elif query == "sonnet":
    sonnetspider()
elif query == "poem":
    poemspider()