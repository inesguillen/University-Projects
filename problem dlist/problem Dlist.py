from dlist import DList
from dlist import DNode

class DList2:
    def isSorted(self):
        #checks if the list is sorted in ascending order (in this case it returns True). Otherwise, it must return False.
        if len(self)<=1:
            return True
        else:
            node1=self._head #node1=self.tail
            node2=node1.next #node2=node1.prev
            for i in range(1,len(self)):
                if node1.elem>node2.elem:
                    return False
                node1=node2
                node2=node2.next
            return True

    def isPalindrome(self):
        mid=len(self)//2
        left=self._head
        right=self._tail
        i=0
        isPal=True
        while i<mid and isPal:
            if left.elem!=right.elem:
                isPal=False
            left=left.next
            right=right.prev
            i+=1
        return isPal

    def removeDuplicatesSorted(self):
        if len(self)>1:
            prev=self._head
            nodeIt=prev.next

            while nodeIt: # is not None:
                if prev.elem==nodeIt.elem:
                    prev.next=nodeIt.next

                    if nodeIt.next:
                        nodeIt.next.prev=prev
                    else:
                        self.tail=prev
                    self.size-=1
                    nodeIt=nodeIt.next
                else:
                    prev=nodeIt
                    nodeIt=nodeIt.next




list=DList2()
for i in [2,4,7,3,43,6,23,5,2]:
    list.addFirst(i)
print(list.isSorted())











