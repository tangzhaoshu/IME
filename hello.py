import Queue

data = set([1, 2, 3, 4])
queue = Queue.Queue()
for d in data:
    queue.put(d)
while not queue.empty():
    q = queue.get()
    if q == 1:
        queue.put(5)
    print q