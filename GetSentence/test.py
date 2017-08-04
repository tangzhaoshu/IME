#-*-coding:utf8 -*-
import urllib2  
import re
import time
import Queue
import socket
import sched



timeout = 5
sleep_download_time = 2
socket.setdefaulttimeout(timeout)


def get_CH(str):
    line = str.strip().decode('utf-8', 'ignore')
    p = re.compile(u'[\u4e00-\u9fa5]+')
    outStr = p.match(line)
    outStr1 = p.findall(line)
    return outStr1


def get_seturl(data, url):
    p = re.compile(r'<a[\s\S]&?href="(.*?)"[\s\S]*?target=')
    href = p.findall(data)
    set_url = set()
    for h in href:
        if re.match('http', h):
            set_url.add(h)
        else:
            h = url + h
            set_url.add(h)
  #  for u in set_url:
#     print u
    return set_url


def data_Wirte(data):
    f = open('test.txt', 'a')
    for d in data:
        f.write(d.encode('utf-8') + '\n')
    f.close



def process(url):
    data_count = 0
    queue = Queue.Queue()
    queue.put(url)
    url_set = set(url)
    url_html = set()
    print " start: " 
    while not queue.empty():
        q = queue.get()
        print time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) 
        print q
        time.sleep(sleep_download_time)
        try:
            response = urllib2.urlopen(q)
            html = response.read()
            response.close()
        except:
            continue
        data_list = get_CH(html)
        data_count += len(data_list)
        data_Wirte(data_list)
        url_html = get_seturl(html, q)
        for u in url_html:
            if u in url_set:
                pass
            else:
                url_set.add(u)
                queue.put(u)
        if data_count > 1000000:
            break
    

    
url = "http://www.gov.cn"
print "start"
process(url)